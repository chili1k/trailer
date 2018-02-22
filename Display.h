#ifndef Display_h
#define Display_h

class Display {
  public:
    static void print(const char *message);
    static void print(float number);
    static void println(const char *message);
    static void newline();
    static void setup();
  private:
};

#endif
