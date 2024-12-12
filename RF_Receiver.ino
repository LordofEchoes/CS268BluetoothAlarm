#include "Arduino.h"
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include "pitches.h"

// Some of the code used here can be found in this tutorial:
// https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/

// pin constants
int const LEDPIN = 4;
int const SPEAKERPIN = 2;

// change to 8 if using full alarm
const int number_of_notes = 8;

int melody[] = {
  NOTE_C4, NOTE_G3
};

int melody2[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8
};

int noteDurations2[] = {
  4, 8, 4, 8, 4, 8, 4, 8
};

RF24 radio(7,8); //CE, CNS
// const byte addresses[][6] = {"00001","00002"};
const byte address[5] = {'R','x','A','A','A'};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // establish pin outmodes
  pinMode(LEDPIN,OUTPUT);
  pinMode(SPEAKERPIN,OUTPUT);
  // set up the radio
  radio.begin();
  // close reading pipes that may have been used previously
  radio.closeReadingPipe(0);
  radio.closeReadingPipe(1);
  // setup printf
  printf_begin();
  // check SPI bus connection
  if (radio.isChipConnected())
    Serial.println("Receiver NF24 connected to SPI");
  else {
    Serial.println("Receiver NF24 is NOT connected to SPI");
  }
  // set channel and open writing/reading pipes
  radio.setChannel(76);
  // write to N/A
  // radio.openWritingPipe(addresses[1]); // 00002
  // receive on RxAAA
  radio.openReadingPipe(1, address); // 00001 
  // set radio transmission power and data rate
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  // set radio to receive
  radio.startListening();
  // check radio is valid
  if (radio.isValid())
    Serial.println("Receiver NF24 is a valid radio");
  else {
    Serial.println("Receiver NF24 is NOT a valid radio");
  }
  // print debugging information
  radio.printPrettyDetails();
}

void loop() {
  // put your main code here, to run repeatedly:
  // debug:
  // PLAYALARM();
  // delay(5000);
  // RECEIVE_MESSAGE();
  
  // Receive_Protocol: with timeout of 10 seconds
  bool alarm = RECEIVE_PROTOCOL(5*1000,50);
  // Serial.println(alarm);
  // if alarm is true, then ring
  if(alarm)
  {
    PLAYALARM();
    alarm = false;
  }

}


void RECEIVE_MESSAGE()
{
  char text[32] = "";
  // start listening
  // check if radio avaliable
  if(radio.available())
  {
    Serial.println("Radio Avaliable");
    radio.read(&text, sizeof(text));
    Serial.print("Received: ");
    Serial.println(text);
    PLAYALARM();
  }
  else if(millis()%5000==0)
  {
    Serial.println("No Messages");
    delay(1);
  }
}

bool RECEIVE_PROTOCOL(int timeout,int LED_delay)
{
  // with a timeout failsafe
  // check for the first integer n message
  if(radio.available())
  {
    unsigned long start_time = millis();
    int n;
    // count the first one as 1 received
    int count = 1;
    radio.read(&n, sizeof(n));
    Serial.print("first message received: ");
    Serial.println(n);
    LED_FLASH(LED_delay);
    // wait for timeout or until messages have been received
    while(true)
    {
      int message;
      if (radio.available())
      {
        // here the message doesn't matter, just that we got something
        count++;
        radio.read(&message, sizeof(message));
        Serial.print("follow up message received: #");
        Serial.println(count);
        LED_FLASH(LED_delay); 
      }
      
      // if time out reached, breakout/return false
      if ((millis() - start_time) > timeout)
      {
        // check if count is a majority of n messages
        if (count > n/2)
        {
          Serial.print("Connection good! Received ");
          Serial.print(count);
          Serial.print(" number of messages out of ");
          Serial.print(n);
          Serial.println(" expected.");
          return false;
        }
        else
        {
          // 
          Serial.print("Poor Connection! Ringing Alarm! Only received ");
          Serial.print(count);
          Serial.print(" number of messages out of ");
          Serial.print(n);
          Serial.println(" expected by timeout.");
          return true;
        }
      }
    }
  }
  // else
  // {
  //   // no radio initiated return false
  //   if(millis()%5000==0)
  //   {
  //     Serial.println("No Messages.");
  //     delay(50);
  //   }
  //   return false;
  // }
}

// turns the LED on and waits for a set delay
void LED_ON(int delay_time)
{
  digitalWrite(LEDPIN,HIGH);
  delay(delay_time);
}

//turns the LED off and waits for set delay
void LED_OFF(int delay_time)
{
  digitalWrite(LEDPIN,LOW);
  delay(delay_time);
}

// flashes the LED on and off for delay_time
void LED_FLASH(int delay_time)
{
  LED_ON(delay_time);
  LED_OFF(delay_time);
}

// sounds the alarm, alarm is set in globals
void PLAYALARM()
{
  LED_ON(1);
  for (int thisNote = 0; thisNote < number_of_notes; thisNote++) {
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations2[thisNote];
    tone(SPEAKERPIN, melody2[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;

    delay(pauseBetweenNotes);

    // stop the tone playing:
    noTone(SPEAKERPIN);
  }
  LED_OFF(1);
}