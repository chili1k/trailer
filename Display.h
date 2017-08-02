#ifndef Display_h
#define Display_h

class Display {
  public:
    void print(const char *message);
    void print(float number);
    void println(const char *message);
    void newline();
    void setup();
  private:
};

#endif
