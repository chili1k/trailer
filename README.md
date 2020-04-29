## About

Trailer balancer written for the Arduino platform.

## Dev env setup

Install MPU, I2C and Debounce2 library using Arudino IDE:

* https://github.com/chili1k/i2cdevlib/tree/master/Arduino/MPU6050 (for of Jeff Rowbergs lib)
* https://github.com/thomasfredericks/Bounce2

Uses Arduino-Makefile project:
https://github.com/sudar/Arduino-Makefile

To setup vim:
https://collectiveidea.com/blog/archives/2017/04/05/arduino-programming-in-vim

## I2C lib issues

If FIFO keeps resetting try configuring the `MPU6050_DMP_FIFO_RATE_DIVISOR` in Gyro.cpp.

To stop I2C library from hanging, patch Wire lib with contents from i2c_patch subdirectory. Directories:
 * Mac: /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/libraries/Wire/
 * Windows: Somewhere in c:\users\YourUserName\AppData\Local\Arduino

 Original commit: https://github.com/wmarkow/Arduino/commits/b325055fda2d3b22c48ed6a316a1efa6f2f10a08

Discussions:
 * https://github.com/jrowberg/i2cdevlib/issues/252
 * https://github.com/arduino/Arduino/issues/1476


## Using

```
make
make upload
make monitor
```

To exit the monitor use C k. To pause monitor press C ESC.  Monitor is actually screen.

## Calibration
**Reset arduino before doing calibration!**

Use sketch in calibration subfolder.

Instructions say that when doing calibration letters should be facing up. However for our use case this doesn't seem to matter. Still the prefered way is that during calibration, letters have the same orientation as during normal operation.


## Pitch and roll orientation
```
Calibration: gyro facing up
A-------------------------------------C
|                                     |
|            A-------C -              |
|            |       |                |
|            |       | pitch          |
|            |       |                |--->
|            |xy     |                |
|            B-------D +              |
|         gyro facing up              |
|            -  roll   +              |
|                                     |
B-------------------------------------D

Calibration: gyro facing down (this is production)
A-------------------------------------C
|                                     |
|                                     |
|            A-------------C +        |
|            |             |          |
|            |             | roll     |--->
|            |xy           |          |
|            B-------------D -        |
|            gyro facing down         |
|            +    pitch     -         |
|                                     |
B-------------------------------------D
```

Example for how pitch, roll signs are treated:
```
1) pitch=+0.3
       \
        \
         \
pitch: +  -
2) pitch=-0.3

         /
        /
       /
pitch: +  -
```
