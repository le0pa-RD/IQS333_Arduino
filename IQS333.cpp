#include "IQS333.h"

uint8_t w_buffer[8];            //Write buffer
uint8_t r_buffer[16];           //Read buffer

uint8_t number_Product = 0;     //Product Number
uint8_t number_Version = 0;     //Version Number

uint8_t CH0,CH1,CH2,CH3,CH4,CH5,CH6,CH7;

int Counts_CH0,Counts_CH1,Counts_CH2,Counts_CH3,Counts_CH4,Counts_CH5,Counts_CH6,Counts_CH7;
int LTA_CH0,LTA_CH1,LTA_CH2,LTA_CH3,LTA_CH4,LTA_CH5,LTA_CH6,LTA_CH7;

//Read Random Bytes function
//Requires Device's I2C Address as given in IQS333.h (0x64)
void readRandomBytes(uint8_t I2C_Address, uint8_t memoryAddress, int numBytes, uint8_t bytesArray[], bool stopOrRestart){
  //Counter variable for loading array
  uint8_t i = 0; 
    //Select Device and start comms
  Wire.beginTransmission(I2C_Address);
  //Specify memory address to start reading data
  Wire.write(memoryAddress);
  //Complete selection
  Wire.endTransmission(RESTART); //Restart comms for reading next
  //Request bytes
  do{
    Wire.requestFrom(I2C_Address, numBytes, stopOrRestart);
  }while(Wire.available() == 0);
  //Load bytes into array
  while(Wire.available()){
    bytesArray[i] = Wire.read();
    i++;
  }
}

//Write Random Bytes function
//Requires Device's I2C Address as given in IQS333.h (0x64)
void writeRandomBytes(uint8_t I2C_Address, uint8_t memoryAddress, int numBytes, uint8_t bytesArray[], bool stopOrRestart){
  //Select Device and start comms
  Wire.beginTransmission(I2C_Address);
  //Specify memory address to start writing data
  Wire.write(memoryAddress);
  //Write data
  for(int i = 0; i < numBytes; i++){
    Wire.write(bytesArray[i]);
  }
  //End trsnsmission, STOP or RESTART
  Wire.endTransmission(stopOrRestart);
}

void IQS333_setup(){
  while(!digitalRead(RDY));                                                 //Wait for RDY line to go High to catch next com window
  while(digitalRead(RDY));                                                  //Wait for RDY line to go Low
  readRandomBytes(IQS333_I2C_Address, VERSION_INFO, 2, r_buffer, STOP);     //Read Device Information of device  
  delay(1); 
  number_Product = r_buffer[0];
  number_Version = r_buffer[1]; 

  //Print Product and Version Numbers out on Serial Monitor
  Serial.print("Product Number : ");Serial.println(number_Product);
  Serial.print("Version Number : ");Serial.println(number_Version);

  //Product Number is 54 from datasheet
  if((number_Product == 54) && (number_Version == 2)){                   
    Serial.println("IQS333 Chip present");    
    IQS333_settings();
  }
  else{
    Serial.println("Unknown Chip ... ");   
    delay(2000); 
    setup();
  }
}

int *IQS333_read(){

  static int IQS_sensor_values[8];
  
  //Read Touch Bytes Status  
  while(digitalRead(RDY)); 
  readRandomBytes(IQS333_I2C_Address, TOUCH_BYTES, 1, r_buffer, RESTART);
  CH0 = bitRead(r_buffer[0],0);
  CH1 = bitRead(r_buffer[0],1);
  CH2 = bitRead(r_buffer[0],2);
  CH3 = bitRead(r_buffer[0],3);
  CH4 = bitRead(r_buffer[0],4);
  CH5 = bitRead(r_buffer[0],5);
  CH6 = bitRead(r_buffer[0],6);
  CH7 = bitRead(r_buffer[0],7);  

  //Read Counts
  readRandomBytes(IQS333_I2C_Address, COUNTS, 16, r_buffer, RESTART);
  Counts_CH0 = (r_buffer[1] << 8) + r_buffer[0];
  Counts_CH1 = (r_buffer[3] << 8) + r_buffer[2];
  Counts_CH2 = (r_buffer[5] << 8) + r_buffer[4];
  Counts_CH3 = (r_buffer[7] << 8) + r_buffer[6];
  Counts_CH4 = (r_buffer[9] << 8) + r_buffer[8];
  Counts_CH5 = (r_buffer[11] << 8) + r_buffer[10];
  Counts_CH6 = (r_buffer[13] << 8) + r_buffer[12];
  Counts_CH7 = (r_buffer[15] << 8) + r_buffer[14];  

  //Read LTAs  
  readRandomBytes(IQS333_I2C_Address, LTA, 16, r_buffer, STOP);
  delay(1);
  LTA_CH0 = (r_buffer[1] << 8) + r_buffer[0];
  LTA_CH1 = (r_buffer[3] << 8) + r_buffer[2];
  LTA_CH2 = (r_buffer[5] << 8) + r_buffer[4];
  LTA_CH3 = (r_buffer[7] << 8) + r_buffer[6];
  LTA_CH4 = (r_buffer[9] << 8) + r_buffer[8];
  LTA_CH5 = (r_buffer[11] << 8) + r_buffer[10];
  LTA_CH6 = (r_buffer[13] << 8) + r_buffer[12];
  LTA_CH7 = (r_buffer[15] << 8) + r_buffer[14];  

  //Calculate Deltas
  IQS_sensor_values[0] = LTA_CH0 - Counts_CH0;
  IQS_sensor_values[1] = LTA_CH1 - Counts_CH1;
  IQS_sensor_values[2] = LTA_CH2 - Counts_CH2;
  IQS_sensor_values[3] = LTA_CH3 - Counts_CH3;
  IQS_sensor_values[4] = LTA_CH4 - Counts_CH4;
  IQS_sensor_values[5] = LTA_CH5 - Counts_CH5;
  IQS_sensor_values[6] = LTA_CH6 - Counts_CH6;
  IQS_sensor_values[7] = LTA_CH7 - Counts_CH7;  

  return IQS_sensor_values;
}

void IQS333_settings(){
  
  //Read Prox Settings  
  while(digitalRead(RDY));                                //Wait for RDY line to go Low
  readRandomBytes(IQS333_I2C_Address, PROXSETTINGS, 6, r_buffer, STOP);  
  delay(1);   
  Serial.print("ProxSettings0 : ");Serial.println(r_buffer[0]);
  Serial.print("ProxSettings1 : ");Serial.println(r_buffer[1]);
  Serial.print("ProxSettings2 : ");Serial.println(r_buffer[2]);
  Serial.print("ProxSettings3 : ");Serial.println(r_buffer[3]);
  Serial.print("ProxSettings4 : ");Serial.println(r_buffer[4]);
  Serial.print("ProxSettings5 : ");Serial.println(r_buffer[5]);

  //Enable Channels 0 to 7
  w_buffer[0] = 255;
  while(digitalRead(RDY));                                //Wait for RDY line to go Low
  writeRandomBytes(IQS333_I2C_Address, ACTIVE_CHANNELS, 1, w_buffer, STOP);
  delay(1);  
}
