/*
  LCD_HD44780.cpp
	Arduino Library for HD44780 based LCDs.
  Features:
	  Capable to edit text right on the screen.
		Uses only 4-bits databus to save the usage of MCU I/O lines.
		Shareable all I/O lines except EN pin.
		Fixes weird row order of the LCD.
  
  Created by ZulNs, @Gorontalo, Indonesia, 18 December 2018
  Released into the public domain.
*/

#include "LCD_HD44780.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

LCD_HD44780::LCD_HD44780(uint8_t rs, uint8_t rw, uint8_t en, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  _rs_pin = rs;
  _rw_pin = rw;
  _en_pin = en;
  _db_pins[0] = d4;
  _db_pins[1] = d5;
  _db_pins[2] = d6;
  _db_pins[3] = d7;
  en2output;
  disableControl;
}

void LCD_HD44780::begin(uint8_t numCols, uint8_t numRows)
{
  _numCols = numCols;
  _numRows = numRows;
  _numPages = (_numRows==1) ? 80/_numCols : 40/_numCols/(_numRows>>1);
  _capacity = _numCols*_numRows*_numPages;
  en2output;
  rw2output;
  rs2output;
  db2output();
  selectIR;
  writeControl;
  delay(50);
  write4bits(0x03);
  delayMicroseconds(4500);
  write4bits(0x03);
  delayMicroseconds(4500);
  write4bits(0x03);
  delayMicroseconds(150);
  write4bits(0x02);
  _displayFunction = LCD_4BITS|LCD_5x8DOTS;
  if (_numRows>1)
  {
    _displayFunction |= LCD_2LINE;
  }
  writeCommand(LCD_FUNCTIONSET | _displayFunction);
  _displayControl = LCD_DISPLAYON | LCD_CURSOROFF|LCD_BLINKOFF;  
  writeCommand(LCD_DISPLAYCONTROL | _displayControl);
  writeCommand(LCD_CLEARDISPLAY);
  _displayMode = LCD_ENTRYCURSORINCREMENT | LCD_ENTRYNOAUTOSHIFT;
  writeCommand(LCD_ENTRYMODESET | _displayMode);
  _currentPage = 0;
}

void LCD_HD44780::clear()
{
  writeCommand(LCD_CLEARDISPLAY);
  writeCommand(LCD_ENTRYMODESET | _displayMode);
  if ((_displayMode & LCD_ENTRYCURSORINCREMENT) == 0)
  {
    setCursor(_numCols-1);
  }
  _currentPage = 0;
}

void LCD_HD44780::home()
{
  writeCommand(LCD_RETURNHOME);
  if ((_displayMode & LCD_ENTRYCURSORINCREMENT) == 0)
  {
    setCursor(_numCols-1);
  }
  _currentPage = 0;
}

/*void LCD_HD44780::autoscroll()
{
  _displayMode |= LCD_ENTRYAUTOSHIFT;
  writeCommand(LCD_ENTRYMODESET | _displayMode);
}*/

/*void LCD_HD44780::noAutoscroll()
{
  _displayMode &= ~LCD_ENTRYAUTOSHIFT;
  writeCommand(LCD_ENTRYMODESET | _displayMode);
}*/

void LCD_HD44780::leftToRight()
{
  _displayMode |= LCD_ENTRYCURSORINCREMENT;
  writeCommand(LCD_ENTRYMODESET | _displayMode);
}

void LCD_HD44780::rightToLeft()
{
  _displayMode &= ~LCD_ENTRYCURSORINCREMENT;
  writeCommand(LCD_ENTRYMODESET | _displayMode);
}

void LCD_HD44780::blink()
{
  _displayControl |= LCD_BLINKON;
  writeCommand(LCD_DISPLAYCONTROL | _displayControl);
}

void LCD_HD44780::noBlink()
{
  _displayControl &= ~LCD_BLINKON;
  writeCommand(LCD_DISPLAYCONTROL | _displayControl);
}

void LCD_HD44780::cursor()
{
  _displayControl |= LCD_CURSORON;
  writeCommand(LCD_DISPLAYCONTROL | _displayControl);
}

