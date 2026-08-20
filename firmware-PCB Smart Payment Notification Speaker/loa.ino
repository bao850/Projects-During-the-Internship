#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <driver/i2s.h>
#include <Audio.h>
// Thông tin kết nối WiFi và API
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
String api_token = "YOUR_SEPAY_API_TOKEN"; // Thay thế bằng API token của bạn
String accountNumber = "YOUR_ACCOUNT_NUMBER";//Thay thế bằng account của bạn

// Cấu hình I2S
#define I2S_DOUT 27 // Thay đổi chân tùy theo thiết bị của bạn
#define I2S_BCLK 26
#define I2S_LRC  25
// Biến toàn cục
Audio audio;
unsigned long timeGetData = 0;
String transactionDate = "";
long amountIn = 0;
// Thông tin tài khoản
String accountName = "YOUR_ACCOUNT_NAME";  // Tên tài khoản
String bankName = "BIDV";         // Tên ngân hàng
bool isSpeaking = false; // Biến cờ kiểm tra xem loa có đang phát không
String lastTransactionId = "";  // Thay cho transactionDate
String pendingSpeech = "";
long pendingAmount = 0;
//---------------------7 SEGMENT LED-------------------------
// Chân kết nối 74HC595
#define DATA_PIN 4    // Chân SER (DS)
#define CLOCK_PIN 2   // Chân SRCLK (SH_CP)
#define LATCH_PIN 15  // Chân RCLK (ST_CP)
// Mã segment cho các số 0-9, giả sử common cathode
const byte digitCode[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};
byte digitsToShow[8] = {0};  // Lưu dữ liệu 8 chữ số để quét
int currentDigit = 0;
unsigned long lastUpdate = 0;
void clearDisplay() {
  digitalWrite(LATCH_PIN, LOW);
  for (int i = 0; i < 8; i++) {
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0); // segment = 0
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0xFF); // chọn digit không hiển thị (active LOW)
  }
  digitalWrite(LATCH_PIN, HIGH);
}
void prepareDigits(long number) {
  memset(digitsToShow, 0, sizeof(digitsToShow)); // Reset tất cả digits

  if (number == 0) {
    digitsToShow[7] = digitCode[0]; // Hiển thị số 0 ở digit cuối cùng
    return;
  }

  int index = 7;
  while (number > 0 && index >= 0) {
    int digit = number % 10;
    digitsToShow[index] = digitCode[digit];
    number /= 10;
    index--;
  }
}


void updateDisplay() {
  if (millis() - lastUpdate < 2) return;
  lastUpdate = millis();

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitsToShow[currentDigit]);   // SEGMENT trước
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ~(1 << currentDigit));         // DIGIT sau (active LOW)
  digitalWrite(LATCH_PIN, HIGH);

  currentDigit = (currentDigit + 1) % 8;
}
void setup() {
  Serial.begin(115200);
  delay(1000);

pinMode(DATA_PIN, OUTPUT);
pinMode(LATCH_PIN, OUTPUT);
pinMode(CLOCK_PIN, OUTPUT);
 clearDisplay(); // Thêm dòng này
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

 

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(15);
  audio.connecttospeech("Xin chào, đây là loa thông báo chuyển khoản tự động! Hệ thống đã khởi động xong!", "vi");

  Serial.println("Nhập số tiền (ví dụ: 15000000):");
  timeGetData = millis();
}

void loop() {
   updateDisplay();
   // Xử lý nhập số tiền qua Serial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    long number = input.toInt();
    Serial.print("Đọc số: ");
    Serial.println(number);
    String str = "Thanh toán thành công, đã nhận " + String(number) + " đồng";
    isSpeaking = true; // Đặt cờ khi bắt đầu phát
    audio.connecttospeech(str.c_str(), "vi");
    prepareDigits(number); // Hiển thị số nhập lên LED 7 đoạn
    Serial.println("Nhập số tiền mới:");
  }

  // Cập nhật dữ liệu giao dịch mới từ API nếu loa không đang nói và đủ thời gian chờ
  if (!isSpeaking && millis() - timeGetData > 6000) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String apiUrl = "https://my.sepay.vn/userapi/transactions/list?account_number=" + accountNumber + "&limit=5";
    http.begin(apiUrl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + String(api_token));

    int httpCode = http.GET();
    Serial.println("HTTP CODE: " + String(httpCode));

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Received JSON:");
      Serial.println(payload);

      StaticJsonDocument<2048> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("JSON Parsing failed: ");
        Serial.println(error.f_str());
        http.end();
        return;
      }

      JsonArray transactions = doc["transactions"].as<JsonArray>();

      if (transactions.size() > 0) {
        JsonObject transaction = transactions[0];
        String newTransactionId = transaction["id"].as<String>();
        String amountStr = transaction["amount_in"].as<String>();
        long newAmountIn = (long) amountStr.toFloat();

        Serial.print("Parsed amount_in = ");
        Serial.println(newAmountIn);

        if (newTransactionId != lastTransactionId) {
          lastTransactionId = newTransactionId;
          pendingAmount = newAmountIn;
          pendingSpeech = "Thanh toán thành công, đã nhận " + String(newAmountIn) + " đồng";
        }
      }
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
  timeGetData = millis();
}


  
  // Xử lý phát âm thanh
audio.loop();
if (!audio.isRunning()) {
  if (pendingSpeech != "") {
    isSpeaking = true;
    prepareDigits(pendingAmount);
    audio.connecttospeech(pendingSpeech.c_str(), "vi");
    pendingSpeech = "";
  } else {
    isSpeaking = false;
  }
}
}


