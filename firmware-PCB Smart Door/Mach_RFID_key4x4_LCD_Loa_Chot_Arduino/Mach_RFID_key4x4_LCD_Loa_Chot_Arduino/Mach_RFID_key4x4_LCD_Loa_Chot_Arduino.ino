#include <EEPROM.h>       // bo nho nay luu pass khi mat dien
#include <Wire.h>     // chuan i2c cho lcd
#include <SPI.h>
#include <MFRC522.h> // thu vien "RFID".
#include <SoftwareSerial.h> 
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define SS_PIN  8
#define RST_PIN 7
MFRC522 mfrc522(SS_PIN, RST_PIN);       
unsigned long uidDec, uidDecTemp,uidtv; // hien thi so UID dang thap phan
byte bCounter, readBit;
unsigned long ticketNumber;

unsigned char  mp,i,j,chay,tt_mo,tt_them,ttt;
unsigned long    ng,tr,ch,dv;
unsigned long      thechu,thetv;

int h1 = A0;      // có hang cot theo nhe
int h2 = A1;
int h3 = A2;
int h4 = A3;

int c1 = 3;
int c2 = 4;
int c3 = 5;
int c4 = 6;

int chot = 2;

int loa = 10;

char kytu_sao[]    = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
char kytu[]        = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};       // 0x20 là khoang trang
char passmp[]      = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};      // lan dau tien la pass khoang trang
char pass_moi[]    = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
char pass_moi_ss[] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

char b;
int lan,sl,ttht;
int tt,tt_doi,ss,tt1;

void dong_cua();
void mo_cua();
void kiem_tra();
void kiem_tra_key_phim();
void kiem_tra_mo_cua();

char data,chayn,l;
char    dl[16]="";
int   nd;

void setup()
{
  Serial.begin(9600);
  lcd.begin();                      
  lcd.backlight(); 

  lcd.setCursor(0,0); 
  lcd.print(" HE THONG SMARTHOME ");
  
  lcd.setCursor(0,1); 
  lcd.print("NHAP: ");
  lcd.setCursor(0,2); 
  lcd.print("    MOI QUET THE    ");
  SPI.begin();        
  mfrc522.PCD_Init();   

  pinMode(loa, OUTPUT);
  pinMode(chot, OUTPUT);
 
  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);
  pinMode(c3, OUTPUT);
  pinMode(c4, OUTPUT);

  pinMode(h1, INPUT);
  pinMode(h2, INPUT);
  pinMode(h3, INPUT);
  pinMode(h4, INPUT);
  digitalWrite(loa,0);
  digitalWrite(chot,0);
  dong_cua();

  mp=0xff;
  tt=1;         // che do pass dung hoac sai
  tt_doi=0;     // cho phep doi pass
  lan=1;        // so lan doi pass
  sl=0;
  uidtv=0;
  tt_them=0;
  ttt=0;
  ttht=0;
  chayn=0;
  thechu=1908154140;
  thetv=0;
//  for(b=0;b<10;b++)          // luu gia tri pass moi vao eeprom  // nap lan dau tien thi mo len sau do an? di nap lai lan nua
//  {
//     EEPROM.write(b,pass_moi[b]);   // thi luc nay pass lan dau la day khoang trang
//  }
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,1); 
  lcd.print("NHAP: ");
  lcd.setCursor(0,0); 
  lcd.print("  MOI QUET THE  ");
  for(b=0;b<10;b++)     // doc gia tri tu eeprom vao
  {
     passmp[b]=EEPROM.read(b);  
  }

  ng=EEPROM.read(10);
  tr=EEPROM.read(11);
  ch=EEPROM.read(12);
  dv=EEPROM.read(13);
  thetv=ng*65536*256 + tr*65536 + ch*256 + dv; 
}

void dong_cua()
{

  digitalWrite(chot,0);         
}

void mo_cua()
{
  digitalWrite(chot,1);   
}

