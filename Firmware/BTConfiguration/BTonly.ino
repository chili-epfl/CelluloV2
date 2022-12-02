#include "BluetoothSerial.h"

#include <Wire.h>

#include <FastLED.h> //Arduino library

#define LED_PIN     25
#define NUM_LEDS    30

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
#define RXD0 3
#define TXD0 1
CRGB leds[NUM_LEDS];
char a;

void bluetoothInit();
void bluetoothTask();
void bluetoothStop();

void setup() {
//  Serial.begin(115200,SERIAL_8N1,3,1);
//  Serial.println("Serial to computer is ready");//DEBUG MSG FOR COMPUTER
  pinMode(22, OUTPUT);
  digitalWrite(22, LOW);
  Serial.begin(115200,SERIAL_8N1,RXD0,TXD0);
  //Serial2.println("Enter BT mode");
  bluetoothInit();
//  while (!Serial.available()) {
//    Serial.begin(9600,SERIAL_8N1,RXD0,TXD0);
//      //Serial2.println("UART Fine");//DEBUG MSG FOR COMPUTER
//    }
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) 
    leds[i] = CRGB ( 20, 20, 20);
  FastLED.show();
}

void loop(){  
      bluetoothTask();
}

void bluetoothInit(){
  SerialBT.begin("cellulo-4126"); //Bluetooth device name
//  Serial.println("The device started, now you can pair it with bluetooth!");//DEBUG MSG FOR COMPUTER
}

void bluetoothTask(){
    if (Serial.available()) {
      SerialBT.write(a=Serial.read());
    }
    if (SerialBT.available()) {
      Serial.write(a=SerialBT.read());
    }
}

void bluetoothStop(){
  //Serial2.println("Bluetooth disconnecting...");//DEBUG MSG FOR COMPUTER
  SerialBT.println("Bluetooth disconnecting...");
  SerialBT.flush();
  SerialBT.disconnect();
  SerialBT.end();
  //Serial2.println("BT stopped");//DEBUG MSG FOR COMPUTER
}
