/*
  LCD_HD44780.h
	Arduino Library for HD44780 based LCDs.
  Features:
	  Capable to edit text right on the screen.
		Uses only 4-bits databus to save the usage of MCU I/O lines.
		Shareable all I/O lines except EN pin.
		Fixes weird row order of the LCD.
  
  Created by ZulNs, @Gorontalo, Indonesia, 18 December 2018
  Released into the public domain.
*/

#ifndef LCD_HD44780_h
#define LCD_HD44780_h

#include <inttypes.h>
#include "Print.h"

// instructions
#define LCD_CLEARDISPLAY          0x01
#define LCD_RETURNHOME            0x02
#define LCD_ENTRYMODESET          0x04
#define LCD_DISPLAYCONTROL        0x08
#define LCD_CURSORSHIFT           0x10
#define LCD_FUNCTIONSET           0x20
#define LCD_SETCGRAMADDR          0x40
#define LCD_SETDDRAMADDR          0x80

// flags for entry mode set
#define LCD_ENTRYAUTOSHIFT        0x01
#define LCD_ENTRYNOAUTOSHIFT      0x00
#define LCD_ENTRYCURSORINCREMENT  0x02
#define LCD_ENTRYCURSORDECREMENT  0x00

// flags for display on/off control
#define LCD_BLINKON               0x01
#define LCD_BLINKOFF              0x00
#define LCD_CURSORON              0x02
#define LCD_CURSOROFF             0x00
#define LCD_DISPLAYON             0x04
#define LCD_DISPLAYOFF            0x00

// flags for display/cursor shift
#define LCD_SHIFTRIGHT            0x04
#define LCD_SHIFTLEFT             0x00
#define LCD_DISPLAYSHIFT          0x08
#define LCD_CURSORMOVE            0x00

// flags for function set
#define LCD_5x10DOTS              0x04
#define LCD_5x8DOTS               0x00
#define LCD_2LINE                 0x08
#define LCD_1LINE                 0x00
#define LCD_8BITS                 0x10
#define LCD_4BITS                 0x00

#define enableControl   ( digitalWrite(_en_pin, HIGH  ) )
#define disableControl  ( digitalWrite(_en_pin, LOW   ) )
#define readControl     ( digitalWrite(_rw_pin, HIGH  ) )
#define writeControl    ( digitalWrite(_rw_pin, LOW   ) )
#define selectDR        ( digitalWrite(_rs_pin, HIGH  ) )
#define selectIR        ( digitalWrite(_rs_pin, LOW   ) )
#define en2output       ( pinMode     (_en_pin, OUTPUT) )
#define rs2output       ( pinMode     (_rs_pin, OUTPUT) )
#define rs2input        ( pinMode     (_rs_pin, INPUT ) )
#define rw2output       ( pinMode     (_rw_pin, OUTPUT) )
#define rw2input        ( pinMode     (_rw_pin, INPUT ) )
#define instReg           LOW
#define dataReg           HIGH

class LCD_HD44780 : public Print
{
public:
  LCD_HD44780(uint8_t rs, uint8_t rw, uint8_t en, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  void begin(uint8_t numCols=16, uint8_t numRows=2);
  
  void clear();
  
  void home();

  //void autoscroll();
  //void noAutoscroll();
  void leftToRight();
  void rightToLeft();
  
  void blink();
  void noBlink();
  void cursor();
  void noCursor();
  void display();
  void noDisplay();
  
  //void scrollDisplayLeft();
  //void scrollDisplayRight();
  void setCursor(uint8_t col, uint8_t row);
  
  void createChar(uint8_t location, uint8_t* charmap);
  
  virtual size_t write(uint8_t chr);
  
  using Print::write;
  
  void    setCursor(uint8_t pos);
  uint8_t getCursor();
  void    gotoCursor();
  void    backspace();
  void    insert();
  void    del();
  void    clearLine();
  void    newLine();
  void    lineHome();
  void    lineEnd();
  void    moveCursorLeft();
  void    moveCursorRight();
  void    moveCursorUp();
  void    moveCursorDown();
  uint8_t readChar(uint8_t pos=255);
  void    readString(uint8_t* dest, uint8_t len, uint8_t pos=255);
  
private:
  void    write4bits(uint8_t val);
  uint8_t read4bits();
  void    writeByte(uint8_t val);
  uint8_t readByte();
  void    writeLCD(uint8_t reg, uint8_t val);
  uint8_t readLCD(uint8_t reg);
  void    writeCommand(uint8_t cmd);
  uint8_t getCharAt(uint8_t pos);
  void    putCharAt(uint8_t pos, uint8_t chr);
  void    copyChar(uint8_t from, uint8_t to);
  void    copyString(uint8_t fromBegin, uint8_t fromEnd, uint8_t to);
  uint8_t getAddr(uint8_t pos);
  uint8_t getPos(uint8_t addr);
  uint8_t getPage(uint8_t addr);
  uint8_t getBaseAddr(uint8_t addr);
  void    fixCursor(uint8_t oldAddr);
  void    db2output();
  void    db2input();
  
  uint8_t _db_pins[4];
  uint8_t _en_pin;
  uint8_t _rw_pin;
  uint8_t _rs_pin;
  
  uint8_t _numCols;
  uint8_t _numRows;
  uint8_t _numPages;
  uint8_t _capacity;
  uint8_t _displayFunction;
  uint8_t _displayControl;
  uint8_t _displayMode;
  uint8_t _currentPage;
};

#endif
