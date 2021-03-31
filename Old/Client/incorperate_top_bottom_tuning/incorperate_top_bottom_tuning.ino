//Arduino code to control angle of a motor shaft using a potentiometer for feedback
//Please use a low rpm motor. Not more than about 500 rpm.
//******POTENTIOMETER SETUP *************

//1. Fix the shaft of the potentiometer to the motor shaft.You might like to use a slightly flexible coupling
//to do this, otherwise even a slight misalignment may cause trouble.

//2. Fix the body of the potentiometer to a rigid surface such as the body of the motor,
//so that when the motor shaft turns, only the potentiometer shaft turns with it.

//3. Now we can read the potentiometer value to get the angle of the motor shaft

//Look at my youtube video to see how I did this. In my video, I fixed the BODY of the potentiometer
//to the motor shaft. It will be better to fix the SHAFT of the potentiomter to the motor
//shaft if you can do it properly

//Fix santa's hand to the motor shaft so that it does not interfere with the potentiometer movement

int potPin = 0; //we will read the potentiometer value on analog pin 5
//**********************************************
#include <Wire.h>
//*****ANGULAR CONSTANTS********
int rollVal;
int pitchVal;
int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs
float roll,pitch,rollF,pitchF=0;

int POT_VALUE_MAX; // potentiometer reading when motor shaft is at 180 degree position. 
//You will need to fill this value according to your setup.See below....

int POT_VALUE_MIN; //potentiometer reading when when motor shaft is at 0 degree position. 
//You will need to fill this value according to your setup.See below....

//To fill up the correct values, first turn the motor shaft manually to 0 degree position. 
//Now read the potentiometer value and edit the #define POT_VALUE_MIN line with your pot reading. 
//Next manually move the motor shaft to 180 degree position, 
//read the pot value and edit #define POT_VALUE_MAX line with your pot reading.

#define PERM_ERROR 5 //the max permissible error in degrees. In my potentiometer, a turn only about 3 degrees
//on the potentiometer shaft causes any real change in the ohmic reading. You can adjust this error
//value as required. If PERM_ERROR is very small, the shaft will keep hunting left and right as the
//analogRead() of the potentiometer pin keeps fluctuating slightly

#define MAX_ANGLE 180 //we will allow our motor to turn by a maximum angle of 180 degrees
//**********************************

//Now we define a class that will control our DC motor....
//This motor must be running from a H bridge like L298 IC

//********************************************************************
class DCMotor
{
private:

    int M_pin1 , M_pin2, M_PWMPin;

    int M_Speed;

    int turnDirection;
    
    enum turnDirection {right, left};

public:

    DCMotor(int p1, int p2, int p3) //Constructor
    {
            M_pin1 = p1; //direction pin on L298

            M_pin2 = p2;  //direction pin on L298

            M_PWMPin = p3;  //PWM pin on L298
            
            pinMode(M_pin1, OUTPUT);
            
            pinMode(M_pin2, OUTPUT);
    }

    void SetTurnDirection(int dir)//Setting turn directions on L298 IC
    {
        turnDirection = dir;

        switch(turnDirection)
        {
            case right: //turning Right

                    //motor moves CW

                    digitalWrite(M_pin1, HIGH);
                    digitalWrite(M_pin2, LOW);

                    break;

            case left: //turning Left

                    //motor moves CCW

                    digitalWrite(M_pin1, LOW);
                    digitalWrite(M_pin2, HIGH);

                	break;
        }
    }

    void SetTurnSpeed(int s)
    {
         M_Speed = s;
    }
    
    void Turn()
    {
        analogWrite(M_PWMPin, M_Speed);
    }
    
    void Stop()
    {
        analogWrite(M_PWMPin, 0);
    }
    
    void GoToAngle(int target, int howFast)
    {
      //find out the current angle of the motor shaft
  
      int currentAngle = ((float)analogRead(potPin) - POT_VALUE_MIN)/(POT_VALUE_MAX - POT_VALUE_MIN) * MAX_ANGLE;
      Serial.println(currentAngle); 
      //First Check if we need to turn left or right.....
      if (currentAngle < target)
      {
        SetTurnDirection(right);
      } 
      else if (currentAngle > target)
      {
        SetTurnDirection(left);
      }
      
      SetTurnSpeed(howFast);
      
      while(abs(currentAngle - target) > PERM_ERROR)//if the shaft is not at the required value,
      {
        Turn();//Keep on turning the shaft
        
        //Allow the motor to turn a little and wait here for a moment...
        
        delay(1); //adjust the delay as required depending on your motor speed
        
        //update the current angle of the shaft 
        
        currentAngle = ((float)analogRead(potPin) - POT_VALUE_MIN)/(POT_VALUE_MAX - POT_VALUE_MIN) * MAX_ANGLE;
      } 
      
      Stop(); //Stop the shaft after the error is acceptable
    }

};



//DC Motor Class definition completed................
//************************************************************

//L298 pin defintions.....
int motor_p1 = 7;
int motor_p2 = 8;
int pwmPin = 9;

int motor2_p1 = 3;
int motor2_p2 = 4;
int pwm2Pin = 5;


DCMotor customServo(motor_p1, motor_p2, pwmPin); //create an instance of the DC motor Class
DCMotor customServo2(motor2_p1, motor2_p2, pwm2Pin);

//set the speed at which the motor will turn (Max speed = 255)
int turnSpeed = 210;
int val=0;
int i=0;
int val2=0;
int i2=0;

void setup()
{
 Serial.begin(9600); //no code is needed here for a simple up and down swing of santa's arm

 Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345); // Start communicating with the device
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8); // Bit D3 High for measuring enable (8dec -> 0000 1000 binary)
  Wire.endTransmission();
  delay(10);
  //Off-set Calibration
  //X-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1E);
  Wire.write(1);
  Wire.endTransmission();
  delay(10);
  //Y-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1F);
  Wire.write(-2);
  Wire.endTransmission();
  delay(10);
  //Z-axis
  Wire.beginTransmission(ADXL345);
  Wire.write(0x20);
  Wire.write(-9);
  Wire.endTransmission();
  delay(10);
}

void loop()
{
  
  // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read() | Wire.read() << 8); // X-axis value
  X_out = X_out / 256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read() | Wire.read() << 8); // Y-axis value
  Y_out = Y_out / 256;
  Z_out = ( Wire.read() | Wire.read() << 8); // Z-axis value
  Z_out = Z_out / 256;
  // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
  roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
  //yaw= atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  // Low-pass filter
  rollF = 0.94 * rollF + 0.06 * roll;
  pitchF = 0.94 * pitchF + 0.06 * pitch;
  rollVal= rollF;          
  pitchVal= pitchF; // reads the value of the potentiometer (value between 0 and 1023)
  rollVal = map(rollVal, -90, 90, 0, 180);
  pitchVal = map(pitchVal, 0, 90, 0, 180); // scale it to use it with the servo (value between 0 and 180)
  Serial.print("pitch");
  Serial.println( pitchVal);
  Serial.print("/");
  Serial.print("roll ");
  Serial.println( rollVal);
  potPin = 0;
  val=pitchVal;
  i= val;
  POT_VALUE_MIN = 570;
  POT_VALUE_MAX = 1000;
   customServo.GoToAngle(i, turnSpeed);
   potPin = 3;
  val2=rollVal;
  i2= val2;
  POT_VALUE_MIN = 0;
  POT_VALUE_MAX = 900;
    customServo2.GoToAngle(i2, turnSpeed);
     
  
}
