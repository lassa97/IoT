#include <SPXbee.h>
#include <Wire.h>
#include <Adafruit_HTU21DF.h>

SPXbee cosa;

Adafruit_HTU21DF sensor = Adafruit_HTU21DF();

byte protocolID = 0xAA;

byte payload;

byte messageID;

byte sourceAddressHigh = 0x20;
byte sourceAddressLow = 0x03;

byte destinationAddressHigh = 0xFF;
byte destinationAddressLow = 0xFF;

byte moteIDHigh = 0x00;
byte moteIDLow = 0x05;

//0x01 (Humedad/temperatura) 0x02 (Intensidad luminosa) 0x03 (Interruptor magnetico) 0x04 (Humedad/temperatura + interruptor magnetico) 0x05 (Intensidad luminosa + interruptor magnetico)
byte moteType = 0x01;

//0x01 (Humedad) 0x02 (Temperatura) 0x03 (Intensidad luminosa) 0x04 (Estado interruptor magnetico)
byte measurementType = 0x02;  //Temperatura

unsigned long timeReference;
byte timeBuf[4];
char c;

union measurementUnion {
  float measurement;
  byte measurementBuf[4];
} medicion;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cosa.Init();
  sensor.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  payload = 0x11;
  messageID = 0x05;

  cosa.writeData(protocolID);
  cosa.writeData(payload);
  cosa.writeData(messageID);
  cosa.writeData(sourceAddressHigh);
  cosa.writeData(sourceAddressLow);
  cosa.writeData(destinationAddressHigh);
  cosa.writeData(destinationAddressLow);
  cosa.writeData(moteIDHigh);
  cosa.writeData(moteIDLow);
  cosa.writeData(moteType);
  cosa.writeData(measurementType);

  timeReference = millis();
  timeBuf[0] = timeReference >> 24;
  timeBuf[1] = timeReference >> 16;
  timeBuf[2] = timeReference >> 8;
  timeBuf[3] = timeReference;
  
  for (int i = 0; i < 4; i++) {
    cosa.writeData(timeBuf[i]); 
  }

  medicion.measurement = sensor.readTemperature();
  
  for (int i = 0; i < 4; i++) {
    cosa.writeData(medicion.measurementBuf[i]);
  }

  delay(1000);

}
