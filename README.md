# Projects During the Internship
A collection of embedded and IoT projects developed during my internship, including a Smart Payment Notification System and a Smart Door Lock System.
# Smart Payment Notification Speaker
Smart Payment Notification Speaker – An ESP32-based system that retrieves transaction data from a banking API and automatically announces new payments using text-to-speech while displaying the received amount on an 8-digit 7-segment LED.
<p align="center">
 <img width="400"  alt="image" src="https://github.com/user-attachments/assets/d5846774-206b-4ca9-83f7-76aa60b57b8e" />
    <br>
  Image 1.1. Front view of the PCB
</p>
<p align="center">
 <img width="400"  alt="image" src="https://github.com/user-attachments/assets/5ce2f249-f9d3-4142-be78-18669903d1eb" />
    <br>
  Image 1.2. Back view of the PCB
</p>
<p align="center">
 <img width="400" alt="image" src="https://github.com/user-attachments/assets/67d083bd-ef17-4da2-ac8e-66d18ce4470b" />
    <br>
  Image 1.3. Assembled speaker circuit displaying the received amount on the 7-segment LED
</p>
The image shows the assembled smart payment notification speaker system. An ESP32 development board is connected to several electronic modules and an 8-digit 7-segment LED display. The display shows the received transaction amount, while the speaker is connected to the system to provide audio notifications. The components are connected using jumper wires and powered through the circuit, demonstrating the completed hardware assembly of the smart payment notification system.
<p align="center">
 <img width="400"  alt="image" src="https://github.com/user-attachments/assets/16677581-1ca9-4057-9f74-b577864d15c5" />
    <br>
  Image 1.4. SePay interface displaying updated transaction information
</p>
The image shows the SePay transaction management interface, where the system displays a list of recent bank transactions. Each transaction includes information such as the transaction ID, bank account, amount, transaction type, time, and reference code. The interface is used to monitor and update transaction data, which allows the smart payment notification system to detect new incoming payments and process the received amount automatically.

# Smart Door Lock System
Smart Door Lock System – An Arduino-based smart door lock system that uses RFID cards and a keypad for access control. The system stores passwords and authorized card IDs in EEPROM, displays system status on a 20×4 LCD, and activates a door lock when valid credentials are detected. It also supports adding or removing RFID cards, changing passwords, and triggering an alarm after multiple failed access attempts.
<p align="center">
<img width="400"  alt="image" src="https://github.com/user-attachments/assets/d1594b96-83b1-4b6d-8c55-5d79b0f242fd" />
    <br>
  Image 1.1. Actual image of the system
</p>
The image shows an assembled Arduino-based smart door lock system designed for access control. The system consists of a microcontroller development board connected to a keypad, 20×4 LCD display, and RFID reader module. The keypad is used to enter passwords and control system functions, while the RFID module allows users to unlock the door using authorized RFID cards. The LCD displays system status and user prompts, and the electronic components are connected together using jumper wires and a custom circuit board. Overall, the image demonstrates the completed hardware setup of the smart door lock system.
<p align="center">
<img width="400"  alt="image" src="https://github.com/user-attachments/assets/820083bb-86a0-453b-99d6-811156e725c6" />
    <br>
  Image 1.2. Image of the circuit when the correct password is entered and the electric door lock is unlocked
</p>
<p align="center">
<img width="400"  alt="image" src="https://github.com/user-attachments/assets/28a45962-2b8b-4326-acbe-29d3c10bc80b" />
    <br>
  Image 1.3. Image of the circuit in password change mode
</p>
<p align="center">
<img width="400"  alt="image" src="https://github.com/user-attachments/assets/53685f7c-7f3c-4007-b0af-d04c82d185dc" />
    <br>
  Image 1.4. Image of the circuit while adding a new member RFID card
</p>
<p align="center">
<img width="400" alt="image" src="https://github.com/user-attachments/assets/10ad1a69-2891-4dd0-b1fe-dd138c8079b3" />
    <br>
  Image 1.5. Image of the circuit while deleting an RFID member card
</p>
<p align="center">
<img width="400"  alt="image" src="https://github.com/user-attachments/assets/d33522da-a90d-4769-84b1-84c7b78a5a15" />
    <br>
  Image 1.6. Image of the system displaying a theft warning and activating the buzzer after more than two incorrect password attempts
</p>
The image shows the 20×4 LCD display of the smart door lock system during a security alert condition. The LCD displays a warning message indicating that the password has been entered incorrectly, while the system triggers the buzzer alarm after multiple failed password attempts. This function helps improve the security of the door lock system by alerting users to repeated unauthorized access attempts.


