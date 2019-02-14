#include <Keyboard.h>

char ascii_to_scan[255];
char ctrl_state = 0;
char alt_state = 0;
char sendable_key = 0;

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);

  //Define special keys
  ascii_to_scan[0xC4] = KEY_BACKSPACE;
  ascii_to_scan[0xC2] = KEY_TAB;
  ascii_to_scan[0xC0] = '`';        //delete
  ascii_to_scan[0x91] = KEY_CAPS_LOCK;  //Gotta remap this into a layer lock(maybe control?)
  ascii_to_scan[0x92] = KEY_LEFT_CTRL;  //(insert) This will fucking suck to deal with...
  ascii_to_scan[0xC6] = KEY_RETURN;
  ascii_to_scan[0xB0] = KEY_UP_ARROW;
  ascii_to_scan[0xB2] = KEY_DOWN_ARROW;
  ascii_to_scan[0xB4] = KEY_LEFT_ARROW;
  ascii_to_scan[0xB6] = KEY_RIGHT_ARROW;
  
  ascii_to_scan[0xA8] = KEY_ESC;          //DO key
  ascii_to_scan[0x96] = KEY_HOME;         //Menu key
  ascii_to_scan[0x98] = KEY_PAGE_UP;      //Print key
  ascii_to_scan[0xAA] = KEY_END;          //Help key
  ascii_to_scan[0xAC] = KEY_PAGE_DOWN;    //Undo key

  ascii_to_scan[0x94] = KEY_ESC;//KEY_F1;  //STYLE
  ascii_to_scan[0x9E] = KEY_F2;  //CENTER
  ascii_to_scan[0x9C] = KEY_F3;  //PAGE
  ascii_to_scan[0xB8] = KEY_F4;  //FIND
  ascii_to_scan[0x9A] = KEY_F5;  //SPELL
  ascii_to_scan[0xA4] = KEY_F6;  //MOVE
  ascii_to_scan[0xA2] = KEY_F7;  //COPY
  ascii_to_scan[0xA0] = KEY_F8;  //SPLIT
  ascii_to_scan[0xA6] = KEY_F9;  //GOTO

  ascii_to_scan[0xC1] = '~';      //. + DELETE
  //. + TAB
  ascii_to_scan[0xC5] = KEY_DELETE;   //. + BACK SPACE
  //. + MENU
  //. + STYLE
  //. + SPELL
  //. + Up
  //. + Down
  //. + Left
  //. + Right
  ascii_to_scan[0xAB] = '|';     //. + HELP

  Keyboard.begin();
}

void loop(){
   if(Serial1.available()){
    unsigned char val = Serial1.read();
    sendable_key = 1;

    Serial.print(val, HEX);
    Serial.print(" -- > ");

    //Update control lock
    if(val == 0xA8){
      sendable_key = 0;
      
      if(ctrl_state == 0){
        ctrl_state = 1;
        Keyboard.press(KEY_LEFT_CTRL);
      }else{
        ctrl_state = 0;
        Keyboard.release(KEY_LEFT_CTRL);
      }
    }

    //Update alt lock
    if(val == 0x92){
      sendable_key = 0;
      
      if(alt_state == 0){
        alt_state = 1;
        Keyboard.press(KEY_LEFT_ALT);
      }else{
        alt_state = 0;
        Keyboard.release(KEY_LEFT_ALT);
      }
    }

    //Do translation if outside of alpha-numeric block
    if(val < 32 || val >= 126){
      val = ascii_to_scan[val];
    }

    //Press control if lock is on
    if(ctrl_state != 0){
      //Keyboard.press(KEY_LEFT_CTRL);
      //Serial.print("CTRL ");
    }

    //Press alt if lock is on
    if(alt_state != 0){
      //Keyboard.press(KEY_LEFT_ALT);
      //Serial.print("ALT ");
    }

    //Check if we have an actual key press to send
    if(sendable_key == 1){
      Keyboard.press(val);
      Keyboard.release(val);
    }
    
    //Keyboard.releaseAll();

    Serial.print(val, HEX);
    Serial.print("\n");
  }
}
