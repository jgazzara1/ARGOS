#include <Wire.h>
#include <MPU6050.h>
int count = 0;
MPU6050 mpu; //This library must be downloaded from here: https://github.com/jarzebski/Arduino-MPU6050/archive/master.zip

// Timers
unsigned long timer = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values
float pitch, pitch2 = 0;
float roll = 0;
float yaw, yaw2= 0;
int yaw3=0;
int pitch3=0;

void setup() 
{
  Serial.begin(115200);

  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

}

void loop()
{
  timer = millis();

  // Read normalized values
  Vector norm = mpu.readNormalizeGyro();

  // Calculate Pitch, Roll and Yaw
  pitch = pitch + norm.YAxis * timeStep;
  roll = roll + norm.XAxis * timeStep;
  yaw = yaw + norm.ZAxis * timeStep;

  //make sure the values do not exceed -90 or 90 degrees
  yaw2=yaw;
  pitch2=pitch;
  if(yaw2>90){
    yaw2 = 90;
  }
  if(yaw2<-90){
    yaw2 = -90;
  }
  if(pitch2>90){
    pitch2 = 90;
  }
  if(pitch2<-90){
    pitch2 = -90;
  }

//map -90 to 90 -> 100 to 999 so that the same number of bytes is sent every transmission
yaw2=map(yaw2,-90,90,100,999);
  pitch2=map(pitch2,-90,90,100,999);
  pitch3 = (int) pitch2;
  yaw3 = (int) yaw2;

 count=count+1;
  // Output raw
  if(count == 30){
    //was 30
  Serial.print(pitch3);
  Serial.print("/");
  Serial.println(yaw3);
  count = 0;
  }
  // Wait to full timeStep period
  delay((timeStep*1000) - (millis() - timer));
  
}
