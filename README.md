# LCD_HD44780
Arduino library for HD44780 based or compatible LCDs.

##Features:
  - Supports read a character or a string from the screen.
  - Supports editing some text right on the screen (insert and delete a character, backspace, clear line, and new line).
  - Supports cursor movement in all directions (left, right, up, and down, line home, line end, screen home).
  - Fixing weird order of the screen rows.
  - Shareable all used I/O lines except for ENABLE pin one.
  - Accepts println() command or method.
  - Automatically scrolls the display if current page has been changed. Eg. in LCD 1602, it consist of two pages and four lines, if third lines have been reached, the display will automatically scroll to second page then.
