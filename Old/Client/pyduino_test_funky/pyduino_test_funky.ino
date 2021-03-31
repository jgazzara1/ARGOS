int pitch= 0;
int pitch2, yaw=100;
String data ;
void setup(){
   
  // Set the baud rate  
  Serial.begin(115200);
   
}
 
void loop(){
 
  if (Serial.available() > 0) {
    digitalWrite(13, HIGH);
  }
  else{
    digitalWrite(13, LOW);
  }
    data = Serial.readStringUntil('\n');
    
    //Serial.println(data);
   
   
  //}
 // Serial.print("Hi Raspberry Pi! You sent me: ");
  data= data.substring(2,5);
   pitch =(int) data.toInt();
   //Serial.println(pitch);
  if(pitch >300){
     digitalWrite(12, HIGH);
     
   }
   if(pitch <=300){
     digitalWrite(12,LOW);
   }
  
  
  
  
}
