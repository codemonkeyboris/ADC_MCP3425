
#include <Arduino.h>
#include <Wire.h>

#define Addr 0x68 // 0b0110,1 A2A2A0(000) == 0nb0110,1000

void setup() 
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Start serial communication and set baud rate = 9600
  Serial.begin(9600);
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Send configuration command
  // Continuous conversion mode, 16-bit resolution
  Wire.write(0x18);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

#define buf_size  200
uint16_t i=0;
int16_t buf[buf_size]; 

void loop()
{
  uint8_t data[2];
/*  
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x00);
  Wire.endTransmission();
*/
  // Serial.println(i);
  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    // data[2] = Wire.read(); // configuration byte = 0x18 = 24
  }
   
  // Convert the data to 12-bits
  int16_t raw_adc = (data[0] << 8) + data[1];
  if(raw_adc > 32767)
  {
    raw_adc -= 65535;
  }

  if(i<buf_size)
  {
  	buf[i]=raw_adc;
  	//Serial.println(raw_adc);
  	i++;
  }
  else if(i==buf_size)
  {
  	int32_t tmp = 0;
  	for(int j=0;j<buf_size;j++)
  	{
  		tmp+= buf[j];
  	}
  	tmp = tmp/buf_size;
  	Serial.println(tmp);
  	i=0;
  }

}
