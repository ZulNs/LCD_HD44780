/**********************************************************
 *  Typewriting Demo on LCD 1602 using 4x4 Matrix Keypad  *
 **********************************************************
 * 
 * Description:
 *   This example exposed the library capabilities to edit some text right on the LCD screen.
 *   By using this LCD library you can share the I/O lines used by the LCD with other hardware.
 *   In this example digital pins D4, D5, D6, and D7 are used in common with the LCD databus pins
 *   and the matrix keypad column pins.
 *   As you can see in this example, the library has fixed the weird row order of the LCD.
 * 
 * 
 * Keypad map:
 *   In Alphabetic Mode:
 *     Multi tap:
 * 
 *     [Symbols]   [ A B C ]   [ D E F ]   [BackSpc]
 * 
 *     [ G H I ]   [ J K L ]   [ M N O ]   [ABC abc]
 * 
 *     [P Q R S]   [ T U V ]   [W X Y Z]   [  Up   ]
 * 
 *     [ Left  ]   [ Space ]   [ Right ]   [ Down  ]
 * 
 * 
 *     Long tap:
 * 
 *     [   1   ]   [   2   ]   [   3   ]   [ClrScrn]
 * 
 *     [   4   ]   [   5   ]   [   6   ]   [ABC 123]
 * 
 *     [   7   ]   [   8   ]   [   9   ]   [ClrLine]
 * 
 *     [LnHome ]   [   0   ]   [ LnEnd ]   [NewLine]
 * 
 * 
 *   In Numeric Mode:
 *     Single tap:
 * 
 *     [   1   ]   [   2   ]   [   3   ]   [BackSpc]
 * 
 *     [   4   ]   [   5   ]   [   6   ]   [123 Edt]
 * 
 *     [   7   ]   [   8   ]   [   9   ]   [  Up   ]
 * 
 *     [ Left  ]   [   0   ]   [ Right ]   [ Down  ]
 * 
 * 
 *     Long tap:
 * 
 *     [       ]   [       ]   [       ]   [ClrScrn]
 * 
 *     [       ]   [       ]   [       ]   [ABC 123]
 * 
 *     [       ]   [       ]   [       ]   [ Home  ]
 * 
 *     [LnHome ]   [       ]   [ LnEnd ]   [NewLine]
 * 
 * 
 *   In Edit Mode:
 *     Single tap:
 * 
 *     [   1   ]   [   2   ]   [   3   ]   [BackSpc]
 * 
 *     [   4   ]   [   5   ]   [   6   ]   [123 Edt]
 * 
 *     [   7   ]   [   8   ]   [   9   ]   [Insert ]
 * 
 *     [ Left  ]   [   0   ]   [ Right ]   [Delete ]
 * 
 * 
 *     Long tap:
 * 
 *     [       ]   [       ]   [       ]   [ClrScrn]
 * 
 *     [       ]   [       ]   [       ]   [ABC 123]
 * 
 *     [       ]   [       ]   [       ]   [       ]
 * 
 *     [LnHome ]   [       ]   [ LnEnd ]   [       ]
 * 
 * 
 * The circuit:
 *    ___________
 *   |           |
 *   | HD44780   |
 *   | based LCD |
 *   |           |
 *   |       Vss |  ----  5V
 *   |       Vdd |  -------------------------------------------  GND
 *   |        V0 |  --------------  [ 10kΩ Pot ]  -------------  GND
 *   |        RS |  ----  D2
 *   |        RW |  ----  D3
 *   |        EN |  ----  A2
 *   |       DB0 |
 *   |       DB1 |        A1  ----  [ Buzzer ]  ---------------  GND
 *   |       DB2 |        A0  ----  [ 4k7Ω R ] ----
 *   |       DB3 |                                 |
 *   |       DB4 |  ----  D4                       |
 *   |       DB5 |  ----  D5                       |
 *   |       DB6 |  ----  D6                   _________
 *   |       DB7 |  ----  D7                  |    B    |
 *   |         A |  ----  5V                  |   TUN   |
 *   |         K |  ------------------------  | C     E |  ----  GND
 *   |___________|                            |_________|
 * 
 * 
 *    ------------------------ C0  -----  D4
 *    |
 *    |     ------------------ C1  -----  D5
 *    |     |
 *    |     |     ------------ C2  -----  D6
 *    |     |     |
 *    |     |     |     ------ C3  -----  D7
 *    |     |     |     |
 *   [1]---[2]---[3]---[A]---- R0  -----  D9
 *    |     |     |     |
 *   [4]---[5]---[6]---[B]---- R1  -----  D10
 *    |     |     |     |
 *   [7]---[8]---[9]---[C]---- R2  -----  D11
 *    |     |     |     |
 *   [*]---[0]---[#]---[D]---- R3  -----  D12
 *  
 *  
 **********************************************************
 * Created 25 December 2018
 * @Gorontalo, Indonesia
 * by ZulNs
 **********************************************************/

