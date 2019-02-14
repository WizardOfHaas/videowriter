
#include <Keyboard.h>
//#include "SoftwareSerial.h"

#define PIN_DATA 3
#define PIN_CLK 2

//SoftwareSerial keybd_port(PIN_DATA, 1); // RX, TX

//Shift up by 32 to decode
unsigned char ascii_to_down[] = {
  0x39, // Space
  0x02, // !
  0x28, // "
  0x04, // #
  0x05, // $
  0x06, // %
  0x08, // &
  0x28, // '
  0x0A, // (
  0x0B, // )
  0x09, // *
  0x0D, // +
  0x33, // ,
  0x0C, // -
  0x34, // .
  0x35, // /
  0x0B, // 0
  0x02, // 1
  0x03, // 2
  0x04, // 3
  0x05, // 4
  0x06, // 5
  0x07, // 6
  0x08, // 7
  0x09, // 8
  0x0A, // 9
  0x27, // :
  0x27, // ;
  0x33, // <
  0x0D, // =
  0x34, // >
  0x35, // ?
  0x03, // @
  0x1E, // A
  0x30, // B
  0x2E, // C
  0x20, // D
  0x12, // E
  0x21, // F
  0x22, // G
  0x23, // H
  0x17, // I
  0x24, // J
  0x25, // K
  0x26, // L
  0x32, // M
  0x31, // N
  0x18, // O
  0x19, // P
  0x10, // Q
  0x13, // R
  0x1F, // S
  0x14, // T
  0x16, // U
  0x2F, // V
  0x11, // W
  0x2D, // X
  0x15, // Y
  0x2C, // Z
  0x1A, // [
  0x2B, // slash
  0x1B, // ]
  0x07, // ^
  0x0C, // _
  0x29, // `
  0x1E, // A (but lower case...)
  0x30, // B
  0x2E, // C
  0x20, // D
  0x12, // E
  0x21, // F
  0x22, // G
  0x23, // H
  0x17, // I
  0x24, // J
  0x25, // K
  0x26, // L
  0x32, // M
  0x31, // N
  0x18, // O
  0x19, // P
  0x10, // Q
  0x13, // R
  0x1F, // S
  0x14, // T
  0x16, // U
  0x2F, // V
  0x11, // W
  0x2D, // X
  0x15, // Y
  0x2C, // Z
  0x1A, // {
  0x2B, // |
  0x1B, // }
  0x29, // ~ (code 0x7E)
  0x00,
  0x00, //0x80
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x3A, // Caps Lock (0x90)
  0x00, // Caps lock (alt)
  0x00, // Insert
  0x00,
  0x00, // Style
  0x00,
  0x00, // Menu
  0x00,
  0x00, // Print
  0x00,
  0x00, // Spell
  0x00,
  0x00, // Page
  0x00,
  0x00, // Center
  0x00,
  0x43, //Split (0xA0) -> F9
  0x00,
  0x42, // Copy -> F8
  0x00,
  0x41, // Move -> F7
  0x00,
  0x44, // Go To -> F10
  0x00,
  0x00, // Do!
  0x00,
  0x00, // Help
  0x00,
  0x00, // Undo
  0x00,
  0x00,
  0x00,
  0x48, //Up (0xB0)
  0x00,
  0x50, // Down
  0x00,
  0x4B, // Left
  0x00,
  0x4D, // Right
  0x00,
  0x00, // Find
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x53, // Delete (0xC0)
  0x00,
  0x0F, // Tab
  0x00,
  0x0E, // Backspace
  0x00,
  0x1C, // Enter
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
}; //94 so far

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
    /*if(val == 0x90){
      ctrl_state = 1;
      sendable_key = 0;
    }else if(val == 0x91){
      ctrl_state = 0;
      sendable_key = 0;
    }*/
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
