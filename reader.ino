/* ROM reader for AM27128A
* <adam.w.meek@gmail.com>
* This program reads the contents of an AM27128A
* OTPROM and relays the data over serial
* 
* PINS:
* Arduino Mega 2560 | AM27128A
*
* even 48 downto 22 | adress 0 upto 13
* odd 43 downto 31  | data 0 upto 7
* 52                | OE 
*
* Note: Output Enable (OE) is active LOW
*
*/
byte addressPins[] = {48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22};
byte dataPins[] = {45, 43, 41, 39, 37, 35, 33, 31};
byte outputEnable = 52;

void setup() {
  
  //setup data pins
  int i;
  for(i = 7; i >= 0; i = i - 1) {
    
    pinMode(dataPins[i], INPUT);
  }
  
  //setup address pins
  for(i = 13; i >= 0; i = i - 1) {
    
    pinMode(addressPins[i], OUTPUT);
  }
  
  //setup OE
  pinMode(outputEnable, OUTPUT);
  
  //setup serial 
  Serial.begin(9600);
  
}


void loop() {
 
 
 Serial.println("START");
 
 int i;
 for(i = 0; i < 16384; i = i + 1) {
   
   byte data = readAddress(i);
 
   Serial.write(data); 
 }
 
 Serial.println("ENDTX");
 
 while(true) {
   //loop forver when done
 }
}
byte readAddress(int address) {
 
 byte retVal = 0;
 
 //First, pull OE high
 digitalWrite(outputEnable, HIGH);
 
 //Format address pins
 int i;
 for(i = 13; i >= 0; i = i - 1) {
   
   //read each place column for address
   if(((address & (1 << i)) >> i) == 1) {
     
     digitalWrite(addressPins[i], HIGH);
   }
   else {
     
     digitalWrite(addressPins[i], LOW);
   }
 }
 
 //Now, address is valid
 //Pull OE low and read data
 
 digitalWrite(outputEnable, LOW);
 
 //Delay to allow data to stabilize
 //AM27128 needs 250 ns to stabiliza
 delayMicroseconds(1);
 
 for(i = 7; i >= 0; i = i - 1) {
   
   retVal = retVal | (digitalRead(dataPins[i]) << i );
 }
 
 return retVal;
}
