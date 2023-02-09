/* 1byte LORa SX1278 TRANSMITTER example.
/* Tutorial link: http://electronoobs.com/eng_arduino_tut97.php
 * Code: http://electronoobs.com/eng_arduino_tut97_code1.php
 * Scheamtic: http://electronoobs.com/eng_arduino_tut97_sch1.php
 * Youtube Channel: http://www.youtube/c/electronoobs   
 * 
  Module SX1278 // Arduino UNO/NANO    
    GND         ->   GND
    Vcc         ->   3.3V
    MISO        ->   D12
    MOSI        ->   D11     
    SLCK        ->   D13
    Nss         ->   D10
    Di00        ->   D2
    RST         ->   D9      
 */

#include <SPI.h>
#include <LoRa.h>   //Download here: http://electronoobs.com/eng_arduino_LoRa_SX1278.php.php
String inString = "";    // string to hold input
String val = "";
int rel1 = 8;
int rel2 = 6;
int rel3 = 7;
int button =  3;

int isstart = 0;
int int_dist = 0;
int isready = 1;
int empty = 0;
long holdingTime;
long previousHoldingTime;
unsigned long firstButtonPressTime;
byte buttonState;
byte previousButtonState = HIGH;


void setup() {
  Serial.begin(9600);
  pinMode(rel1,OUTPUT);
  pinMode(rel2,OUTPUT);
  pinMode(rel3,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(868E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  digitalWrite(rel1,1);
  digitalWrite(rel2,1);
  digitalWrite(rel3,1);
  
}

int LongPress(){
  
  buttonState = digitalRead(button);
  Serial.println(buttonState);
  delay(200);
  //int distance  = readDistance();
  if (buttonState == LOW && previousButtonState == HIGH && (millis() - firstButtonPressTime) > 200) 
  {
    firstButtonPressTime = millis();
  }
  holdingTime = (millis() - firstButtonPressTime);

  if (holdingTime > 50) 
  {
    if (buttonState == LOW && holdingTime > previousHoldingTime) 
    {
//      Serial.println("button is pressed");
    }
    if (buttonState == HIGH && previousButtonState == LOW) 
    {
      if (holdingTime > 3300 ) 
      {
          // if (!int_dist){
          //   int_dist = distance;
          //   printdata();
            
          // }
          return 1;
      }
      
    }
  }
   previousButtonState = buttonState;
   previousHoldingTime = holdingTime;
   return 0;
}

void sendStartcmd(){

  //Serial.print("Sending button: ");
  int btn = LongPress();
  // LoRa.beginPacket();
  
  // Serial.println(btn);
  // LoRa.print(btn);
  // delay(100);
   
  // LoRa.endPacket();
  
}



void loop() {
  
  // try to parse packet
  sendStartcmd();
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    // read packet    
    while (LoRa.available())
    {
      int inChar = LoRa.read();
      
      inString += (char)inChar;
      val = inString; 
      if(val != "")
        Serial.print("Distance: ");
        Serial.println(val);
        if(val == "1"){
          long s = millis();
          digitalWrite(rel1,1);
          digitalWrite(rel2,1);
          digitalWrite(rel3,0);
          
          while(millis()-s < 3200);
          digitalWrite(rel1,0);
          digitalWrite(rel2,0);
          val = "";
        }
        if(val == "0"){
          digitalWrite(rel3,0);          
          long s = millis();
          while(millis()-s < 1200);
          digitalWrite(rel3,1);
          val = "";

        }
     }
    }
    inString = "";
    val = "";     
    LoRa.packetRssi();    
  }
