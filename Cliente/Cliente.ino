#include <MPXbee.h>

MPXbee cliente;
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
  Serial.begin(9600);
  cliente.Init(MPXBEE_SERIALPORT_1);
  Serial.println("Red creada");
  getInfo();
  
}

void loop() {
  cliente.readData(&c);
  Serial.print("Protocol ID: ");
  Serial.print(byte(c), HEX);
  Serial.println();

  cliente.readData(&c);
  Serial.print("Payload: ");
  Serial.print(byte(c));
  Serial.println();

  cliente.readData(&c);
  Serial.print("Message ID: ");
  Serial.print(byte(c), HEX);
  Serial.println();
  
  getMedida();

}

void getInfo() {
  byte puerto, canal;
  int PAN, opPAN;
  unsigned int srcAddress, destAddressH, destAddressL;
  
  cliente.getSerialPort(&puerto);
  Serial.print("Puerto: ");
  Serial.println(puerto);
  
  cliente.getChannel(&canal);
  Serial.print("Canal: ");
  Serial.println(canal);
  
  cliente.getExtendedPAN(&PAN);
  Serial.print("PAN: ");
  Serial.println(PAN);
  
  cliente.getOperationPAN(&opPAN);
  Serial.print("Operational PAN: ");
  Serial.println(opPAN);
  
  cliente.getSrcAddress(&srcAddress);
  Serial.print("Source address: ");
  Serial.println(srcAddress);
  
  cliente.getDestAddress(&destAddressH, &destAddressL);
  Serial.print("Destination address: ");
  unsigned int destAddress = destAddress*256;
  destAddress += destAddressL;
  Serial.println(destAddress);
  
  Serial.println();
}

void getMedida() {
  cliente.readData(&c);
  sourceAddress = byte(c)*256;
  cliente.readData(&c);
  sourceAddress += byte(c);
  Serial.print("Source address: ");
  Serial.print(sourceAddress);
  Serial.println();
  
  cliente.readData(&c);
  destinationAddress = byte(c)*256;
  cliente.readData(&c);
  destinationAddress += byte(c);
  Serial.print("Destination address: ");
  Serial.print(destinationAddress);
  Serial.println();
  
  cliente.readData(&c);
  moteID = byte(c)*256;
  cliente.readData(&c);
  moteID += byte(c);
  Serial.print("Mote ID: ");
  Serial.print(moteID, HEX);
  Serial.println();
  
  cliente.readData(&c);
  Serial.print("Mote type: ");
  Serial.print(byte(c), HEX);
  Serial.println();
  
  cliente.readData(&c);
  Serial.print("Measurement type: ");
  Serial.print(byte(c), HEX);
  Serial.println();
  
  cliente.readData(&c);
  timeReference = (long) byte(c) << 24;
  cliente.readData(&c);
  timeReference += (long) byte(c) << 16;
  cliente.readData(&c);
  timeReference += (long) byte(c) << 8;
  cliente.readData(&c);
  timeReference += (long) byte(c);
  Serial.print("Time reference: ");
  Serial.print(timeReference);
  Serial.println();
  
  for (int i = 0; i < 4; i++) {
    cliente.readData(&c);
    medicion.measurementBuf[i] = c;
  }
  Serial.print("Measurement: ");
  Serial.println(medicion.measurement, 2);
  Serial.println();
}