void them_the_tu()
{
  if(mfrc522.PICC_IsNewCardPresent())      // kiem tra co de the tu len khong
  { 
    if( mfrc522.PICC_ReadCardSerial())       // tien hanh doc the tu ve
    {
      uidDec = 0;
      for (byte i = 0; i < mfrc522.uid.size; i++)      // chieu dai ky tu la hàm mfrc522.uid.size // 0x20     // 0010 0000 = 2^8 = 256
      {                                               // moi lan tra ve 1 byte
        uidDecTemp = mfrc522.uid.uidByte[i];      // doc ve luu gia tri vao temp
        uidDec = uidDec*256+uidDecTemp;         // cong don gia tri.  vi byte thu nhat > byte la 2^8 = 256
      }  
      if(uidDec==thechu)            // day la ma the chu nen khong the them
      {
        lcd.setCursor(0,0);        //dinh vi toa do tren LCD
        lcd.print("   MA THE CHU   ");// hien thi chuoi tren LCD 
        delay(1000);
        lcd.setCursor(0,0);      //dinh vi toa do tren LCD
        lcd.print(" KHONG THE THEM ");// hien thi chuoi tren LCD
        tt_mo=0;
        chay=0;
        tt_them=0;
        delay(2000);
        lcd.setCursor(0,0);       //dinh vi toa do tren LCD
        lcd.print("  MOI QUET THE  ");// hien thi chuoi tren LCD 
        dong_cua();
      }
      else        // khong phai ma the chu tien hanh them
      { 
         ng=uidDec/65536/256; 
         tr=uidDec/65536%256;  
         ch=uidDec%65536/256; 
         dv=uidDec%65536%256;        
         EEPROM.write(10,ng);
         EEPROM.write(11,tr);
         EEPROM.write(12,ch);
         EEPROM.write(13,dv);
         ng=EEPROM.read(10);
         tr=EEPROM.read(11);
         ch=EEPROM.read(12);
         dv=EEPROM.read(13);
         thetv=ng*65536*256 + tr*65536 + ch*256 + dv;
         lcd.setCursor(0,0);        //dinh vi toa do tren LCD
         lcd.print("   THANH CONG   ");// hien thi chuoi tren LCD
         tt_mo=0;
         chay=0;
         tt_them=0;
         delay(2000);
         lcd.setCursor(0,0);       //dinh vi toa do tren LCD
         lcd.print("  MOI QUET THE  ");// hien thi chuoi tren LCD 
         dong_cua();
       }
        lcd.setCursor(0,1);
        lcd.print("                    ");
        lcd.setCursor(0,1); 
        lcd.print("NHAP: ");
    }
  }    
}
void xoa_the_tu()
{
  if(mfrc522.PICC_IsNewCardPresent())      // kiem tra co de the tu len khong
  { 
    if( mfrc522.PICC_ReadCardSerial())       // tien hanh doc the tu ve
    {
      uidDec = 0;
      for (byte i = 0; i < mfrc522.uid.size; i++)      // chieu dai ky tu la hàm mfrc522.uid.size // 0x20     // 0010 0000 = 2^8 = 256
      {                                               // moi lan tra ve 1 byte
        uidDecTemp = mfrc522.uid.uidByte[i];      // doc ve luu gia tri vao temp
        uidDec = uidDec*256+uidDecTemp;         // cong don gia tri.  vi byte thu nhat > byte la 2^8 = 256
      }
      if(uidDec==thechu)         // neu the2 chua duoc them hoac uid la the chu cung khong xoa duoc
      {
        lcd.setCursor(0,0);        //dinh vi toa do tren LCD
        lcd.print(" K XOA THE CHU  ");// hien thi chuoi tren LCD
        tt_mo=0;
        chay=0;
        tt_them=0;
        delay(1000);
        lcd.setCursor(0,0);       //dinh vi toa do tren LCD
        lcd.print("  MOI QUET THE  ");// hien thi chuoi tren LCD 
        dong_cua();
      }
      else if(thetv==0)         // neu the2 chua duoc them hoac uid la the chu cung khong xoa duoc
      {
        lcd.setCursor(0,0);       //dinh vi toa do tren LCD
        lcd.print(" CHUA THEM THE  ");// hien thi chuoi tren LCD 
        tt_mo=0;
        chay=0;
        tt_them=0;
        delay(1000);
        lcd.setCursor(0,0);       //dinh vi toa do tren LCD
        lcd.print("  MOI QUET THE  ");// hien thi chuoi tren LCD 
        dong_cua();
      }
      else          // nguoc lai
      {
         EEPROM.write(10,0);
         EEPROM.write(11,0);
         EEPROM.write(12,0);
         EEPROM.write(13,0);
         ng=EEPROM.read(10);
         tr=EEPROM.read(11);
         ch=EEPROM.read(12);
         dv=EEPROM.read(13);
         thetv=ng*65536*256 + tr*65536 + ch*256 + dv;
         
         lcd.setCursor(0,0);        //dinh vi toa do tren LCD
         lcd.print("   THANH CONG   ");// hien thi chuoi tren LCD
         tt_mo=0;
         chay=0;
         tt_them=0;
         delay(1000);
         lcd.setCursor(0,0);       //dinh vi toa do tren LCD
         lcd.print("  MOI QUET THE  ");// hien thi chuoi tren LCD 
         dong_cua();
      } 
        lcd.setCursor(0,1);
        lcd.print("                    ");
        lcd.setCursor(0,1); 
        lcd.print("NHAP: ");
    }
  }    
}

