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
