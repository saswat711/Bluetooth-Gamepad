 
#include <Arduino.h>
#include <Gamepad.h>
#include <Bounce2.h> // For debounceing the switches immediately

#define BOUNCE_WITH_PROMPT_DETECTION // Make button state changes available immediately

Gamepad bleGamepad;

// Defining Inputs pins
     //joystick
#define xsP 32
#define ysP 33

                                                                              
#define zP 34
#define rzP 35

// trigger
#define ltriP 13
#define rtriP 26

//Control buttons
#define aP 15

#define bP 23
#define xP 21
#define yP 19
#define lbP 18
#define rbP 2

// Function buttons
#define selP 5
#define startP 4

// For creating multiple bounce objects
#define noButtons 10
byte buttonPins[noButtons] = {(byte)ltriP,(byte)rtriP, (byte)aP, (byte)bP, (byte)xP, (byte)yP, (byte)lbP, (byte)rbP, (byte)selP, (byte)startP};
Bounce debouncers[noButtons];

// Joysticks Integer
int XS = 0;
int YS = 0;
int ZS = 0;
int RZS = 0;

// for Trigger
int lTrig = 0 ;
int rTrig = 0;

// for Buttons



int A = 0;
int B = 0;
int X = 0;
int Y = 0;
int sel = 0;
int start = 0;
int Lb = 0;
int Rb = 0;



// function to read joystick values -------------------------------------------->
void readJoy(){
  // For right joystick
  XS = analogRead(xsP);
  YS = analogRead(ysP);
  // for left joystick
  ZS = analogRead(zP);
  RZS = analogRead(rzP);

  Serial.println((String)"X VALUE: "+XS);
  Serial.println((String)"Y VALUE: "+YS);
  Serial.println((String)"Z VALUE: "+ZS);
  Serial.println((String)"RZ VALUE: "+RZS);     
}
 


// function to read trigger values -------------------------------------------->
//buttonPins index 0-ltriP, 1-rtriP  
void readTrig(){
  // For Triggers
  rTrig = debouncers[0].read();
  lTrig = debouncers[1].read();
  Serial.println((String)"Trigger Values: left Trig- "+lTrig+" Right Trig: "+rTrig);
}



// function to read Buttons values -------------------------------------------->
void readButs(){
  // to read buttons
  //abxy
  //buttonPins index 2-aP, 3-bP,  4-xP, 5-yP
  A = debouncers[2].read();
  B = debouncers[3].read();
  X = debouncers[4].read();
  Y = debouncers[5].read();

   //buttonPins index 6-lbP, 7-rbP, 
  Lb = debouncers[8].read();
  Rb = debouncers[9].read();

// To print serial values
  Serial.println((String)"A, B, X, Y: "+A+" "+B+" "+X+" "+Y);
  Serial.println((String)"leftBtn, rightBtn: "+Lb+" "+Rb);
}


// Function to read function button values --------------------------------------------------->
void readFun(){
    // select,start
   // button pins index 8-selP, 9-startP
  sel = debouncers[6].read();
  start = debouncers[7].read();
 Serial.println((String)"select, start: "+sel+" "+start);
}


// Updating  Jotstick Values -------------------------------------------->
void updateJoyVal(){
  // updating joystick values
    Serial.println((String)"Updating Values: ");
    XS = map(XS,0,4095, 32767, -32767);
    YS = map(YS,0,4095, -32767, 32767);  
    ZS = map(ZS,0,4095, 32767, -32767);  
    RZS = map(RZS,0,4095, 32767, -32767);  

    bleGamepad.setLeftThumb(XS,YS);
    bleGamepad.setRightThumb(ZS,RZS);
    Serial.println((String)"Updated joysticks Values: ");     
}

// FOr updating trigger values
void updateTrigVal(){
  if(!lTrig){
     bleGamepad.setLeftTrigger(32767);
  }
  else{
     bleGamepad.setLeftTrigger(0);
  }
  if(!rTrig){
     bleGamepad.setRightTrigger(32767);
  }
  else{
       bleGamepad.setRightTrigger(0);
  }
}

// For updating buttons values -------------------------------------------->
void updateBtnVal(){
    // For A button
   if(!A){
        bleGamepad.press(BUTTON_1);
  }
  else{
     bleGamepad.release(BUTTON_1);
  }
    // For B button
  if(!B){
        bleGamepad.press(BUTTON_2);
  }
  else{
      bleGamepad.release(BUTTON_2);
  }
    // For X button
  if(!X){
        bleGamepad.press(BUTTON_3);
  }
  else{
      bleGamepad.release(BUTTON_3);
  }
    // For Y button
  if(!Y){
        bleGamepad.press(BUTTON_4);
  }
  else{
      bleGamepad.release(BUTTON_4);
  }
    // For Left button
  if(!Lb){
     bleGamepad.press(BUTTON_5);
  }
  else{
      bleGamepad.release(BUTTON_5);
  }
  // For right button
  if(!Rb){
     bleGamepad.press(BUTTON_6);
  }
  else{
      bleGamepad.release(BUTTON_6);
  }   
}

// To update function btn values -------------------------------------------->
void updateFunVal(){ 
    // For select function
  if(!sel){
    bleGamepad.pressSelect();
    bleGamepad.releaseSelect();
  }
  else{
      bleGamepad.releaseSelect();
  }
  // For start function
  if(!start){
     bleGamepad.pressStart();
     bleGamepad.releaseStart();
  }
  else{
    bleGamepad.releaseStart();
  }
}


void setup()
{
  // for attachign debouncers with the pins
   for(byte i=0; i< noButtons;i++){
    debouncers[i] = Bounce();
    debouncers[i].attach(buttonPins[i], INPUT_PULLUP);
    debouncers[i].interval(5);
    }    
    // The default bleGamepad.begin() above enables 16 buttons, all axes, one hat, start and select and no simulation controls
        bleGamepad.begin();
        Serial.begin(115200);
        Serial.println("Starting BLE work!");
        
}


void loop()
{
// put your main code here, to run repeatedly:
     
   if (bleGamepad.isConnected())
    {
        Serial.println("Ble is connected");
       for(byte i=0; i< noButtons;i++)
         {
           debouncers[i].update();
         }
         
        // reading joystic values
        readJoy();
        // reading trigger values
        readTrig();
        // reading buttons values
        readButs();
        // reading function values
        readFun();
        
       // ------------------------------------------------->
       
        // updating joystick values 
        updateJoyVal();
        // updating trig values
        updateTrigVal();
        // Updating btn values
        updateBtnVal();
        // updating Fun values
        updateFunVal();  
        
    }
   else
    {
        Serial.println("Ble is not connected!"); 
                delay(500);
    }
}