void kiem_tra()
{
  // Tim the moi
  if(mfrc522.PICC_IsNewCardPresent())      // kiem tra co de the tu len khong
  { 
    if(mfrc522.PICC_ReadCardSerial())       // tien hanh doc the tu ve
    {
      uidDec = 0;
      for (byte i = 0; i < mfrc522.uid.size; i++)      // chieu dai ky tu la hàm mfrc522.uid.size // 0x20     // 0010 0000 = 2^8 = 256
      {                                               // moi lan tra ve 1 byte
        uidDecTemp = mfrc522.uid.uidByte[i];      // doc ve luu gia tri vao temp
        uidDec = uidDec*256+uidDecTemp;         // cong don gia tri.  vi byte thu nhat > byte la 2^8 = 256
      }  
      Serial.println(uidDec);       
      if(uidDec == thechu)      // dung the   uid cua the chu
      {
         tt_mo=1;       // cho mo cua
         chay=0;
         tt_them=0;  
         sl=0;
         ttt=1;
         lcd.setCursor(0,0);       //dinh vi toa do tren LCD
         lcd.print("    DUNG THE    ");// hien thi chuoi tren LCD 
      }
      else if(uidDec == thetv)       // tuong tu the chu o tren
      {
         tt_mo=1;
         chay=0;
         tt_them=0;  
         sl=0;
         ttt=1;
         lcd.setCursor(0,0);       //dinh vi toa do tren LCD
         lcd.print("    DUNG THE    ");// hien thi chuoi tren LCD 
      }
      else
      {
          dong_cua();
          ttt=0;
          lcd.setCursor(0,0);       //dinh vi toa do tren LCD
          lcd.print("    SAI THE     ");// hien thi chuoi tren LCD 
          delay(1000);
          lcd.setCursor(0,0);       //dinh vi toa do tren LCD
          lcd.print("  MOI QUET THE  ");// hien thi chuoi tren LCD 
          sl++;            // moi lan quet sai 1 lan thi tang len 1 don vi
          if(sl>=3)      // qua 3 lan
          {
             canh_bao_sai();     // mo bao sai
             sl=0;
          }    
      }   
    }
  }
}

void dich_nhap()      // 
{
   for(b=0;b<9;b++)       // vong for dich du lieu nha. minh dich tu phai sang trai
   {
       kytu[b]=kytu[b+1];     // nen vi tri tu b = b+ 1
   }
   kytu[9]=mp+0x30;    // truyen mp o phia sau  // 0x30 chuyen ma thap phan sang ascii   // 

   for(b=0;b<9;b++)       // vong for dich du lieu nha. minh dich tu phai sang trai
   {
       kytu_sao[b]=kytu_sao[b+1];     // nen vi tri tu b = b+ 1
   }
   kytu_sao[9]=0x2a;    // truyen mp o phia sau  // 0x30 chuyen ma thap phan sang ascii   // 
   
  lcd.setCursor(6,1);   // cot 7 hang 2    vi minh co 10 ky tu
  for(b=0;b<10;b++)
  {
    lcd.write(kytu_sao[b]);
    delay(50);
  }
}

