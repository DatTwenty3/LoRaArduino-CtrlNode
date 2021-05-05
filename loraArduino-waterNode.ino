#include <SoftwareSerial.h>
#define RX 6
#define TX 7
SoftwareSerial loraSerial(RX, TX); // RX, TX
//AS Sensor
#define rxSensor 2
#define txSensor 3
SoftwareSerial sensorSerial(rxSensor, txSensor);
int Pin_x = 4;
int Pin_y = 5;
char computerdata[20];
char sensordata[30];
byte computer_bytes_received=0;
byte sensor_bytes_received=0;
char *cmd;
int counterSensorAS;
float dataToFloat;
int pH;
int ORP;
//TIMER
long clkTime = 0;
//LCD 2004
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
  loraSerial.begin(9600);
  //AS SENSOR SERIAL SETUP
  pinMode(Pin_x, OUTPUT);
  pinMode(Pin_y, OUTPUT);
  sensorSerial.begin(9600); 

  //LCD 2004
  lcd.init();
  lcd.init();

  lcd.backlight();
  lcd.print("**LEDAT WATER NODE**");
  lcd.setCursor(0,2);
  lcd.print("pH:");
  lcd.setCursor(10,2);
  lcd.print("ORP:");
}

//FUNCTION AS SENSOR
void serialEvent(){                                                 
  sensor_bytes_received=sensorSerial.readBytesUntil(13,sensordata,30); 
  sensordata[sensor_bytes_received]=0;
}

void loop() {
  if(millis() - clkTime > 20000){
    clkTime = millis();
    //AS Sensor
    loraSerial.print(" ORP" + (String)ORP);
    Serial.println("Sent ORP parametter!");
    delay(500);
    loraSerial.print(" pH" + (String)pH);
    Serial.println("Sent pH parametter!");
    delay(500);

    //LCD 2004
    //clear LCD
    lcd.setCursor(3,2);
    lcd.print("  ");
    lcd.setCursor(14,2);
    lcd.print("    ");
    //Display para
    lcd.setCursor(3,2);
    lcd.print((String)pH);
    lcd.setCursor(14,2);
    lcd.print((String)ORP);
    }
  //AS SENSOR
  if(counterSensorAS  == 0){
      open_channel (1);
      counterSensorAS = 1;
      }
    else{
      open_channel (2);
      counterSensorAS = 0;
      }
  if(sensorSerial.available() > 0){
    sensor_bytes_received=sensorSerial.readBytesUntil(13,sensordata,30);
    sensordata[sensor_bytes_received]=0;
    dataToFloat = atof(sensordata);
    if (dataToFloat >= 0 && dataToFloat <= 14){
      pH = dataToFloat;
      Serial.print("pH: ");
      Serial.println(pH);
      }
    else {
      ORP = dataToFloat;
      Serial.print("ORP: ");
      Serial.println(ORP);
      }
  } delay(1000);
}
//FUNCTION AS SENSOR
void open_channel(int channel){                          
     switch (channel) {                               
       case 1:                                       
         digitalWrite(Pin_x, LOW);                   
         digitalWrite(Pin_y, LOW);                  
       break;                                
        
       case 2:
         digitalWrite(Pin_x, LOW);
         digitalWrite(Pin_y, HIGH);
       break;

       case 3:
         digitalWrite(Pin_x, HIGH);
         digitalWrite(Pin_y, LOW);
       break;

       case 4:
         digitalWrite(Pin_x, HIGH);
         digitalWrite(Pin_y, HIGH);
       break;
      }
}    
