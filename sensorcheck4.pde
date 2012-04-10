  int analogValue = 0;   
  int analogValue1 = 1; 
  int analogValue2 = 2; 
  int analogValue3 = 3; 
  int analogValue4 = 4; 
void setup() {

  Serial.begin(9600);
}

void loop() {

  analogValue = analogRead(0);
  analogValue1 = analogRead(1);
  analogValue2 = analogRead(2);
  analogValue3 = analogRead(3);
  analogValue4 = analogRead(4);
  


  Serial.print(analogValue, DEC);         
  Serial.print("\t A:");              
  Serial.print(analogValue1, DEC);    
  Serial.print("\t B:");                
  Serial.print(analogValue2, DEC);    
  Serial.print("\t C:");               
  Serial.print(analogValue3, DEC);    
  Serial.print("\t D:"); 
  Serial.print(analogValue4, DEC);    
  Serial.print("\t E:");   
  Serial.println();                 

  // delay 10 milliseconds before the next reading:
  delay(10);
}