void dich_pass_doi()  // dich lan 1 pass moi
{
   for(b=0;b<9;b++)     // doi pass nhap lan 1 luu vao pass moi
   {
       pass_moi[b]=pass_moi[b+1];
   }
   pass_moi[9]=mp+0x30;
   
  lcd.setCursor(6,1);
  for(b=0;b<10;b++)
  {
    lcd.write(pass_moi[b]);
    delay(50);
  }
}

void dich_pass_doi_ss()
{
   for(b=0;b<9;b++)       // lan 2 luu vao pass moi ss
   {
       pass_moi_ss[b]=pass_moi_ss[b+1]; //so sanh mat khau vua doi
   }
   pass_moi_ss[9]=mp+0x30;        // giong nhap ky tu nhe
   
  lcd.setCursor(6,1);
  for(b=0;b<10;b++)
  {
    lcd.write(pass_moi_ss[b]);
    delay(50);
  }
}

void dich_xoa()
{
  if(tt_doi==0)     // xoa khi nhap pass
  {
    for(b=9;b>0;b--)      // xoa thi dich lui tu trai sang phai
    {
       kytu[b]=kytu[b-1];
       kytu_sao[b]=kytu_sao[b-1];
    }
    kytu[0]=0x20;     // 0x20 la khoang trang
    kytu_sao[0]=0x20;     // 0x20 la khoang trang
    
    lcd.setCursor(6,1);
     for(b=0;b<10;b++)
     {
       lcd.write(kytu_sao[b]);
       delay(50);
     }
  }
  else           // xoa khi doi pass
  {
    if(lan==1)           // xoa pass_moi      nhap pass 2 lan, chuong trinh ben duoi
    {
       for(b=9;b>0;b--)
       {
          pass_moi[b]=pass_moi[b-1];
       }
       pass_moi[0]=0x20;
       lcd.setCursor(6,1);
       for(b=0;b<10;b++)
       {
          lcd.write(pass_moi[b]);
          delay(50);
       }
    }
    else if(lan==2)         // xoa pass_moi_ss
    {
       for(b=9;b>0;b--)
       {
          pass_moi_ss[b]=pass_moi_ss[b-1];
       }
       pass_moi_ss[0]=0x20;
       lcd.setCursor(6,1);
       for(b=0;b<10;b++)
       {
          lcd.write(pass_moi_ss[b]);
          delay(50);
       }
    }
  }
}

