## About

Trailer balancer written for the Arduino platform.

## Dev env setup

Install MPU, I2C and Debounce2 library using Arudino IDE:

* https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050
* https://github.com/thomasfredericks/Bounce2

Uses Arduino-Makefile project:
https://github.com/sudar/Arduino-Makefile

To setup vim:
https://collectiveidea.com/blog/archives/2017/04/05/arduino-programming-in-vim

## Using

```
make
make upload
make monitor
```

To exit the monitor use C k. Monitor is actually screen.