#include <MultitapKeypad.h>
#include <LCD_HD44780.h>

const uint16_t BACKLIGHT_ON_PERIOD = 10000;
const uint16_t FLASHING_PERIOD = 1500;

const uint8_t ROW0 = 9;
const uint8_t ROW1 = 10;
const uint8_t ROW2 = 11;
const uint8_t ROW3 = 12;
const uint8_t COL0 = 4;
const uint8_t COL1 = 5;
const uint8_t COL2 = 6;
const uint8_t COL3 = 7;

const uint8_t LCD_RS = 2;
const uint8_t LCD_RW = 3;
const uint8_t LCD_EN = A2;
const uint8_t LCD_D4 = 4;
const uint8_t LCD_D5 = 5;
const uint8_t LCD_D6 = 6;
const uint8_t LCD_D7 = 7;
const uint8_t BACKLIGHT = A0;
const uint8_t BEEP = A1;

const uint8_t UPPER_A  = 0;
const uint8_t UPPER_B  = 1;
const uint8_t LOWER_A  = 2;
const uint8_t LOWER_B  = 3;
const uint8_t NUMBER_1 = 4;
const uint8_t NUMBER_2 = 5;
const uint8_t UPPER_E  = 6;
const uint8_t UPPER_D  = 7;

const uint8_t CHR_BOUND       = 3;
const uint8_t BACKSPACE       = 8;
const uint8_t INSERT          = 9;
const uint8_t DELETE          = 10;
const uint8_t CLEARSCREEN     = 12;
const uint8_t CARRIAGE_RETURN = 13;
const uint8_t CAPSLOCK_ON     = 17;
const uint8_t CAPSLOCK_OFF    = 18;
const uint8_t NUMLOCK_ON      = 19;
const uint8_t NUMLOCK_OFF     = 20;
const uint8_t EDITMODE_ON     = 21;
const uint8_t EDITMODE_OFF    = 22;
const uint8_t CURSOR_LEFT     = 28;
const uint8_t CURSOR_RIGHT    = 29;
const uint8_t CURSOR_UP       = 30;
const uint8_t CURSOR_DOWN     = 31;

const char SYMBOL[] PROGMEM =
{
  ',', '.', ';', ':', '!',
  '?', '\'', '"','-', '+',
  '*', '/', '=', '%', '^',
  '(', ')', '#', '@', '$',
  '[', ']', '{', '}', CHR_BOUND
};

const char ALPHABET[] PROGMEM =
{
  'A', 'B', 'C', CHR_BOUND, CHR_BOUND,
  'D', 'E', 'F', CHR_BOUND, CHR_BOUND,
  'G', 'H', 'I', CHR_BOUND, CHR_BOUND,
  'J', 'K', 'L', CHR_BOUND, CHR_BOUND,
  'M', 'N', 'O', CHR_BOUND, CHR_BOUND,
  'P', 'Q', 'R', 'S',       CHR_BOUND,
  'T', 'U', 'V', CHR_BOUND, CHR_BOUND,
  'W', 'X', 'Y', 'Z',       CHR_BOUND
};

uint8_t letter_A[8] =
{
  0b11111,
  0b11001,
  0b10110,
  0b10110,
  0b10000,
  0b10110,
  0b10110,
  0b11111
};

uint8_t letter_B[8] =
{
  0b11111,
  0b10001,
  0b10110,
  0b10001,
  0b10110,
  0b10110,
  0b10001,
  0b11111
};

uint8_t letter_a[8] =
{
  0b11111,
  0b11111,
  0b11000,
  0b11110,
  0b11000,
  0b10110,
  0b11000,
  0b11111
};

uint8_t letter_b[8] =
{
  0b11111,
  0b10111,
  0b10111,
  0b10001,
  0b10110,
  0b10110,
  0b10001,
  0b11111
};

uint8_t number_1[8] =
{
  0b11111,
  0b11011,
  0b10011,
  0b11011,
  0b11011,
  0b11011,
  0b10001,
  0b11111
};

