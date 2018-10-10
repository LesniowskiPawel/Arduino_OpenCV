#include "U8g2lib.h"
#include <stdlib.h>
#include <EEPROM.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

//Default baud speed for communication
#define BAUD 9600
//led 
#define led 13
//macro for on/off
#define on (digitalWrite(led, HIGH))
#define off (digitalWrite(led, LOW))
#define MAX_DATA_LENGTH 255

unsigned int suma_wr;
unsigned int suma_odcz;
int addr = 0;
//long address = 0;
byte value;
char m_str[5] = "";
char m_str2[5] = "";
String input;


void EEPROMWritelong(int address, long value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte two = (value & 0xFF);
      byte one = ((value >> 8) & 0xFF);
      
      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, two);
      EEPROM.write(address + 1, one);
      }

//This function will return a 2 byte (16bit) long from the eeprom
//at the specified address to address + 3.
long EEPROMReadlong(long address)
      {
      //Read the 2 bytes from the eeprom memory.
      long two = EEPROM.read(address);
      long one = EEPROM.read(address + 1);
      

      //Return the recomposed long by using bitshift.
      return ((two << 0) & 0xFF) + ((one << 8) & 0xFFFF);
      }

char incomingData[MAX_DATA_LENGTH];

void setup(){
  Serial.begin(BAUD);
  pinMode(led, OUTPUT);
  u8g2.begin();
}

void loop(){
  u8g2.clearBuffer();
  long address=0;
  
 //EEPROM.write(addr, address);
 //EEPROM.write(addr+1, address);
  
  int ile;
  //If any input is detected in arduino
  if(Serial.available() > 0){
    //read the whole string until '\n' delimiter is read
    input = Serial.readStringUntil('\n');
    //If input == "ON" then turn on the led 
    //and send a reply
    if (input.equals("NO")){
      digitalWrite(led, LOW);
      
      u8g2.clearBuffer();          
      u8g2.setFont(u8g2_font_logisoso30_tf);
      u8g2.drawStr(0,30,"BRAK...");
      delay(700);
      u8g2.clearBuffer();
      //strcpy(m_str, u8x8_u8toa(s, 2));    // convert m to a string with two digits 
      //strcpy(m_str, &input);
      /*
      input.toCharArray(m_str, 3);
      u8g2.firstPage();
      do {
      u8g2.drawStr(25,32,m_str);
      } while ( u8g2.nextPage() );
      delay(1000);
      //Serial.println("Led is on"); */
    } 
    
    //If input == "OFF" then turn off the led 
    //and send a reply
    else if (input!="NO") {
      digitalWrite(led,HIGH);
      u8g2.setFont(u8g2_font_logisoso26_tn);
       ile = input.toInt();
      
      //strcpy(m_str, u8x8_u8toa(s, 2));    // convert m to a string with two digits 
      //strcpy(m_str, &input);
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_logisoso16_tf);
      //u8g2.drawStr(4,27,"Nowe osoby");
      //u8g2.sendBuffer();
      delay(800);
      suma_wr = EEPROMReadlong(0);
      suma_wr += ile;
      EEPROMWritelong(address, suma_wr);
      address+=4;
      suma_odcz = suma_wr;
      itoa(suma_odcz, m_str2, 10);
      
      /*
      input.toCharArray(m_str, 3);
      u8g2.firstPage();
      do { 
          u8g2.drawStr(45,25,m_str);
      } while ( u8g2.nextPage() );
      delay(1000);
      u8g2.clearBuffer(); 
      
      //Serial.println("Led is on"); */
    } 

    //input = "rst";
    
  }
  
  
  u8g2.clearBuffer();          
  u8g2.setFont(u8g2_font_logisoso30_tf);
  u8g2.drawStr(0,30,"SUMA...");
  u8g2.sendBuffer();
  delay(800);
  u8g2.clearBuffer();          
  u8g2.setFont(u8g2_font_logisoso30_tf);
  u8g2.drawStr(0,30,"OSOB:");
  u8g2.sendBuffer();
  
  suma_odcz = EEPROMReadlong(0);
  u8g2.clearBuffer();
  delay(800);
  do {  
      u8g2.drawStr(25,30,m_str2);
      } while ( u8g2.nextPage() );
      
      //ile = 0;
      delay(600); 
      digitalWrite(led,HIGH);
      u8g2.clearBuffer(); 
      
}