void LCD_HD44780::noCursor()
{
  _displayControl &= ~LCD_CURSORON;
  writeCommand(LCD_DISPLAYCONTROL | _displayControl);
}

void LCD_HD44780::display()
{
  _displayControl |= LCD_DISPLAYON;
  writeCommand(LCD_DISPLAYCONTROL | _displayControl);
}

void LCD_HD44780::noDisplay()
{
  _displayControl &= ~LCD_DISPLAYON;
  writeCommand(LCD_DISPLAYCONTROL | _displayControl);
}

/*void LCD_HD44780::scrollDisplayLeft()
{
  writeCommand(LCD_CURSORSHIFT | LCD_DISPLAYSHIFT | LCD_SHIFTLEFT);
}*/

/*void LCD_HD44780::scrollDisplayRight()
{
  writeCommand(LCD_CURSORSHIFT | LCD_DISPLAYSHIFT | LCD_SHIFTRIGHT);
}*/

void LCD_HD44780::setCursor(uint8_t col, uint8_t row)
{
  setCursor(row*_numCols + col);
}

void LCD_HD44780::createChar(uint8_t location, uint8_t* charmap)
{
  uint8_t addr = readLCD(instReg);
  location &= 0x7; // we only have 8 locations 0-7
  writeCommand(LCD_SETCGRAMADDR | (location<<3));
  for (uint8_t i=0; i<8; i++)
  {
    writeLCD(dataReg, charmap[i]);
  }
  writeCommand(LCD_SETDDRAMADDR | addr);
}

/**********************************************************/

size_t LCD_HD44780::write(uint8_t chr)
{
  if (7<chr && chr<32)
  {
    if (chr==10)
    {
      newLine();
    }
  }
  else
  {
    uint8_t addr = readLCD(instReg);
    writeLCD(dataReg, chr);
    fixCursor(addr);
    gotoCursor();
  }
  return 1;
}

/**********************************************************/

void LCD_HD44780::setCursor(uint8_t pos)
{
  writeCommand(LCD_SETDDRAMADDR | getAddr(pos));
}

uint8_t LCD_HD44780::getCursor()
{
  return getPos(readLCD(instReg));
}

void LCD_HD44780::gotoCursor()
{
  uint8_t page = getPage(readLCD(instReg)), diff, cmd, i;
  if (_currentPage==page)
  {
    return;
  }
  if (_currentPage<page)
  {
    diff = page-_currentPage;
    cmd = LCD_CURSORSHIFT | LCD_DISPLAYSHIFT | LCD_SHIFTLEFT;
  }
  else
  {
    diff = _currentPage-page;
    cmd = LCD_CURSORSHIFT | LCD_DISPLAYSHIFT | LCD_SHIFTRIGHT;
  }
  diff *= _numCols;
  for (i=0; i<diff; i++)
  {
    writeCommand(cmd);
  }
  _currentPage = page;
}

void LCD_HD44780::backspace()
{
  uint8_t pos = getCursor(), basePos = pos/_numCols*_numCols, topPos = basePos+_numCols-1; 
  if ((_displayMode & LCD_ENTRYCURSORINCREMENT)==0)
  {
    if (pos<topPos)
    {
      copyString(basePos, pos, basePos+1);
      putCharAt(basePos, 32);
      setCursor(pos+1);
    }
  }
  else
  {
    if (pos>basePos)
    {
      copyString(pos, topPos, pos-1);
      putCharAt(topPos, 32);
      setCursor(pos-1);
    }
  }
}

void LCD_HD44780::insert()
{
  uint8_t pos = getCursor(), basePos = pos/_numCols*_numCols, topPos = basePos+_numCols-1; 
  if ((_displayMode & LCD_ENTRYCURSORINCREMENT)==0)
  {
    if (pos>basePos+1 && getCharAt(basePos)==32)
    {
      copyString(basePos+1, pos, basePos);
      putCharAt(pos, 32);
    }
  }
  else
  {
    if (pos<topPos-1 && getCharAt(topPos)==32)
    {
      copyString(pos, topPos-1, pos+1);
      putCharAt(pos, 32);
    }
  }
  setCursor(pos);
}

