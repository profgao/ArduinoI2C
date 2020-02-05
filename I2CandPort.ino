#include <Wire.h>

byte val = 0;
int led = 0;
int ledPin = 13;  // LED connected to digital pin 13
bool ledOn = false;

int readMasterTransceiverReg(int regAddr) {

  int receiveDate = 0;
  int sendNumber = 0;

  Wire.beginTransmission(0x68); // transmit to master transciver device, BASE_Addr is 0x68
                              // device address is specified in datasheet
  sendNumber=Wire.write(regAddr);             // sends value byte 
  //Serial.print(val);         // print the character
  //Serial.print(sendNumber);         // print the character
  
  Wire.endTransmission();     // stop transmitting


  Wire.requestFrom(0x68, 1);    // request 1 bytes from register,  A2B master I2C BASE_Addr is 0x68, R/-W=1

  while(1 <= Wire.available())    // slave may send less than requested
  { 
    receiveDate = Wire.read(); // receive a byte as character
    //receiveDate = receiveDate << 8; //shift high byte to high 8 bits
    //receiveDate |= Wire.read();
    Serial.print(" receiveDate=ox"); 
    Serial.print(receiveDate, HEX);         // print, the data should be 0xad
    Serial.println();
    return receiveDate;
  }
  return -1;
}

void writeMasterTransceiverReg(int regAddr, int data) {

  Wire.beginTransmission(0x68); // transmit to master transciver device, BASE_Addr is 0x68
                              // device address is specified in datasheet
  Wire.write(regAddr);             // sends value byte 
  //Serial.print(val);         // print the character
  //Serial.print(sendNumber);         // print the character
  Wire.write(data);             // sends value byte 
  
  Wire.endTransmission();     // stop transmitting
}

int writeSlaveTransceiverReg(int regAddr, int data, int nodeNumber) {
  int regData = 0;
  if (nodeNumber>10 || nodeNumber<0) {
    return -1;
  }
  else{
    regData = nodeNumber & 0xFF;
  }

  Wire.beginTransmission(0x68); // transmit to master transciver device, BASE_Addr is 0x68
                              // device address is specified in datasheet
  Wire.write(0x01);             // set A2B_NODEADR register addr

  //Wire.write(regData);             // set bit7:BRCST=0, bit5:PERI=0, bit3-0:slave Node number
  Wire.write(0x00);             // set BRCST=0, PERI=0, slave Node number=0
  Wire.write(0x69);             // BUS_Addr is 0x68

  Wire.write(regAddr);             // sends reg addr 
  Wire.write(data);             // sends value byte 
  
  Wire.endTransmission();     // stop transmitting
  return 0;
}

void setup()
{
  Serial.begin(9600);  // start Serial for output
  Wire.setClock(400000L);
  Wire.begin();                // join i2c bus with master
  pinMode(ledPin, OUTPUT);  // sets the digital pin 13 as output
}

// every loop() will spend ~14us
void loop(){

  int receiveDate = 0;
  int errorWriteCount = 0;
  int regAddr = 0;
  int errorCount = 0;
  /*
  Wire.requestFrom(2, 6);    // request 6 bytes from slave device #2

  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
*/
  regAddr = 0x02;
  for (int i=0; i< 10; i++){
 
    receiveDate = readMasterTransceiverReg(regAddr);
    //delay(500);
    
    if (receiveDate != 0xAD)
    {
      errorCount++;
      Serial.print("errorCount=");
      Serial.print(errorCount); 
      Serial.println();
    }
    // write Master enable into reg A2B_CONTROL 0x12
    writeMasterTransceiverReg(0x12, 0x80);
    receiveDate = readMasterTransceiverReg(0x12);
    if (receiveDate != 0x80)
    {
      errorWriteCount++;
      Serial.print("receiveDat=");
      Serial.print(receiveDate); 
      Serial.print(" errorWriteCount=");
      Serial.print(errorWriteCount); 
      Serial.println();
    }
  }

  /*send pulse to A2
  for (int i=0; i< 100; i++){
    digitalWrite(A2, 1);
    delay(1);
    digitalWrite(A2, 0);
    delay(1);
    //Serial.print(i);         // print the character

  }
  */
  //delay(500);
}