uint8_t number_2[8] =
{
  0b11111,
  0b11001,
  0b10110,
  0b11110,
  0b11101,
  0b11011,
  0b10000,
  0b11111
};

uint8_t letter_E[8] =
{
  0b11111,
  0b10000,
  0b10111,
  0b10000,
  0b10111,
  0b10111,
  0b10000,
  0b11111
};

uint8_t letter_D[8] =
{
  0b11111,
  0b10001,
  0b10110,
  0b10110,
  0b10110,
  0b10110,
  0b10001,
  0b11111
};

char modeBuf[3];
uint8_t cursorPos;
uint8_t flashPos;
boolean isAlphaMode = true;
boolean isUpperCaseMode = true;
boolean isEditMode = false;
boolean isFlashing = false;
boolean isBacklightOn = false;
unsigned long backlightStartTime;
unsigned long flashingStartTime;

LCD_HD44780 lcd(LCD_RS, LCD_RW, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
MultitapKeypad kpd( ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2, COL3 );
Key key;

void setup()
{
  kpd.attachFunction( otherProcess );
  pinMode( BACKLIGHT, OUTPUT );
  digitalWrite( BACKLIGHT, HIGH );
  pinMode( LED_BUILTIN, OUTPUT );
  /////////////////////////////
  // pinMode( 8, OUTPUT );   //
  // digitalWrite( 8, LOW ); //
  /////////////////////////////
  lcd.begin( 16, 2 );
  // create a new character
  lcd.createChar( UPPER_A,  letter_A );
  lcd.createChar( UPPER_B,  letter_B );
  lcd.createChar( LOWER_A,  letter_a );
  lcd.createChar( LOWER_B,  letter_b );
  lcd.createChar( NUMBER_1, number_1 );
  lcd.createChar( NUMBER_2, number_2 );
  lcd.createChar( UPPER_E,  letter_E );
  lcd.createChar( UPPER_D,  letter_D );
  lcd.print( F( "Multitap Keypad\nby ZulNs" ) );
  delay( 5000 );
  lcd.print( F( "\nPress [*] key..." ) );
  // wait until [*] key has been pressed
  do
  {
    key = kpd.getKey();
  }
  while ( key.beforeLastCode != NO_KEY || key.lastCode != KEY_ASTERISK || key.code != NO_KEY );
  tone( BEEP, 4000, 50 );
  lcd.clear();
  lcd.cursor();
  lcd.blink();
}

void loop()
{
  char chr;
  key = kpd.getKey();
  switch ( key.state )
  {
    case CANCELED:
      cursorPos = lcd.getCursor();
      lcd.setCursor( flashPos );
      lcd.print( modeBuf );
      lcd.setCursor( cursorPos );
      lcd.gotoCursor();
      lcd.cursor();
      lcd.blink();
      isFlashing = false;
      break;
    case KEY_DOWN:
    case MULTI_TAP:
      tone( BEEP, 5000, 20 );
      digitalWrite( LED_BUILTIN, HIGH );
      switch ( key.code )
      {
        case KEY_ASTERISK:
          lcd.moveCursorLeft();
          break;
        case KEY_NUMBER_SIGN:
          lcd.moveCursorRight();
          break;
        case KEY_0:
          lcd.write( isAlphaMode ? ' ' : '0' );
          break;
        case KEY_1:
          if ( isAlphaMode )
          {
            chr = getSymbol( key.tapCounter );
            if ( key.state == MULTI_TAP )
            {
              lcd.moveCursorLeft();
            }
          }
          else
          {
            chr = '1';
          }
          lcd.write( chr );
          break;
        case KEY_A:
          lcd.backspace();
          break;
        case KEY_B:
          initFlashing();
          if ( isAlphaMode )
          {
            isUpperCaseMode = !isUpperCaseMode;
            if ( isUpperCaseMode )
            {
              lcd.write( UPPER_A );
              lcd.write( UPPER_B );
            }
            else
            {
              lcd.write( LOWER_A );
              lcd.write( LOWER_B );
            }
          }
          else
          {
            isEditMode = !isEditMode;
            if ( isEditMode )
            {
              lcd.write( UPPER_E );
              lcd.write( UPPER_D );
            }
            else
            {
              lcd.write( NUMBER_1 );
              lcd.write( NUMBER_2 );
            }
          }
          lcd.setCursor( cursorPos );
          lcd.gotoCursor();
          break;
        case KEY_C:
          if ( isEditMode )
          {
            lcd.insert();
          }
          else
          {
            lcd.moveCursorUp();
          }
          break;
        case KEY_D:
          if ( isEditMode )
          {
            lcd.del();
          }
          else
          {
            lcd.moveCursorDown();
          }
          break;
        default:
          if ( isAlphaMode )
          {
            chr = getAlphabet( key.character, key.tapCounter );
            if ( !isUpperCaseMode )
            {
              chr += 32;
            }
            if ( key.state == MULTI_TAP )
            {
              lcd.moveCursorLeft();
            }
          }
          else
          {
            chr = key.character;
          }
          lcd.write( chr );
      }
      isBacklightOn = false;
      digitalWrite( BACKLIGHT, HIGH );
      break;
    case LONG_TAP:
      switch ( key.code )
      {
        case KEY_ASTERISK:
          tone( BEEP, 5000, 20 );
          lcd.lineHome();
          break;
        case KEY_NUMBER_SIGN:
          tone( BEEP, 5000, 20 );
          lcd.lineEnd();
          break;
        case KEY_A:
          tone( BEEP, 5000, 20 );
          lcd.clear();
          break;
        case KEY_B:
          tone( BEEP, 5000, 20 );
          initFlashing();
          isAlphaMode = !isAlphaMode;
          isEditMode = false;
          if ( isAlphaMode )
          {
            if ( isUpperCaseMode )
            {
              lcd.write( UPPER_A );
              lcd.write( UPPER_B );
            }
            else
            {
              lcd.write( LOWER_A );
              lcd.write( LOWER_B );
            }
          }
          else
          {
            isUpperCaseMode = !isUpperCaseMode; // do this to avoid change by previous key state
            lcd.write( NUMBER_1 );
            lcd.write( NUMBER_2 );
          }
          lcd.setCursor( cursorPos );
          lcd.gotoCursor();
          break;
        case KEY_C:
          if ( isAlphaMode )
          {
            tone( BEEP, 5000, 20 );
            lcd.moveCursorDown();
            lcd.clearLine();
          }
          else if ( !isEditMode )
          {
            tone( BEEP, 5000, 20 );
            lcd.home();
          }
          break;
        case KEY_D:
          if ( !isEditMode );
          {
            tone( BEEP, 5000, 20 );
            lcd.lineHome(); // Actually this is a newline, since it preceded by a cursor down in the multi tap state.
          }
          break;
        default:
          if ( isAlphaMode )
          {
            tone( BEEP, 5000, 20 );
            lcd.moveCursorLeft();
            lcd.write( key.character );
          }
      }
      break;
    case MULTI_KEY_DOWN:
      tone( BEEP, 4000, 100 );
      digitalWrite( LED_BUILTIN, LOW );
      break;
    case KEY_UP:
      digitalWrite( LED_BUILTIN, LOW );
      backlightStartTime = millis();
      isBacklightOn = true;
  }
}

void otherProcess( void ) {
  if ( isBacklightOn && millis() - backlightStartTime >= BACKLIGHT_ON_PERIOD )
  {
    digitalWrite( BACKLIGHT, LOW );
    isBacklightOn = false;
  }
  if ( isFlashing && millis() - flashingStartTime >= FLASHING_PERIOD )
  {
    kpd.isCanceled = true;
  }
}

void initFlashing()
{
  cursorPos = lcd.getCursor();
  if ( isFlashing )
  {
    lcd.setCursor( flashPos );
    lcd.print( modeBuf );
  }
  else
  {
    lcd.noCursor();
    lcd.noBlink();
  }
  flashPos = ( cursorPos % 16 < 8 ) ? 14 : 0;
  if ( cursorPos / 16 % 2 == 0 )
  {
    flashPos += 16;
  }
  flashPos += cursorPos / 32 * 32;
  lcd.readString( modeBuf, 2, flashPos);
  lcd.setCursor( flashPos );
  isFlashing = true;
  flashingStartTime = millis();
}

uint8_t getSymbol( uint8_t ctr )
{
  uint8_t sym = pgm_read_byte_near( SYMBOL + ctr );
  if ( sym == CHR_BOUND )
  {
    sym = pgm_read_byte_near( SYMBOL );
    kpd.resetTapCounter();
  }
  return sym;
}

uint8_t getAlphabet( uint8_t chr, uint8_t ctr )
{
  chr = ( chr - '2' ) * 5;
  uint8_t alpha = pgm_read_byte_near( ALPHABET + chr + ctr );
  if ( alpha == CHR_BOUND )
  {
    alpha = pgm_read_byte_near( ALPHABET + chr );
    kpd.resetTapCounter();
  }
  return alpha;
}
