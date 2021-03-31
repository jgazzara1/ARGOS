int pitch, pitch2,yaw=0;
void setup(){
   
  // Set the baud rate  
  Serial.begin(15200);
   
}
 
void loop(){
 
  if(Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    //Serial.print("Hi Raspberry Pi! You sent me: ");
   // Serial.println(data);
   pitch= data.substring(0,2);
   pitch2= pitch.toInt();
  }
}