void kiem_tra_mp()
{
  digitalWrite(c1, LOW);      // quet phim,  
  digitalWrite(c2, HIGH);   // cách quet se the nay, khi bat dau xuat 1 cot = 0 truoc, kiem tra 4 hang xem hang nao ve 0 nua,
  digitalWrite(c3, HIGH);    
  digitalWrite(c4, HIGH);
  if(digitalRead(h1)==0)        // hang 1 trả ve 0 vi doc adc nen anh so sanh <100  
  {
      delay(20);
      if(digitalRead(h1)==0)
      {
        mp=13; //        // tra ve mp = 1
      }
      while(digitalRead(h1)==0);   
  }
  
  if(digitalRead(h2)==0)
  {
      delay(20);
      if(digitalRead(h2)==0)
      {
        mp=15; //                // tuong tu cac truong hop khac
      }
      while(digitalRead(h2)==0);   
  }
  
  if(digitalRead(h3)==0)
  {
      delay(20);
      if(digitalRead(h3)==0)
      {
        mp=0; // 
      }
      while(digitalRead(h3)==0);  
  }
  if(digitalRead(h4)==0)
  {
      delay(20);
      if(digitalRead(h4)==0)
      {
        mp=14; // 
      }
      while(digitalRead(h4)==0);   
  }
  
  digitalWrite(c1, HIGH);
  digitalWrite(c2, LOW);        // cai nay xuat chan thu 2 ve 0
  digitalWrite(c3, HIGH);
  digitalWrite(c4, HIGH);
  if(digitalRead(h1)==0)        // hang 1 trả ve 0 vi doc adc nen anh so sanh <100  
  {
      delay(20);
      if(digitalRead(h1)==0)
      {
        mp=12; //        // tra ve mp = 1
      }
      while(digitalRead(h1)==0);   
  }
  
  if(digitalRead(h2)==0)
  {
      delay(20);
      if(digitalRead(h2)==0)
      {
        mp=9; //                // tuong tu cac truong hop khac
      }
      while(digitalRead(h2)==0);   
  }
  
  if(digitalRead(h3)==0)
  {
      delay(20);
      if(digitalRead(h3)==0)
      {
        mp=8; // 
      }
      while(digitalRead(h3)==0);   
  }
  if(digitalRead(h4)==0)
  {
      delay(20);
      if(digitalRead(h4)==0)
      {
        mp=7; // 
      }
      while(digitalRead(h4)==0);   
  }

  digitalWrite(c1, HIGH);
  digitalWrite(c2, HIGH);
  digitalWrite(c3, LOW);        // chan so 3 xuong 0
  digitalWrite(c4, HIGH);
  if(digitalRead(h1)==0)        // hang 1 trả ve 0 vi doc adc nen anh so sanh <100  
  {
      delay(20);
      if(digitalRead(h1)==0)
      {
        mp=11; //        // tra ve mp = 1
      }
      while(digitalRead(h1)==0);   
  }
  
  if(digitalRead(h2)==0)
  {
      delay(20);
      if(digitalRead(h2)==0)
      {
        mp=6; //                // tuong tu cac truong hop khac
      }
      while(digitalRead(h2)==0);   
  }
  
  if(digitalRead(h3)==0)
  {
      delay(20);
      if(digitalRead(h3)==0)
      {
        mp=5; // 
      }
      while(digitalRead(h3)==0);   
  }
  if(digitalRead(h4)==0)
  {
      delay(20);
      if(digitalRead(h4)==0)
      {
        mp=4; // 
      }
      while(digitalRead(h4)==0);   
  }
  
  digitalWrite(c1, HIGH);
  digitalWrite(c2, HIGH);
  digitalWrite(c3, HIGH);
  digitalWrite(c4, LOW);
  if(digitalRead(h1)==0)        // hang 1 trả ve 0 vi doc adc nen anh so sanh <100  
  {
      delay(20);
      if(digitalRead(h1)==0)
      {
        mp=10; //        // tra ve mp = 1
      }
      while(digitalRead(h1)==0);   
  }
  
  if(digitalRead(h2)==0)
  {
      delay(20);
      if(digitalRead(h2)==0)
      {
        mp=3; //                // tuong tu cac truong hop khac
      }
      while(digitalRead(h2)==0);   
  }
  
  if(digitalRead(h3)==0)
  {
      delay(20);
      if(digitalRead(h3)==0)
      {
        mp=2; // 
      }
      while(digitalRead(h3)==0);   
  }
  if(digitalRead(h4)==0)
  {
      delay(20);
      if(digitalRead(h4)==0)
      {
        mp=1; // 
      }
      while(digitalRead(h4)==0);   
  }
}

void test_mp()
{
  kiem_tra_mp();
  if(mp!=0xff)
  {
    Serial.println(mp);  
    mp=0xff;
  }  
}

void canh_bao_sai()
{
  for(int a=10;a>=0;a--)       // chinh thoi gian dem giay hien dan de 7s
  {
    digitalWrite(loa,1);
    lcd.setCursor(0,0);
    lcd.print("CO TROM     :");
    lcd.write(a/10+0X30);
    lcd.write(a%10+0X30);
    lcd.print("s");
    delay(1000);
  }
  digitalWrite(loa,0);
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1); 
  lcd.print("NHAP:");
  lcd.setCursor(0,0); 
  lcd.print("  MOI QUET THE  ");
  sl=0; 
}

