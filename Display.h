/*
  Display.h - Visual display.
  Created by Mitja Bezensek, Jun 26, 2017
*/

#ifndef Display_h
#define Display_h

class Display {
  public:
    void print(const char *message);
    void println(const char *message);
  private:
    void setup();
};

#endif
