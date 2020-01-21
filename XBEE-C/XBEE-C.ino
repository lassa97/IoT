#include <MPXbee.h>

MPXbee cosa;
char c;
unsigned int sourceAddress;
unsigned int destinationAddress;
unsigned int moteID;
unsigned long timeReference;

union measurementUnion {
  float measurement;
  byte measurementBuf[4];
} medicion;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cosa.Init(MPXBEE_SERIALPORT_1);
  Serial.println("Red creada");
  getInfo();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  cosa.readData(&c);
  Serial.print("Protocol ID: ");
  Serial.print(byte(c), HEX);
  Serial.println();

  cosa.readData(&c);
  Serial.print("Payload: ");
  Serial.print(byte(c));
  Serial.println();

  cosa.readData(&c);
  Serial.print("Message ID: ");
  Serial.print(byte(c), HEX);
  Serial.println();
  
  getMedida();

}

void getInfo() {
  byte puerto, canal;
  int PAN, opPAN;
  unsigned int srcAddress, destAddressH, destAddressL;
  
  cosa.getSerialPort(&puerto);
  Serial.print("Puerto: ");
  Serial.println(puerto);
  
  cosa.getChannel(&canal);
  Serial.print("Canal: ");
  Serial.println(canal);
  
  cosa.getExtendedPAN(&PAN);
  Serial.print("PAN: ");
  Serial.println(PAN);
  
  cosa.getOperationPAN(&opPAN);
  Serial.print("Operational PAN: ");
  Serial.println(opPAN);
  
  cosa.getSrcAddress(&srcAddress);
  Serial.print("Source address: ");
  Serial.println(srcAddress);
  
  cosa.getDestAddress(&destAddressH, &destAddressL);
  Serial.print("Destination address: ");
  unsigned int destAddress = destAddress*256;
  destAddress += destAddressL;
  Serial.println(destAddress);
  
  Serial.println();
}

void getMedida() {
  cosa.readData(&c);
  sourceAddress = byte(c)*256;
  cosa.readData(&c);
  sourceAddress += byte(c);
  Serial.print("Source address: ");
  Serial.print(sourceAddress);
  Serial.println();
  
  cosa.readData(&c);
  destinationAddress = byte(c)*256;
  cosa.readData(&c);
  destinationAddress += byte(c);
  Serial.print("Destination address: ");
  Serial.print(destinationAddress);
  Serial.println();
  
  cosa.readData(&c);
  moteID = byte(c)*256;
  cosa.readData(&c);
  moteID += byte(c);
  Serial.print("Mote ID: ");
  Serial.print(moteID, HEX);
  Serial.println();
  
  cosa.readData(&c);
  Serial.print("Mote type: ");
  Serial.print(byte(c), HEX);
  Serial.println();
  
  cosa.readData(&c);
  Serial.print("Measurement type: ");
  Serial.print(byte(c), HEX);
  Serial.println();
  
  cosa.readData(&c);
  timeReference = (long) byte(c) << 24;
  cosa.readData(&c);
  timeReference += (long) byte(c) << 16;
  cosa.readData(&c);
  timeReference += (long) byte(c) << 8;
  cosa.readData(&c);
  timeReference += (long) byte(c);
  Serial.print("Time reference: ");
  Serial.print(timeReference);
  Serial.println();
  
  for (int i = 0; i < 4; i++) {
    cosa.readData(&c);
    medicion.measurementBuf[i] = c;
  }
  Serial.print("Measurement: ");
  Serial.println(medicion.measurement, 2);
  Serial.println();
}