void kiem_tra_key_phim()      // chuong trinh quan trong 
{
  kiem_tra_mp();        // doc mp o tren ve nhé
  if(mp!=0xff)
  {
    Serial.println(mp);  
    if(tt_mo==1)
    {
      if(mp==14)
      {
        tt_them++;
        if(tt_them>2)     tt_them=0;  
      }  
    }
    if(tt_doi==0)    // nhap pass mo cua
    {
       if(mp<10)        // co phep nhap tu 0 den 9
       {
          dich_nhap();    // dich du lieu vao mang kytu  xem lai cho nay kỹ ti nhé
       }
       else if(mp==15)          // nut #
       {
          for(b=0;b<10;b++)     
          {
             if(kytu[b]!=passmp[b])     // so sanh pass gom 10 ky tu
             {
                tt=1;             // bao sai 
                break;
             }
             else
             {
                tt=0;       // bao dung pass
             }
          }
          
          if(tt==1)   // nhay bao sai xuong day
          {
            lcd.setCursor(0,1);        
            lcd.print("  SAI MAT KHAU  ");
            delay(1000);
            lcd.setCursor(0,1);
            lcd.print("                ");
            lcd.setCursor(0,1); 
            lcd.print("NHAP: ");
            tt_mo=0;
            sl++;        // kiem tra sai
            if(sl>=3)      // neu qua 3 lan
            {
              canh_bao_sai();        // dem giay bao chuong
              lcd.setCursor(0,1);
              lcd.print("                ");
              lcd.setCursor(0,1); 
              lcd.print("NHAP: ");
            }
          }
          else   // tt=0  bao dung
          {
             lcd.setCursor(0,1);
             lcd.print(" DUNG MAT KHAU  ");
             delay(1000);
             lcd.setCursor(0,1);
             lcd.print("  XIN MOI VAO   ");
             chay=0;
             tt_mo=1;  
             sl=0;
          }
          
          for(b=0;b<10;b++)       // xoa luon mang ky tu ve khoang trang de bat dau lai tu dau
          {
             kytu[b]=0x20;   // xoa ky tu cu~
             kytu_sao[b]=0x20;
          }
       }
       else if(mp==10)            // cho phep doi pass
       {
          mo_cua();
          lcd.setCursor(0,1);
          lcd.print("   XIN MOI RA   ");
          delay(2000);
          dong_cua();
          lcd.setCursor(0,1); 
          lcd.print("NHAP:           ");
       }
       else if(mp==11)            // cho phep doi pass
       {

       }
       else if(mp==13)            // cho phep doi pass
       {
          if(tt==0)             // khi nhap pass thanh cong. kiem tra lai ham tren nhé
          {
            for(b=0;b<10;b++)           // xoa ky tu chuyen sang che do doi pass
            {
              pass_moi[b]=0x20;   // xoa ky tu cu~
              pass_moi_ss[b]=0x20;   // xoa ky tu cu~
              kytu[b]=0x20;   // xoa ky tu cu~
              kytu_sao[b]=0x20;   // xoa ky tu cu~
            }
            tt_doi=1;            /// cho tt doi = 1 tuc la chuyen sang doi pass
            lan=1;
            lcd.setCursor(0,1);
            lcd.print("MOI DOI MAT KHAU");
            delay(1000);
            lcd.setCursor(0,1);
            lcd.print("LAN 1:          ");
          }
       }
       else if(mp==12)         // xoa 1 ky tu 
       {
          dich_xoa();         // dich chuong trinh xoa o tren
       }
    }
    else        // doi pass   tt_doi=1;
    {
       if(mp==13)     // chuyen tu doi pass sang nhap pass neu khong muon doi
       {
          tt_doi=0;        // cho nhap pass
          tt=1;         // khong cho doi pass
          tt_mo=1;
          chay=98;
          for(b=0;b<10;b++)           // xoa ky tu chuyen sang che do doi pass
          {
            pass_moi[b]=0x20;   // xoa ky tu cu~
            pass_moi_ss[b]=0x20;   // xoa ky tu cu~
            kytu[b]=0x20;   // xoa ky tu cu~
            kytu_sao[b]=0x20;   // xoa ky tu cu~
          }
          lan=0;
          lcd.setCursor(0,1); 
          lcd.print("NHAP:           ");
          lcd.setCursor(0,0);       //dinh vi toa do tren LCD
          lcd.print("  MOI QUET THE  ");// hien thi chuoi tren LCD 
       }
       
       if(lan==1)      // nhap pass moi lan 1
       {
          lcd.setCursor(0,1);
          lcd.print("LAN 1: ");
          if(mp<10)         // lan 1 nen dich vao pass moi. xem chuong trinh o tren 
          { 
             dich_pass_doi();       // dich vao pass moi
          }
          else if(mp==15)     // khi nhan # thi tien hanh
          {
             lan=2;
             lcd.setCursor(0,1);
             lcd.print("LAN 2:          ");
          }
          else if(mp==12)       // 
          {
             dich_xoa();
          }
       }
       else if(lan==2)      // khi nhan # chuyen sang lan = 2
       {
          lcd.setCursor(0,1);
          lcd.print("LAN 2: ");      // 
          
          if(mp<10)       // luc nay nhap vao mang de so sanh
          {
             dich_pass_doi_ss();      // nhap vao mang 
          }
          else if(mp==15)   // nhan # lan nua
          {
            lan=1;
            for(b=0;b<10;b++)
            {
              if(pass_moi[b]!=pass_moi_ss[b])       // tien hanh so sanh 2 mang vua nhap, chi can 1 trong 10 vi tri co 1 vi tri sai
              {
                 ss=1;             // bao sai 
                 break;           // ket thuc khong di so sanh nua
              }
              else
              {
                 ss=0;        // bao dung
              }
            }
            if(ss==0)      // neu dung thi
            {  
              lcd.setCursor(0,1);
              lcd.print(" DOI THANH CONG ");
              for(b=0;b<10;b++)          // luu gia tri pass moi vao eeprom
              {
                 EEPROM.write(b,pass_moi[b]);       // luu pass moi vao eeprom phong khi mat dien
              }
              for(b=0;b<10;b++)     // doc gia tri tu eeprom vao
              {
                 passmp[b]=EEPROM.read(b);  // DOC MAT KHAU DUC LUU TRONG EEPROM
              }
            }
            else        // ss=1      neu sai thi bao that bai 
            {
              lcd.setCursor(0,1);
              lcd.print("DOI MK THAT BAI ");
            }
            delay(1000);
            dong_cua();
            tt_doi=0;
            tt=1;
            tt_mo=1;
            chay=98;
            for(b=0;b<10;b++)
            {
              pass_moi[b]=0x20;   // xoa ky tu cu~
              pass_moi_ss[b]=0x20;   // xoa ky tu cu~
            }
            lcd.setCursor(0,1); 
            lcd.print("NHAP:           ");
            lcd.setCursor(0,0);       //dinh vi toa do tren LCD
            lcd.print("  MOI QUET THE  ");// hien thi chuoi tren LCD 
          }
          else if(mp==12)       // tuong tu tren
          {
             dich_xoa();
          }
       }
    }
    mp=0xff;  
  } 
}

