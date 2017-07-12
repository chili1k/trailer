ARDUINO_DIR = /Applications/Arduino.app/Contents/Java
ARDMK_DIR =/usr/local/opt/arduino-mk
AVR_TOOLS_DIR = $(ARDUINO_DIR)/hardware/tools/avr
#MONITOR_PORT = /dev/cu.wchusbserial1410
ARDUINO_LIBS += MPU6050 I2Cdev Wire

include /usr/local/opt/arduino-mk/Arduino.mk
