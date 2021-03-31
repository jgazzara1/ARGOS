
//This code is the Heart of ARGOS Credit to Carmine Freda
//receives string from python and interprets as yaw and pitch values
//commands motors to go to these values

int potPin = 0; //we will read the potentiometer value on analog pin 5

//**********************************************
#include <Wire.h>

//*****ANGULAR CONSTANTS********
int yawVal;
int pitchVal;
int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs
float roll,pitch,rollF,pitchF=0;
String data, dataY, dataP;

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


//set the pins used on arduino to go to H-Bridge
int motor_p1 = 7;
int motor_p2 = 8;
int pwmPin = 9;

int motor2_p1 = 3;
int motor2_p2 = 4;
int pwm2Pin = 5;


DCMotor customServo(motor_p1, motor_p2, pwmPin); //create an instance of the DC motor Class
DCMotor customServo2(motor2_p1, motor2_p2, pwm2Pin);

//set the speed at which the motor will turn (Max speed = 255)
int turnSpeed = 190;
int val=0;
int i=0;
int val2=0;
int i2=0;

void setup()
{
 Serial.begin(115200); 

 Serial.setTimeout(100);
  delay(10);
}

void loop()
{
  
  // === Read acceleromter data === //
  data = Serial.readStringUntil('\n');
  //read serial data 
  if (data== 0){
    //prevents server from reading null data and setting value to 0
  }
  else{
  
  //chop string to remove excess characters in format b'123/456' and change to int
  dataP= data.substring(2,5);
  dataY= data.substring(6,9);
   pitchVal =(int) dataP.toInt();
  yawVal = (int) dataY.toInt();
  
  
  //map pack to 180 degrees 
  yawVal = map(yawVal, 999, 100, 0, 180);
  pitchVal = map(pitchVal, 100, 999, 0, 180); 

//move head up down
  potPin = 0;
  POT_VALUE_MIN = 160;
  POT_VALUE_MAX = 750;
  customServo.GoToAngle(pitchVal, turnSpeed);

//move head left right
   potPin = 3;
   POT_VALUE_MIN = 180;
   POT_VALUE_MAX = 700;
   customServo2.GoToAngle(yawVal, turnSpeed);
  }
  
}
