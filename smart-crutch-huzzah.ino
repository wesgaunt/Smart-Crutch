#include <ESP8266WiFi.h>
#include <Wire.h>
 
#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C
 
#define GYRO_FULL_SCALE_250_DPS 0x00 
#define GYRO_FULL_SCALE_500_DPS 0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18
 
#define ACC_FULL_SCALE_2_G 0x00 
#define ACC_FULL_SCALE_4_G 0x08
#define ACC_FULL_SCALE_8_G 0x10
#define ACC_FULL_SCALE_16_G 0x18

const String SSIDS[2] = {"Fellas WiFi", "Closed Network"};
const String PASSWORDS[2] = {"Silverton4ever", "portugal1"};
const int NUM_NETWORKS = 2;
 
const char* ssid     = "Fellas WiFi";
const char* password = "Silverton4ever";
 
const char* host = "wifitest.adafruit.com";

void connectToWifi() {
  WiFi.scanNetworks(false, false);
  Serial.println("Scanning for networks");
  while (WiFi.scanComplete() < 0) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("Found:");
  int numberOfAccessPoints = WiFi.scanComplete();
  for(int n = 0; n < numberOfAccessPoints; n++) {
    Serial.println(WiFi.SSID(n));
  }

  bool connected = false;

  for(int n = 0; n < numberOfAccessPoints; n++) {
    for(int m = 0; m < NUM_NETWORKS; m++) {
      if (SSIDS[m] == WiFi.SSID(n)) {
        connected = true;
        Serial.print("Connecting to ");
        Serial.print(SSIDS[m]);
        WiFi.begin(SSIDS[m], PASSWORDS[m]);
        while (WiFi.status() != WL_CONNECTED) {
          Serial.print(".");
          delay(500);
        }
        Serial.println(".");
        Serial.println("WiFi connected");
      }
    }
  }
  if(!connected) {
    Serial.println("WiFi not connected");
  }
}

// This function read Nbytes bytes from I2C device at address Address. 
// Put read bytes starting at register Register in the Data array. 
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data) {
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  
  // Read Nbytes
  Wire.requestFrom(Address, Nbytes);
  uint8_t index=0;
  while (Wire.available()) {
    Data[index++]=Wire.read();
  }
}

// Write a byte (Data) in device (Address) at register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data) {
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

uint8_t* readAccel() {
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);
  
  // Create 16 bits values from 8 bits data
  
  // Accelerometer
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t az=Buf[4]<<8 | Buf[5];
  
  // Gyroscope
  int16_t gx=-(Buf[8]<<8 | Buf[9]);
  int16_t gy=-(Buf[10]<<8 | Buf[11]);
  int16_t gz=Buf[12]<<8 | Buf[13];
  return(Buf);
}
 
void setup() {
  Serial.begin(115200);
  delay(5000);

  connectToWifi();
//  testIMU();

}

void loop() {
  delay(500);
  uint8_t Buf[14] = {*readAccel()};

  // Accelerometer
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t az=Buf[4]<<8 | Buf[5];
  
  // Gyroscope
  int16_t gx=-(Buf[8]<<8 | Buf[9]);
  int16_t gy=-(Buf[10]<<8 | Buf[11]);
  int16_t gz=Buf[12]<<8 | Buf[13];
  
  Serial.print("X: ");
  Serial.println(ax,DEC);

  Serial.print("Y: ");
  Serial.println(ay,DEC);

  Serial.print("Z: ");
  Serial.println(az,DEC);
}