void LCD_HD44780::del()
{
  uint8_t pos = getCursor(), basePos = pos/_numCols*_numCols, topPos = basePos+_numCols-1; 
  if ((_displayMode & LCD_ENTRYCURSORINCREMENT)==0)
  {
    if (pos>basePos)
    {
      setCursor(pos-1);
      backspace();
    }
  }
  else
  {
    if (pos<topPos)
    {
      setCursor(pos+1);
      backspace();
    }
  }
}

void LCD_HD44780::clearLine()
{
  uint8_t addr = readLCD(instReg), baseAddr = getBaseAddr(addr), topAddr = baseAddr+_numCols-1, i;
  addr = ((_displayMode & LCD_ENTRYCURSORINCREMENT)==0) ? topAddr : baseAddr;
  writeCommand(LCD_SETDDRAMADDR | addr);
  for (i=baseAddr; i<=topAddr; i++)
  {
    writeLCD(dataReg, 32);
  }
  writeCommand(LCD_SETDDRAMADDR | addr);
}

void LCD_HD44780::newLine()
{
  moveCursorDown();
  lineHome();
}

void LCD_HD44780::lineHome()
{
  uint8_t addr = readLCD(instReg), baseAddr = getBaseAddr(addr), topAddr = baseAddr+_numCols-1;
  addr = ((_displayMode & LCD_ENTRYCURSORINCREMENT)==0) ? topAddr : baseAddr;
  writeCommand(LCD_SETDDRAMADDR | addr);
}

void LCD_HD44780::lineEnd()
{
  uint8_t pos = getCursor(), basePos = pos/_numCols*_numCols, topPos = basePos+_numCols-1; 
  if ((_displayMode & LCD_ENTRYCURSORINCREMENT)==0)
  {
    pos = basePos+1;
    while (pos<topPos)
    {
      if (getCharAt(pos)!=32)
      {
        pos--;
        break;
      }
      pos++;
    }
  }
  else
  {
    pos = topPos-1;
    while (pos>basePos)
    {
      if (getCharAt(pos)!=32)
      {
        pos++;
        break;
      }
      pos--;
    }
  }
  setCursor(pos);
}

void LCD_HD44780::moveCursorLeft()
{
  uint8_t pos = getCursor()-1;
  if (pos > _capacity)
  {
    pos +=_capacity;
  }
  setCursor(pos);
  gotoCursor();
}

void LCD_HD44780::moveCursorRight()
{
  setCursor(getCursor()+1);
  gotoCursor();
}

void LCD_HD44780::moveCursorUp()
{
  uint8_t pos = getCursor()-_numCols;
  if (pos>_capacity)
  {
    pos +=_capacity;
  }
  setCursor(pos);
  gotoCursor();
}

void LCD_HD44780::moveCursorDown()
{
  setCursor(getCursor()+_numCols);
  gotoCursor();
}

uint8_t LCD_HD44780::readChar(uint8_t pos)
{
  uint8_t chr, addr;
  if (pos==255)
  {
    addr = readLCD(instReg);
  }
  else
  {
    addr = getAddr(pos);
    writeCommand(LCD_SETDDRAMADDR | addr);
  }
  chr = readLCD(dataReg);
  fixCursor(addr);
  return chr;
}

void LCD_HD44780::readString(uint8_t* dest, uint8_t len, uint8_t pos)
{
  uint8_t i;
  if (pos!=255)
  {
    writeCommand(LCD_SETDDRAMADDR | getAddr(pos));
  }
  for (i=0; i<len; i++)
  {
    dest[i] = readLCD(dataReg);
  }
  dest[i] = 0;
}

/**********************************************************/

void LCD_HD44780::write4bits(uint8_t val)
{
  for (uint8_t i=0; i<4; i++)
  {
    digitalWrite(_db_pins[i], bitRead(val, i));
  }
  enableControl;
  delayMicroseconds(4);
  disableControl;
  delayMicroseconds(4);
}

