#include "Arduino.h"
#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>


// pin constants
int const LEDPIN = 4;

// radio variables
RF24 radio(7,8); //CE, CNS
// const byte addresses[][6] = {"00001","00002"};
const byte address[5] = {'R','x','A','A','A'};
// cooldown is multiplied by 1000 to get the real CD in seconds
int const cooldown = 30*1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // establish pin outmodes
  pinMode(LEDPIN,OUTPUT);
  // set up the radio
  radio.begin();
  // close reading pipes used previously
  radio.closeReadingPipe(0);
  radio.closeReadingPipe(1);
  // begin printf
  printf_begin();
  // check SPI bus connection
  if (radio.isChipConnected())
    Serial.println("Transmitter NF24 connected to SPI");
  else {
    Serial.println("Transmitter NF24 is NOT connected to SPI");
  }
  // Set channel and open writing/reading pipes
  radio.setChannel(76);
  // write to RxAAA
  radio.openWritingPipe(address); 
  // receive on 000002
  // radio.openReadingPipe(0,addresses[1]); // 00002
  // set radio transmission power and data rate
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  // disable retries
  radio.setRetries(0, 0);
  // set radio to transmit
  radio.stopListening();
  if (radio.isValid())
    Serial.println("Transmitter NF24 is a valid radio");
  else {
    Serial.println("Transmitter NF24 is NOT a valid radio");
  }
  radio.printPrettyDetails();
}


bool info;
const char text[] = "Hello World";

void loop() {
  // put your main code here, to run repeatedly:
  // transmitter sends message to address
  // LED_FLASH(500);
  START_PROTOCOL(2,50);
  delay(cooldown);

  // debug code:
  // info = radio.write(&text, sizeof(text));
  // if (info == 0)
  // {
  //   Serial.println("Failure");
  // }
  // else if (info == 1)
  // {
  //   Serial.println("Success");
  // }
  // LED_FLASH(100);
  // delay(3000);
}

void SEND_MESSAGE(char* message)
{
  // establish radio as sender
  radio.stopListening();
  // send the message
  radio.write(&message, sizeof(message));
  Serial.print("Sending message: ");
  Serial.println(message);
}

void SEND_MESSAGE(int message)
{
  // establish radio as sender
  radio.stopListening();
  // send the message
  radio.write(&message, sizeof(message));
  Serial.print("Sending message: ");
  Serial.println(message);
}

char* RECEIVE_MESSAGE()
{
  char text[32] = "";
  // start listening
  radio.startListening();
  // wait until radio avaliable
  while (!radio.available());
  // radio must have been avaliable to read
  radio.read(&text, sizeof(text));
  // Serial.println(text);
  // return the text message
  return text;
}

// Protocol sends n_checks number of messages
// if a majority are not received then the RF_Receiver sounds the alarm
void START_PROTOCOL(int n,int LED_delay)
{
  // no need to wait confirmation, send n messages
  for(int i = 0; i < n; i++)
  {
    // send the message n, n times
    SEND_MESSAGE(n);
    // delay
    LED_FLASH(LED_delay);
  }
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
