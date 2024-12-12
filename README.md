# CS268BluetoothAlarm
Class project for CS268 Mobile Networks, an Arduino implementation using nrf24 library to sound a speaker alarm if connection is poor.

## Deliverables
Within the Project is three files, Transmitter, Receiver, and pitches header. This project requires 2 Arduino Uno modules. One as a transmitter and another as the receiver.

## Setup
The transmitter only requires the nRF24L01+PA+LNA transreceiver module and the Arduino Uno to run the code. The pins of the Arduino Uno can be connected in the following diagram:

<img width="291" alt="Screenshot 2024-12-11 at 7 23 22 PM" src="https://github.com/user-attachments/assets/ad09c157-5e1a-4de5-9ff4-6e88e3338fb1" />

The receiver is connected to the Arduino Uno in the same manner however a speaker is also connected througha breadboard. This speaker is connected simply using a power and ground.

Although unnecessary, I found it helpful to connect the Arduino UNO to a module with an LED light. This is done by connecting 2 100omh resistors in series and creating a circuit with the 5V power output from the Arduino UNO. Likewise, the nRF24L01 module performs inconsistently due to the poor power supply from the 3.3V output of certain Arduinos. If this occurs, a 10uF capacitor can be directly sodered onto the pins of the nRF24L01 module. 

![Breadboard | width=300](https://github.com/user-attachments/assets/2f69ac6c-e5bc-4288-b0bc-a116df530b70)


## Changables
Within the transmitter, there are a few varaibles that can be changed to increase or decrease the messaging cooldown time, the number of messages sent, and the time between each message. The message cooldown is found as a global variable named "cooldown". The number of messages is the first parameter to the START_PROTOCOL function. The second parameter is the LED delay time and therefore the delay between each message as well.

Within the receiver, we can first address the alarm. At the top, the number of notes, note duration, and melody can all be changed. Note that the melody should correspond with the notes from the pitches.h file. Like the transmitter, we can adjust the timeout timer and the LED delay between receiving messages. The receiver should have a message buffer and therefore the LED delay should only effect the timeout function and not the actual reception of the antenna. These two parameters can be changed in the RECEIVE_PROTOCOL function with the first parameter being the timeout and the second parameter being the delay.
