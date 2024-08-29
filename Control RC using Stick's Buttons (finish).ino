//OLED Libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// include libraries PS2X
#include <PS2X_lib.h>
PS2X ps2x; // create PS2 Controller Class
int error = 0; 
byte type = 0;
byte vibrate = 0;

// Driver motor's pins 
const int ENA_PIN = 2;
const int IN1_PIN = 22;
const int IN2_PIN = 24;
const int ENB_PIN = 3;
const int IN3_PIN = 26;
const int IN4_PIN = 28;
const int ENC_PIN = 4;
const int IN5_PIN = 30;
const int IN6_PIN = 32;
const int END_PIN = 5;
const int IN7_PIN = 34;
const int IN8_PIN = 36;

//Servo library
#include <Servo.h>
//inisiasi servo pin
Servo servo1;
Servo servo2;
Servo servo3;

void setup(){

//Membangkitkan output tegangan servo data dari 0 sampai 0,5 volt pada pin 11, 9, 10 (PWM)
servo1.attach(8); //turun  
servo2.attach(9); //buka   
servo3.attach(10); //buka

 // Configure output pins
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(ENC_PIN, OUTPUT);
  pinMode(IN5_PIN, OUTPUT);
  pinMode(IN6_PIN, OUTPUT);
  pinMode(END_PIN, OUTPUT);
  pinMode(IN7_PIN, OUTPUT);
  pinMode(IN8_PIN, OUTPUT);

 // Disable both motors
 digitalWrite(ENA_PIN,0);
 digitalWrite(ENB_PIN,0);
 digitalWrite(ENC_PIN,0);
 digitalWrite(END_PIN,0);

 // Start serial communication
 Serial.begin(57600);
  
 error = ps2x.config_gamepad(52,51,53,50, false , true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

 // Check for error
 if(error == 0){
  Serial.println("Found Controller, configured successful");
 }
   
 else if(error == 1)
  Serial.println("No controller found, check wiring or reset the Arduino");
   
 else if(error == 2)
  Serial.println("Controller found but not accepting commands");
  
 else if(error == 3)
  Serial.println("Controller refusing to enter Pressures mode, may not support it.");
   
 // Check for the type of controller
 type = ps2x.readType();
 switch(type) {
  case 0:
    Serial.println("Unknown Controller type");
    break;
  case 1:
    Serial.println("DualShock Controller Found");
    break;
  case 2:
    Serial.println("GuitarHero Controller Found");
    break;
  }

// initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

  oled.setTextSize(2);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(10, 10);        // position to display
  oled.println("TRANSPORTER");     // text to display
  oled.setCursor(27,40 );  
  oled.println("ROBOT");
  oled.display();               // show on OLED
}

// Main loop
void loop(){
   
 if(error == 1) //skip loop if no controller found
  return; 
  
 else { //DualShock Controller
  
  ps2x.read_gamepad(false, vibrate); // disable vibration of the controller       
                        
  //Servo turun
  int value1 = ps2x.Analog(PSS_RY); //analog R gerakan atas bawah
  int angle1 = map(value1, 0, 255, 135, 45); // sudut servo dari 135 sampai 45 derajat dengan titik tengah(netral) 90 derajat
  servo1.write(angle1); // rotate servo motor 1
  // Yang ditampilkan di serial monitor (penting!!!)
  Serial.print("Joystick: ");
  Serial.print(value1);
  Serial.print(", ");
  Serial.print(" => Servo Motor: ");
  Serial.print(angle1);
  Serial.print("°, ");
  Serial.println("°");
  
  //Servo buka 1
  int value2 = ps2x.Analog(PSS_LX); //analog L gerakan kanan kiri
  int angle2 = map(value2, 0, 255, 45, 135); // 45 sampai 135 derajat
  servo2.write(angle2);  
  //serial print
  Serial.print("Joystick: ");
  Serial.print(value2);
  Serial.print(", ");
  Serial.print(" => Servo Motor: ");
  Serial.print(angle2);
  Serial.print("°, ");
  Serial.println("°");
  
  //Servo buka 2
  int value3 = ps2x.Analog(PSS_LX); //analog L gerakan kanan kiri
  int angle3 = map(value3, 0, 255, 135, 45);
  servo3.write(angle3);
  //Serial print
  Serial.print("Joystick: ");
  Serial.print(value3);
  Serial.print(", ");
  Serial.print(" => Servo Motor: ");
  Serial.print(angle3);
  Serial.print("°, ");
  Serial.println("°");      
  
  //Roda Mekanum
      if(ps2x.Button(PSB_PAD_UP))
      {
        Serial.println("Maju");
        //Maju                 
        digitalWrite(IN1_PIN, HIGH); // control motor 1 maju
        digitalWrite(IN2_PIN, LOW);  
        digitalWrite(IN3_PIN, HIGH); // control motor 2 maju
        digitalWrite(IN4_PIN, LOW);
        digitalWrite(IN5_PIN, HIGH); // control motor 3 maju
        digitalWrite(IN6_PIN, LOW);  
        digitalWrite(IN7_PIN, HIGH); // control motor 4 maju
        digitalWrite(IN8_PIN, LOW);          
        analogWrite(ENA_PIN, 1023); // control the speed
        analogWrite(ENB_PIN, 1023); // control the speed 
        analogWrite(ENC_PIN, 1023); // control the speed
        analogWrite(END_PIN, 1023); // control the speed            
      }
      if(ps2x.Button(PSB_PAD_DOWN))
      {
        Serial.println("Mundur");
        digitalWrite(IN1_PIN, LOW); // control motor 1 mundur
        digitalWrite(IN2_PIN, HIGH);  
        digitalWrite(IN3_PIN, LOW); // control motor 2 mundur
        digitalWrite(IN4_PIN, HIGH);
        digitalWrite(IN5_PIN, LOW); // control motor 3 mundur
        digitalWrite(IN6_PIN, HIGH);  
        digitalWrite(IN7_PIN, LOW); // control motor 4 mundur
        digitalWrite(IN8_PIN, HIGH);          
        analogWrite(ENA_PIN, 1023); // control the speed
        analogWrite(ENB_PIN, 1023); // control the speed 
        analogWrite(ENC_PIN, 1023); // control the speed
        analogWrite(END_PIN, 1023); // control the speed
      }
      if(ps2x.Button(PSB_PAD_RIGHT))
      {
        Serial.println("Kanan");
        digitalWrite(IN1_PIN, HIGH); // control motor 1 maju
        digitalWrite(IN2_PIN, LOW);  
        digitalWrite(IN3_PIN, LOW); // control motor 2 mundur
        digitalWrite(IN4_PIN, HIGH);
        digitalWrite(IN5_PIN, LOW); // control motor 3 mundur
        digitalWrite(IN6_PIN, HIGH);  
        digitalWrite(IN7_PIN, HIGH); // control motor 4 maju
        digitalWrite(IN8_PIN, LOW);          
        analogWrite(ENA_PIN, 1023); // control the speed
        analogWrite(ENB_PIN, 1023); // control the speed 
        analogWrite(ENC_PIN, 1023); // control the speed
        analogWrite(END_PIN, 1023); // control the speed
      }
      if(ps2x.Button(PSB_PAD_LEFT))
      {
        Serial.println("Kiri");
        digitalWrite(IN1_PIN, LOW); // control motor 1 mundur
        digitalWrite(IN2_PIN, HIGH);  
        digitalWrite(IN3_PIN, HIGH); // control motor 2 maju
        digitalWrite(IN4_PIN, LOW); 
        digitalWrite(IN5_PIN, HIGH); // control motor 3 maju
        digitalWrite(IN6_PIN, LOW);  
        digitalWrite(IN7_PIN, LOW); // control motor 4 mundur
        digitalWrite(IN8_PIN, HIGH);          
        analogWrite(ENA_PIN, 1023); // control the speed
        analogWrite(ENB_PIN, 1023); // control the speed 
        analogWrite(ENC_PIN, 1023); // control the speed
        analogWrite(END_PIN, 1023); // control the speed
      }
      if(ps2x.Button(PSB_PAD_UP) && ps2x.Button(PSB_PAD_RIGHT))
      {
        Serial.println("Maju + Kanan");
        digitalWrite(IN1_PIN, HIGH); // control motor 1 maju
        digitalWrite(IN2_PIN, LOW);  
        digitalWrite(IN3_PIN, LOW); // control motor 2 off
        digitalWrite(IN4_PIN, LOW);
        digitalWrite(IN5_PIN, LOW); // control motor 3 off
        digitalWrite(IN6_PIN, LOW);  
        digitalWrite(IN7_PIN, HIGH); // control motor 4 maju
        digitalWrite(IN8_PIN, LOW);          
        analogWrite(ENA_PIN, 1023); // control the speed
        //analogWrite(ENB_PIN, 1023); // control the speed 
        //analogWrite(ENC_PIN, 1023); // control the speed
        analogWrite(END_PIN, 1023); // control the speed
      }
      if(ps2x.Button(PSB_PAD_UP) && ps2x.Button(PSB_PAD_LEFT))
      {
        Serial.println("Maju + Kiri");
        digitalWrite(IN1_PIN, LOW); // control motor 1 off
        digitalWrite(IN2_PIN, LOW);  
        digitalWrite(IN3_PIN, HIGH); // control motor 2 maju
        digitalWrite(IN4_PIN, LOW);
        digitalWrite(IN5_PIN, HIGH); // control motor 3 maju
        digitalWrite(IN6_PIN, LOW);  
        digitalWrite(IN7_PIN, LOW); // control motor 4 off
        digitalWrite(IN8_PIN, LOW);          
        //analogWrite(ENA_PIN, 1023); // control the speed
        analogWrite(ENB_PIN, 1023); // control the speed 
        analogWrite(ENC_PIN, 1023); // control the speed
        //analogWrite(END_PIN, 1023); // control the speed
      } 
      if(ps2x.Button(PSB_PAD_DOWN) && ps2x.Button(PSB_PAD_RIGHT))
      {
        Serial.println("Mundur + Kanan");
        digitalWrite(IN1_PIN, LOW); // control motor 1 off
        digitalWrite(IN2_PIN, LOW);  
        digitalWrite(IN3_PIN, LOW); // control motor 2 mundur
        digitalWrite(IN4_PIN, HIGH);
        digitalWrite(IN5_PIN, LOW); // control motor 3 mundur
        digitalWrite(IN6_PIN, HIGH);  
        digitalWrite(IN7_PIN, LOW); // control motor 4 off
        digitalWrite(IN8_PIN, LOW);          
        //analogWrite(ENA_PIN, 1023); // control the speed
        analogWrite(ENB_PIN, 1023); // control the speed 
        analogWrite(ENC_PIN, 1023); // control the speed
        //analogWrite(END_PIN, 1023); // control the speed
      }
      if(ps2x.Button(PSB_PAD_DOWN) && ps2x.Button(PSB_PAD_LEFT))
      {
        Serial.println("Mundur + Kiri");
        digitalWrite(IN1_PIN, LOW); // control motor 1 mundur
        digitalWrite(IN2_PIN, HIGH);  
        digitalWrite(IN3_PIN, LOW); // control motor 2 off
        digitalWrite(IN4_PIN, LOW);
        digitalWrite(IN5_PIN, LOW); // control motor 3 off
        digitalWrite(IN6_PIN, LOW);  
        digitalWrite(IN7_PIN, LOW); // control motor 4 mundur
        digitalWrite(IN8_PIN, HIGH);          
        analogWrite(ENA_PIN, 1023); // control the speed
        //analogWrite(ENB_PIN, 1023); // control the speed 
        //analogWrite(ENC_PIN, 1023); // control the speed
        analogWrite(END_PIN, 1023); // control the speed
      }
      if(ps2x.Button(PSB_CIRCLE) && ps2x.Button(PSB_PAD_RIGHT))
      {
        Serial.println("Putar Kanan");
        digitalWrite(IN1_PIN, HIGH); // control motor 1 maju
        digitalWrite(IN2_PIN, LOW);  
        digitalWrite(IN3_PIN, LOW); // control motor 2 mundur
        digitalWrite(IN4_PIN, HIGH);
        digitalWrite(IN5_PIN, HIGH); // control motor 3 maju
        digitalWrite(IN6_PIN, LOW);  
        digitalWrite(IN7_PIN, LOW); // control motor 4 mundur
        digitalWrite(IN8_PIN, HIGH);          
        analogWrite(ENA_PIN, 1023); // control the speed
        analogWrite(ENB_PIN, 1023); // control the speed 
        analogWrite(ENC_PIN, 1023); // control the speed
        analogWrite(END_PIN, 1023); // control the speed
      }
      if(ps2x.Button(PSB_CIRCLE) && ps2x.Button(PSB_PAD_LEFT))
      {
        Serial.println("Putar Kiri");
        digitalWrite(IN1_PIN, LOW); // control motor 1 mundur
        digitalWrite(IN2_PIN, HIGH);  
        digitalWrite(IN3_PIN, HIGH); // control motor 2 maju
        digitalWrite(IN4_PIN, LOW);
        digitalWrite(IN5_PIN, LOW); // control motor 3 mundur
        digitalWrite(IN6_PIN, HIGH);  
        digitalWrite(IN7_PIN, HIGH); // control motor 4 maju
        digitalWrite(IN8_PIN, LOW);          
        analogWrite(ENA_PIN, 1023); // control the speed
        analogWrite(ENB_PIN, 1023); // control the speed 
        analogWrite(ENC_PIN, 1023); // control the speed
        analogWrite(END_PIN, 1023); // control the speed
      }
      if (!ps2x.Button(PSB_PAD_DOWN) && !ps2x.Button(PSB_PAD_UP) && !ps2x.Button(PSB_PAD_RIGHT) && !ps2x.Button(PSB_PAD_LEFT) && !ps2x.Button(PSB_CIRCLE)) 
      {
        analogWrite(ENA_PIN, 0); // control the speed
        analogWrite(ENB_PIN, 0); // control the speed
        analogWrite(ENC_PIN, 0); // control the speed
        analogWrite(END_PIN, 0); // control the speed
      }        
                                                      
    } 
     
 delay(50);
 }    