void loop()
{
  //test_mp();      // ham nay test de biet vi tri key phim thuc te. 
  if(tt_mo==1)         // dung the
  {
     kiem_tra_key_phim(); 
     if(tt_them==1)           // cho phep them the
     {
        lcd.setCursor(0,0);       //dinh vi toa do tren LCD
        lcd.print("THEM THANH VIEN ");// hien thi chuoi tren LCD 
        them_the_tu();         
     }
     else if(tt_them==2)        // cheo phep xoa th
     {
        lcd.setCursor(0,0);       //dinh vi toa do tren LCD
        lcd.print(" XOA THANH VIEN ");// hien thi chuoi tren LCD 
        xoa_the_tu();         
     }
     else if(tt_doi==1)
     {
         
     }
     else     // tt_them=0, khong them the
     {
        chay++;
        if(chay<100)       // sau co 10 lan dem neu khong them hoac xoa the thi 
        {
           lcd.setCursor(0,0);       //dinh vi toa do tren LCD
           lcd.print("     CUA MO     ");// hien thi chuoi tren LCD  
           lcd.setCursor(0,1);       //dinh vi toa do tren LCD
           lcd.print("   XIN MOI VAO  ");// hien thi chuoi tren LCD 
           mo_cua();
        }
        else        // sau do 
        {
           tt_mo=0;     // chot lai
           chay=101;
           tt_them=0;     // chuan bi chu ky moi
           dong_cua();
           lcd.setCursor(0,1); 
           lcd.print("NHAP:           ");
           lcd.setCursor(0,0);       //dinh vi toa do tren LCD
           lcd.print("  MOI QUET THE  ");// hien thi chuoi tren LCD 
        }
     }
  }  
  else        // neu tt_mo=0;
  {
    kiem_tra();       // kiem tra the tu
    kiem_tra_key_phim();
    lcd.setCursor(0,0);       //dinh vi toa do tren LCD
    lcd.print("  MOI QUET THE  ");// hien thi chuoi tren LCD 
    chay=0;
    tt_them=0;
    tt_doi=0;
  }
}