uint8_t LCD_HD44780::read4bits()
{
  uint8_t val = 0;
  enableControl;
  delayMicroseconds(4);
  for (uint8_t i=0; i<4; i++)
  {
    bitWrite(val, i, digitalRead(_db_pins[i]));
  }
  disableControl;
  delayMicroseconds(4);
  return val;
}

void LCD_HD44780::writeByte(uint8_t val)
{
  write4bits(val>>4);
  write4bits(val);
}

uint8_t LCD_HD44780::readByte()
{
  return (read4bits()<<4)|read4bits();
}

void LCD_HD44780::writeLCD(uint8_t reg, uint8_t val)
{
  readLCD(instReg);  // Wait until busy flag low
  rw2output;
  rs2output;
  digitalWrite(_rs_pin, reg);
  writeControl;
  db2output();
  writeByte(val);
  db2input();
  rs2input;
  rw2input;
}

uint8_t LCD_HD44780::readLCD(uint8_t reg)
{
  uint8_t val;
  rw2output;
  rs2output;
  readControl;
  selectIR;
  while (true)
  {
    val = readByte();
    if (!bitRead(val, 7))
    {
      val = readByte();
      break;
    }
  }
  if (reg==dataReg)
  {
    selectDR;
    val = readByte();
  }
  rs2input;
  rw2input;
  return val;
}

inline void LCD_HD44780::writeCommand(uint8_t cmd)
{
  writeLCD(instReg, cmd);
}

uint8_t LCD_HD44780::getCharAt(uint8_t pos)
{
  setCursor(pos);
  return readLCD(dataReg);
}

void LCD_HD44780::putCharAt(uint8_t pos, uint8_t chr)
{
  setCursor(pos);
  writeLCD(dataReg, chr);
}

void LCD_HD44780::copyChar(uint8_t from, uint8_t to)
{
  putCharAt(to, getCharAt(from));
}

void LCD_HD44780::copyString(uint8_t fromBegin, uint8_t fromEnd, uint8_t to)
{
  uint8_t i;
  if (fromBegin<=to && to<=fromEnd)
  {
    to += fromEnd-fromBegin;
    for (i=fromEnd+1; i>fromBegin; i--)
    {
      copyChar(i-1, to--);
    }
  }
  else
  {
    for (i=fromBegin; i<=fromEnd; i++)
    {
      copyChar(i, to++);
    }
  }
}

uint8_t LCD_HD44780::getAddr(uint8_t pos)
{
  pos %= _capacity;
  if (_numRows==1)
  {
    return pos;
  }
  uint8_t row = pos/_numCols;
  return bitRead(row, 0)*0x40 + (row>>1)*_numCols + pos%_numCols;
}

uint8_t LCD_HD44780::getPos(uint8_t addr)
{
  if (_numRows==1)
  {
    return addr;
  }
  uint8_t oddRow = bitRead(addr, 6);
  bitClear(addr, 6);
  return oddRow*_numCols + ((addr/_numCols)<<1)*_numCols + addr%_numCols;
}

uint8_t LCD_HD44780::getPage(uint8_t addr)
{
  return getPos(addr)/_numCols/_numRows;
}

uint8_t LCD_HD44780::getBaseAddr(uint8_t addr)
{
  return addr - ((addr&0x3F)%_numCols);
}

void LCD_HD44780::fixCursor(uint8_t oldAddr)
{
  uint8_t addr = readLCD(instReg);
  if (getBaseAddr(oldAddr)!=getBaseAddr(addr))
  {
    if ((_displayMode & LCD_ENTRYCURSORINCREMENT)==0)
    {
      addr = getPos(oldAddr)+(_numCols<<1)-1;
    }
    else
    {
      addr = getPos(oldAddr)+1;
    }
    setCursor(addr);
  }
}

void LCD_HD44780::db2output()
{
  for (uint8_t i=0; i<4; i++)
  {
    pinMode(_db_pins[i], OUTPUT);
  }
}

void LCD_HD44780::db2input()
{
  for (uint8_t i=0; i<4; i++)
  {
    pinMode(_db_pins[i], INPUT);
  }
}
