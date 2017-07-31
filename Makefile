ARDUINO_DIR = /Applications/Arduino.app/Contents/Java
ARDMK_DIR =/usr/local/opt/arduino-mk
AVR_TOOLS_DIR = $(ARDUINO_DIR)/hardware/tools/avr
BOARD_TAG    = mega
BOARD_SUB    = atmega2560
MONITOR_PORT = /dev/cu.usbmodemFA131
ARDUINO_LIBS += MPU6050 I2Cdev Wire

include /usr/local/opt/arduino-mk/Arduino.mk
