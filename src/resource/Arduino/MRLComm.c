/**
*
* @author GroG (at) myrobotlab.org
*
* This file is part of MyRobotLab.
*
* Enjoy !
*
* MRLComm.ino
* -----------------
* Refactoring has made MRLComm.ino far more general
* there are only 2 "types" of things - controllers and pins - or writers and readers
* each now will have sub-types
*
* Controllers
* -----------------
* digital pins, pwm, pwm/dir dc motors, pwm/pwm dc motors
*
* Sensors
* -----------------
* digital polling pins, analog polling pins, range pins, oscope, trigger events
*
* Combination
* -----------------
* pingdar, non-blocking pulsin
*
*
* Requirements: MyRobotLab running on a computer & a serial connection
*
*/

// FIXME FIXME FIXME - ALL defines are defined ONLY in Java - all other files need to be
// generated (INO, PYTHON, ETC) 1 - control definition to rule them all !
// TODO - getBoardInfo() - returns board info !
// TODO - getPinInfo() - returns pin info !

#include <Servo.h>
// Start of Adafruit16CServoDriver I2C import
#define WIRE Wire
#include <Wire.h>
// Start of Adafruit16CServoDriver I2C import
// version to match with MRL
#define MRLCOMM_VERSION         34

// serial protocol functions
#define MAGIC_NUMBER            170 // 10101010

// ----- MRLCOMM FUNCTION GENERATED INTERFACE BEGIN -----------
///// INO GENERATED DEFINITION BEGIN //////
// {publishMRLCommError Integer} 
#define PUBLISH_MRLCOMM_ERROR    1
// {getVersion} 
#define GET_VERSION   2
// {publishVersion Integer} 
#define PUBLISH_VERSION   3
// {analogReadPollingStart Integer} 
#define ANALOG_READ_POLLING_START   4
// {analogReadPollingStop Integer} 
#define ANALOG_READ_POLLING_STOP    5
// {analogWrite Integer Integer} 
#define ANALOG_WRITE    6
// {digitalReadPollingStart Integer} 
#define DIGITAL_READ_POLLING_START    7
// {digitalReadPollingStop Integer} 
#define DIGITAL_READ_POLLING_STOP   8
// {digitalWrite Integer Integer} 
#define DIGITAL_WRITE   9
// {motorAttach Motor} 
#define MOTOR_ATTACH    10
// {motorDetach Motor} 
#define MOTOR_DETACH    11
// {motorMove Motor} 
#define MOTOR_MOVE    12
// {motorMoveTo Motor} 
#define MOTOR_MOVE_TO   13
// {motorReset Motor} 
#define MOTOR_RESET   14
// {motorStop Motor} 
#define MOTOR_STOP    15
// {pinMode Integer Integer} 
#define PIN_MODE    16
// {publishCustomMsg Object[]} 
#define PUBLISH_CUSTOM_MSG    17
// {publishLoadTimingEvent Long} 
#define PUBLISH_LOAD_TIMING_EVENT   18
// {publishPin Pin} 
#define PUBLISH_PIN   19
// {publishPulse Long} 
#define PUBLISH_PULSE   20
// {publishPulseStop Integer} 
#define PUBLISH_PULSE_STOP    21
// {publishSensorData Object} 
#define PUBLISH_SENSOR_DATA   22
// {publishServoEvent Integer} 
#define PUBLISH_SERVO_EVENT   23
// {publishTrigger Pin} 
#define PUBLISH_TRIGGER   24
// {pulse int int int int} 
#define PULSE   25
// {pulseStop} 
#define PULSE_STOP    26
// {sensorAttach SensorDataSink} 
#define SENSOR_ATTACH   27
// {sensorPollingStart String int} 
#define SENSOR_POLLING_START    28
// {sensorPollingStop String} 
#define SENSOR_POLLING_STOP   29
// {servoAttach Servo Integer} 
#define SERVO_ATTACH    30
// {servoDetach Servo} 
#define SERVO_DETACH    31
// {servoEventsEnabled Servo} 
#define SERVO_EVENTS_ENABLED    32
// {servoSweepStart Servo} 
#define SERVO_SWEEP_START   33
// {servoSweepStop Servo} 
#define SERVO_SWEEP_STOP    34
// {servoWrite Servo} 
#define SERVO_WRITE   35
// {servoWriteMicroseconds Servo} 
#define SERVO_WRITE_MICROSECONDS    36
// {setDebounce int} 
#define SET_DEBOUNCE    37
// {setDigitalTriggerOnly Boolean} 
#define SET_DIGITAL_TRIGGER_ONLY    38
// {setLoadTimingEnabled boolean} 
#define SET_LOAD_TIMING_ENABLED   39
// {setPWMFrequency Integer Integer} 
#define SET_PWMFREQUENCY    40
// {setSampleRate int} 
#define SET_SAMPLE_RATE   41
// {setSerialRate int} 
#define SET_SERIAL_RATE   42
// {setServoSpeed Servo} 
#define SET_SERVO_SPEED   43
// {setTrigger int int int} 
#define SET_TRIGGER   44
// {softReset} 
#define SOFT_RESET    45
///// INO GENERATED DEFINITION END //////

// ----- MRLCOMM FUNCTION GENERATED INTERFACE END -----------
// Start of Adafruit16CServoDriver defines
#define AF_BEGIN 50
#define AF_SET_PWM_FREQ 51
#define AF_SET_PWM 52
#define AF_SET_SERVO 53

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE
#define LED0_ON_L 0x6
// End of Adafruit16CServoDriver defines

// TODO - UNIONS !!!! - all unions begin with type

// ------ non generated types begin ------
// FIXME - all freeform types need to be in Java ! - all should be part
// of the generator !!!

// ------ stepper types ------
#define STEPPER_TYPE_SIMPLE       1

// ------ stepper event types ------
#define STEPPER_EVENT_STOP        1
#define STEPPER_EVENT_STEP        2

// ------ stepper event types ------
#define  SERVO_EVENT_STOPPED      1
#define  SERVO_EVENT_POSITION_UPDATE  2

// ------ error types ------
#define ERROR_SERIAL          1
#define ERROR_UNKOWN_CMD        2
#define ERROR_ALREADY_EXISTS      3
#define ERROR_DOES_NOT_EXIST      4
#define ERROR_UNKOWN_SENSOR     5


// ------ sensor types ------
// refer to - org.myrobotlab.service.interfaces.SensorDataSink
#define SENSOR_TYPE_PIN            0
#define SENSOR_TYPE_ULTRASONIC       1
#define SENSOR_TYPE_PULSE          2

#define CUSTOM_MSG            50

// need a method to identify type of board
// http://forum.arduino.cc/index.php?topic=100557.0

#define COMMUNICATION_RESET    252
#define PUBLISH_MESSAGE_ACK    127
#define PUBLISH_DEBUG          126
#define NOP            255

// ----------  MRLCOMM FUNCTION INTERFACE END -----------

// MAX definitions
// MAX_SERVOS defined by boardtype/library
// TODO - BOARD IDENTIFICATION - PIN IDENTIFICATION
// #define NUM_DIGITAL_PINS            20
// #define NUM_ANALOG_INPUTS           6

#define SENSORS_MAX  NUM_DIGITAL_PINS // this is max number of pins (analog included)
// #define SENSORS_MAX  20 // TODO: Setting to value larger than 32 causes TX/RX errors in MRL. (Make sensor loop faster to fix.)
#define DIGITAL_PIN_COUNT

// ECHO FINITE STATE MACHINE
#define ECHO_STATE_START 1
#define ECHO_STATE_TRIG_PULSE_BEGIN 2
#define ECHO_STATE_TRIG_PULSE_END 3
#define ECHO_STATE_MIN_PAUSE_PRE_LISTENING 4
#define ECHO_STATE_LISTENING 5
#define ECHO_STATE_GOOD_RANGE 6
#define ECHO_STATE_TIMEOUT  7

#define SENSOR_TYPE_ANALOG_PIN_READER 0
#define SENSOR_TYPE_DIGITAL_PIN_READER 3

// FIXME FIXME FIXME
// -- FIXME - modified by board type BEGIN --
// Need Arduino to do a hardware abstraction layer
// https://code.google.com/p/arduino/issues/detail?id=59
// AHAAA !! - many defintions in - pins_arduino.h !!!
// Need a "board" identifier at least !!!

// #define MAX_SERVOS 48 - is defined @ compile time !!

#define ARDUINO_TYPE_INT 16; // :) type identifier - not size - but what the hell ;)

/*
* TODO - CRC for last byte
* getCommand - retrieves a command message
* inbound and outbound messages are the same format, the following represents a basic message
* format
*
* MAGIC_NUMBER|NUM_BYTES|FUNCTION|DATA0|DATA1|....|DATA(N)
*              NUM_BYTES - is the number of bytes after NUM_BYTES to the end
*
*/

int msgSize = 0; // the NUM_BYTES of current message

unsigned int debounceDelay = 50; // in ms
byte msgBuf[64];

// FIXME - make union of struct pins
// all start with general / common section - with "type" :)
typedef struct
{
  // general
  int sensorIndex; // the all important index of the sensor - equivalent to the "name" - used in callbacks
  // FIXME - THIS NEEDS TO BE NORMALIZED IN CODE GENERATOR / BINDER
  int sensorType; // SENSOR_TYPE_DIGITAL_PIN_READER |  SENSOR_TYPE_ANALOG_PIN_READER | SENSOR_TYPE_DIGITAL_PIN | SENSOR_TYPE_PULSE | SENSOR_TYPE_ULTRASONIC
  int address; // pin #
  int state; // state - single at the moment to handle all the finite states of the sensor
  int value;

  // FYI - creating unions "might" make things a little more readable
  // MAKE NOTE !! - Pins will need to "reference" one another
  // so that when they are processed the "lead" pin in say a stepper
  // will "activate" the next pin in sequence so that the correct sequence will be 
  // pulsed

  // int mode; // input or output - not needed - Arduino service should handle it
  bool isActive;
  int rateModulus; // sample rate or feedback control with modulus
  int debounce; // long lastDebounceTime - minDebounceTime
  int rate;
  unsigned long count;
  unsigned long target;

  // next pin in a multi-pin process - e.g. 
  // UltrasonicSensor - trigger pin's nextPin would be the echo pin
  int nextPin; 

  // srf05
  int trigPin;
  int echoPin;
  int timeoutUS;
  unsigned long ts;
  unsigned long lastValue;

}  pin_type;

pin_type pins[SENSORS_MAX];

// Servos
typedef struct
{
  Servo* servo;
  int index; // index of this servo
  int speed;
  int targetPos;
  int currentPos;
  bool isMoving;

  int step; // affects speed usually 1

  // sweep related
  int min;
  int max;
  // int delay; - related to speed
  int increment;
  bool isSweeping;

  // event related
  bool eventsEnabled;
} servo_type;


servo_type servos[MAX_SERVOS];

// FIXME - these are const defines
int PIN_TYPE_DIGITAL = 1;
int PIN_TYPE_ANALOG = 2;
int PIN_TYPE_PWM = 4;


unsigned long loopCount = 0;
unsigned long lastMicros = 0;
int byteCount = 0;
unsigned char newByte = 0;
unsigned char ioCmd[64];  // message buffer for all inbound messages
int readValue;

// FIXME - normalize with sampleRate ..
int loadTimingModulus = 1000;

boolean loadTimingEnabled = false;
unsigned long loadTime = 0;
// TODO - avg load time

unsigned int sampleRate = 1; // 1 - 65,535 modulus of the loopcount - allowing you to sample less

//===custom msg interface begin===
byte customParams[256];
int paramBuffIndex;
int paramCnt;
//===custom msg interface end===

// define any functions that pass structs into them.
void sendServoEvent(servo_type& s, int eventType);
unsigned long getUltrasonicRange(pin_type& pin);
void handleUltrasonicPing(pin_type& pin, unsigned long ts);
// void sendMsg ( int num, ... );

//---- data record definitions end -----
// Define the reset function at address 0
// allow us to reset the board if the serial port disconnects. (Thanks Mats! TODO: test me!)
// void(* resetFunc) (void) = 0;


// ----------- send custom msg begin ---------------------
//void append(const int& data) {
//  ++paramCnt;
//  customParams[paramBuffIndex] = ARDUINO_TYPE_INT;
//  customParams[++paramBuffIndex] = (byte)(data >> 8);
//  customParams[++paramBuffIndex] = ((byte)data & 0xff);
//  ++paramBuffIndex;
//}

boolean debug = false;

void setup() {
  // TODO: higher port speeds?
  //Serial.begin(57600);        // connect to the serial port
  Serial.begin(115200);        // connect to the serial port
  while (!Serial){};
  // TODO: do this before we start the serial port?
  softReset();
  // publish version on startup so it's immediately available for mrl.
  getVersion();
}

// This is the main loop that the arduino runs.
void loop() {

  if (debug)
    publishDebug("Main");
  // make sure we're still connected to the serial port.
  //if (!Serial) {
    // if we're disconnected, shutdown / reset
//    resetFunc();
//  }

  // increment how many times we've run
  ++loopCount;
  // get a command and process it from the serial port (if available.)
  if (getCommand()) {
    if (debug)
      publishDebug("GotCMD");
    processCommand();
    // ack that we got a command (should we ack it first? or after we process the command?)
    sendCommandAck();
  }
  if (debug)
    publishDebug("ProcCMD");
  // update servo positions
  updateServos();
  if (debug)
    publishDebug("UpdateServos");

  // update analog sensor data stuffs
  updateSensors();
  if (debug)
    publishDebug("UpdateSensors");

  // update and report timing metrics
  updateStats();
  if (debug)
    publishDebug("UpdatedStat");
  // Serial.flush();
} // end of big loop

void softReset() {
  for (int i = 0; i < MAX_SERVOS - 1; ++i) {
    servo_type& s = servos[i];
    s.speed = 100;
    if (s.servo != 0) {
      s.servo->detach();
    }
  }
  for (int i = 0; i < SENSORS_MAX - 1; ++i) {
    resetPin(i);
  }
  loopCount = 0;
}

void resetPin(int pinIndex) {
  pin_type& pin = pins[pinIndex];
  pin.isActive = false;
  pin.address = pinIndex; // pin #
  pin.sensorIndex = 0; // all pins initially belong to Arduino service
  pin.rateModulus = 1; // full feedback/sample rate
  pin.count = 0;
  pin.target = 0;
  pin.nextPin = -1;
}

unsigned long toUnsignedLong(unsigned char* buffer, int start) {
  return ((buffer[start + 3] << 24) + (buffer[start + 2] << 16) + (buffer[start + 1] << 8) + buffer[start]);
}

/**
* checks the existence of the searched value in the array
* - good for not adding to a dynamic list of values if it
* already exists
*/
bool exists(int array[], int len, int searchValue) {
  for (int i = 0; i < len; ++i) {
    if (searchValue == array[i]) {
      return true;
    }
  }
  return false;
}

/**
* adds new value to a pseudo dynamic array/list
* if successful - if value already exists on list
* sends back an error
*/
bool addNewValue(int array[], int& len, int addValue) {
  if (!exists(array, len, addValue)) {
    array[len] = addValue;
    ++len;
    return true;
  } else {
    sendError(ERROR_ALREADY_EXISTS);
    return false;
  }
}

// Will Be Depricated
bool removeAndShift(int array[], int& len, int removeValue) {
  if (!exists(array, len, removeValue)) {
    sendError(ERROR_DOES_NOT_EXIST);
    return false;
  }
  int pos = -1;
  if (len == 0) {
    // "should" never happen
    // would be calling remove on an empty list
    // the error ERROR_DOES_NOT_EXIST - "should" be called
    return true;
  }
  // find position of value
  for (int i = 0; i < len; ++i) {
    if (removeValue == array[i]) {
      pos = i;
      break;
    }
  }
  // if at the end just decrement size
  if (pos == len - 1) {
    --len;
    return true;
  }

  // if found somewhere else shift left
  if (pos < len && pos > -1) {
    for (int j = pos; j < len - 1; ++j) {
      array[j] = array[j + 1];
    }
    --len;
  }
  return true;
}

boolean getCommand() {
  // handle serial data begin
  int bytesAvailable = Serial.available();
  if (bytesAvailable > 0) {
    // now we should loop over the available bytes .. not just read one by one.
    for (int i = 0 ; i < bytesAvailable; i++) {
      // read the incoming byte:
      newByte = Serial.read();
      if (debug)
        publishDebug("BYTE");
      ++byteCount;
      // checking first byte - beginning of message?
      if (byteCount == 1 && newByte != MAGIC_NUMBER) {
        sendError(ERROR_SERIAL);
        // reset - try again
        byteCount = 0;
        // return false;
      }
      if (byteCount == 2) {
        // get the size of message
        // todo check msg < 64 (MAX_MSG_SIZE)
        if (newByte > 64){
          // TODO - send error back
          byteCount = 0;
          continue; // GroG - I guess  we continue now vs return false on error conditions? 
        }
        msgSize = newByte;
      }
      if (byteCount > 2) {
        // fill in msg data - (2) headbytes -1 (offset)
        ioCmd[byteCount - 3] = newByte;
      }
      // if received header + msg
      if (byteCount == 2 + msgSize) {
        // we've reach the end of the command, just return true .. we've got it
        return true;
      }
    }
  } // if Serial.available
  // we only partially read a command.  (or nothing at all.)
  return false;
}

// This function will switch the current command and call
// the associated function with the command
void processCommand() {
  switch (ioCmd[0]) {
  // === system pass through begin ===
  case DIGITAL_WRITE:
    digitalWrite(ioCmd[1], ioCmd[2]);
    break;
  case ANALOG_WRITE:
    if (debug)
      publishDebug("AW");
    //analogWrite(ioCmd[1], ioCmd[2]);
    break;
  case PIN_MODE:
    if (debug)
      publishDebug("PM");
    pinMode(ioCmd[1], ioCmd[2]);
    break;
  case SERVO_ATTACH:
    servoAttach();
    break;
  case SERVO_SWEEP_START:
    servoStartSweep();
    break;
  case SERVO_SWEEP_STOP:
    servoStopSweep();
    break;
  case SERVO_EVENTS_ENABLED:
    servoEventsEnabled();
    break;
  case SERVO_WRITE:
    servoWrite();
    break;
  case PUBLISH_SERVO_EVENT:
    publishServoEvent();
    break;
  case SERVO_WRITE_MICROSECONDS:
    servoWriteMicroseconds();
    break;
  case SET_SERVO_SPEED:
    setServoSpeed();
    break;
  case SERVO_DETACH:
    servoDetach();
    break;
  case SET_LOAD_TIMING_ENABLED:
    setLoadTimingEnabled();
    break;
  case SET_PWMFREQUENCY:
    setPWMFrequency(ioCmd[1], ioCmd[2]);
    break;
  case ANALOG_READ_POLLING_START:
    analogReadPollingStart();
    break;
  case ANALOG_READ_POLLING_STOP:
    analogReadPollingStop();
    break;
  case DIGITAL_READ_POLLING_START:
    digitalReadPollingStart();
    break;
  case PULSE:
    pulse();
    break;
  case PULSE_STOP:
    pulseStop();
    break;
  case DIGITAL_READ_POLLING_STOP:
    digitalReadPollingStop();
  case SET_TRIGGER:
    setTrigger();
    break;
  case SET_DEBOUNCE:
    setDebounce();
    break;
  case SET_DIGITAL_TRIGGER_ONLY:
    setDigitalTriggerOnly();
    break;
  case SET_SERIAL_RATE:
    setSerialRate();
    break;
  case GET_VERSION:
    getVersion();
    break;
  case SET_SAMPLE_RATE:
    setSampleRate();
    break;
  case SOFT_RESET:
    softReset();
    break;
  case SENSOR_ATTACH:
    sensorAttach();
    break;
  case SENSOR_POLLING_START:
    sensorPollingStart();
    break;
  case SENSOR_POLLING_STOP:
    sensorPollingStop();
    break;
  // Start of Adafruit16CServoDriver commands
  case AF_BEGIN:
    afBegin();
    break;
  case AF_SET_PWM_FREQ:
    afSetPWMFREQ();
    break;
  case AF_SET_PWM:
    afSetPWM();
    break;
  case AF_SET_SERVO:
    afSetServo();
    break;
  case NOP:
    // No Operation
    break;
  default:
    sendError(ERROR_UNKOWN_CMD);
    break;
  } // end switch
  // reset command buffer to be ready to receive the next command.
  memset(ioCmd, 0, sizeof(ioCmd));
  byteCount = 0;
} // process Command

// This function handles updating the servo angles (mostly for sweeping?)
void updateServos() {
  // update moving servos - send events if required
  for (int i = 0; i < MAX_SERVOS; ++i) {
    servo_type& s = servos[i];
    if (s.isMoving && s.servo != 0) {
      if (s.currentPos != s.targetPos) {
        // caclulate the appropriate modulus to drive
        // the servo to the next position
        // TODO - check for speed > 0 && speed < 100 - send ERROR back?
        int speedModulus = (100 - s.speed) * 10;
        if (loopCount % speedModulus == 0) {
          int increment = s.step * ((s.currentPos < s.targetPos) ? 1 : -1);
          // move the servo an increment
          s.currentPos = s.currentPos + increment;
          s.servo->write(s.currentPos);
          if (s.eventsEnabled) sendServoEvent(s, SERVO_EVENT_POSITION_UPDATE);
        }
      } else {
        if (s.isSweeping) {
          if (s.targetPos == s.min) {
            s.targetPos = s.max;
          } else {
            s.targetPos = s.min;
          }
        } else {
          if (s.eventsEnabled)
            sendServoEvent(s, SERVO_EVENT_STOPPED);
          s.isMoving = false;
        }
      }
    }
  }
}

// This function updates the sensor data (both analog and digital reading here.)
void updateSensors() {
  unsigned long ts;

  for (int i = 0; i < SENSORS_MAX; ++i) {
    pin_type& pin = pins[i];
    if (pin.isActive != true) {
      // only process the pin if it's active.
      //publishDebug("PNA");
      continue;
    }
    // publishDebug("USL");
    // continue;

    switch (pin.sensorType) {
      case SENSOR_TYPE_ANALOG_PIN_READER:
        if (debug)
          publishDebug("AREAD");
        pin.value = analogRead(pin.address);
        publishSensorData(pin.sensorIndex, pin.address, pin.value);
        break;
      case SENSOR_TYPE_DIGITAL_PIN_READER:
        if (debug)
          publishDebug("DREAD");
        // read the pin
        pin.value = digitalRead(pin.address);
        publishSensorData(pin.sensorIndex, pin.address, pin.value);
        break;
        // if my value is different from last time - send it
        // if (pin.lastValue != pin.value || !pin.s) //TODO - SEND_DELTA_MIN_DIFF
        //if (pin.lastValue != pin.value || (loopCount%pin.rateModulus) == 0) {
          //sendMsg(4, ANALOG_VALUE, analogReadPin[i], readValue >> 8, readValue & 0xff);
          //publishSensorData(pin.sensorIndex, pin.address, pin.value);
        //}
        // set the last input value of this pin  (This is a type cast error?! why cast to unsigned long here?)
        //pin.lastValue = pin.value;
        // publishDebug("PUBDONE");

      case SENSOR_TYPE_ULTRASONIC:
        if (debug)
          publishDebug("USTYPE");
        // FIXME - handle in own function - the overhead is worth not having
        // 200+ lines of code inlined here !
        // we are running & have an ultrasonic (ping) pin
        // check to see what state we  are in
        handleUltrasonicPing(pin, ts);
        break;
      // because pin pulse & pulsing are so closely linked
      // the pulse will be handled here as well even if the
      // read data sent back on serial is disabled
      case SENSOR_TYPE_PULSE:
        if (debug)
          publishDebug("PULSETYPE");
        // TODO - implement - rate = modulo speed
        // if (loopCount%rate == 0) {
        // toggle pin state
        pin.lastValue = (pin.lastValue == 0) ? 1 : 0;
        // leading edge ... 0 to 1
        if (pin.lastValue == 1) {
          pin.count++;
          if (pin.count >= pin.target) {
            pin.state = PUBLISH_PULSE_STOP;
          }
        }
        // change state of pin
        digitalWrite(pin.address, pin.lastValue);
        // move counter/current position
        // see if feedback rate is valid
        // if time to send feedback do it
        // if (loopCount%feedbackRate == 0)
        // 0--to-->1 counting leading edge only
        // pin.method == PUBLISH_PULSE_PIN &&
        // stopped on the leading edge
        if (pin.state != PUBLISH_PULSE_STOP && pin.lastValue == 1) {
          publishPulseStop(pin.state, pin.sensorIndex, pin.address, pin.count);
          // deactivate
          // lastDebounceTime[digitalReadPin[i]] = millis();
        }
        if (pin.state == PUBLISH_PULSE_STOP) {
          pin.isActive = false;
        }
        // publish the pulse!
        publishPulse(pin.state, pin.sensorIndex, pin.address, pin.count);
        break;
      default:
        if (debug)
          publishDebug("UNKNTYPE");
        sendError(ERROR_UNKOWN_SENSOR);
        break;
    }
  } // end for each pin
}

// This function updates how long it took to run this loop
// and reports it back to the serial port if desired.
void updateStats() {
  // FIXME - fix overflow with getDiff() method !!!
  unsigned long now = micros();
  loadTime = now - lastMicros; // avg outside
  lastMicros = now;
  // report load time
  if (loadTimingEnabled && (loopCount%loadTimingModulus == 0)) {
    // send it
    publishLoadTimingEvent(loadTime);
  }
}

unsigned long getUltrasonicRange(pin_type& pin) {
  // added for pins which have single pin !
  pinMode(pin.trigPin, OUTPUT);
  digitalWrite(pin.trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin.trigPin, LOW);
  // added for pins which have single pin !
  pinMode(pin.echoPin, INPUT);
  // CHECKING return pulseIn(pin.echoPin, HIGH, pin.timeoutUS);
  // TODO - adaptive timeout ? - start big - pull in until valid value - push out if range is coming close
  return pulseIn(pin.echoPin, HIGH);
}

// Start of Adafruit16CServoDriver methods
// I2C write 
void write8(uint8_t i2caddr, uint8_t addr, uint8_t d) {
  WIRE.beginTransmission(i2caddr);
  WIRE.write(addr);
  WIRE.write(d);
  WIRE.endTransmission();
}

// I2C Read
uint8_t read8(uint8_t i2caddr, uint8_t addr) {
  WIRE.beginTransmission(i2caddr);
  WIRE.write(addr);
  WIRE.endTransmission();
  WIRE.requestFrom((uint8_t)i2caddr, (uint8_t)1);
  return WIRE.read();
}

void setPWM(uint8_t i2caddr, uint8_t num, uint16_t on, uint16_t off) {
  WIRE.beginTransmission(i2caddr);
  WIRE.write(LED0_ON_L+4*num);
  WIRE.write(on);
  WIRE.write(on>>8);
  WIRE.write(off);
  WIRE.write(off>>8);
  WIRE.endTransmission();
}
// End of Adafruit16CServoDriver methods

// MRL Command helper methods below:
// GET_VERSION
void getVersion() {
  // call publish version to talk to the serial port.
  publishVersion();
}

// SERVO_ATTACH
void servoAttach() {
  servo_type& s = servos[ioCmd[1]];
  s.index = ioCmd[1];
  if (s.servo == NULL) {
    s.servo = new Servo();
  }
  s.servo->attach(ioCmd[2]);
  s.step = 1;
  s.eventsEnabled = false;
}

// SERVO_START_SWEEP
void servoStartSweep() {
  servo_type& s = servos[ioCmd[1]];
  s.min = ioCmd[2];
  s.max = ioCmd[3];
  s.step = ioCmd[4];
  s.isMoving = true;
  s.isSweeping = true;
}

// SERVO_STOP_SWEEP
void servoStopSweep() {
  servo_type& s = servos[ioCmd[1]];
  s.isMoving = false;
  s.isSweeping = false;
}

// SERVO_EVENTS_ENABLED
void servoEventsEnabled() {
  // Not implemented.
}

// SERVO_WRITE
void servoWrite() {
  servo_type& s = servos[ioCmd[1]];
  if (s.speed == 100 && s.servo != 0) {
    // move at regular/full 100% speed
    s.targetPos = ioCmd[2];
    s.currentPos = ioCmd[2];
    s.isMoving = false;
    s.servo->write(ioCmd[2]);
    if (s.eventsEnabled) sendServoEvent(s, SERVO_EVENT_STOPPED);
  } else if (s.speed < 100 && s.speed > 0) {
    s.targetPos = ioCmd[2];
    s.isMoving = true;
  }
}

// PUBLISH_SERVO_EVENT
void publishServoEvent() {
  servo_type& s = servos[ioCmd[1]];
  s.eventsEnabled = ioCmd[2];
}

// SERVO_WRITE_MICROSECONDS
void servoWriteMicroseconds() {
  // TODO - incorporate into speed control etc
  // normalize - currently by itself doesn't effect events
  // nor is it involved in speed control
  servo_type& s = servos[ioCmd[1]];
  if (s.servo != 0) {
    // 1500 midpoint
    s.servo->writeMicroseconds(ioCmd[2]);
  }
}

// SET_SERVO_SPEED
void setServoSpeed() {
  // setting the speed of a servo
  servo_type& servo = servos[ioCmd[1]];
  servo.speed = ioCmd[2];
}

// SERVO_DETACH
void servoDetach() {
  servo_type& s = servos[ioCmd[1]];
  if (s.servo != 0) {
    s.servo->detach();
  }
}

// SET_LOAD_TIMING_ENABLED
void setLoadTimingEnabled() {
  loadTimingEnabled = ioCmd[1];
  //loadTimingModulus = ioCmd[2];
  loadTimingModulus = 1;
}

// SET_PWMFREQUENCY
void setPWMFrequency(int address, int prescalar) {
  // FIXME - different boards have different timers
  // sets frequency of pwm of analog
  // FIXME - us ifdef appropriate uC which
  // support these clocks TCCR0B
  int clearBits = 0x07;
  if (address == 0x25) {
    TCCR0B &= ~clearBits;
    TCCR0B |= prescalar;
  } else if (address == 0x2E) {
    TCCR1B &= ~clearBits;
    TCCR1B |= prescalar;
  } else if (address == 0xA1) {
    TCCR2B &= ~clearBits;
    TCCR2B |= prescalar;
  }
}

// ANALOG_READ_POLLING_START
void analogReadPollingStart() {
  int pinIndex = ioCmd[1]; // + DIGITAL_PIN_COUNT / DIGITAL_PIN_OFFSET
  pin_type& pin = pins[pinIndex];
  pin.sensorIndex = 0; // FORCE ARDUINO TO BE OUR SERVICE - DUNNO IF THIS IS GOOD/BAD
  pin.sensorType = SENSOR_TYPE_ANALOG_PIN_READER; // WIERD - mushing of roles/responsibilities
  pin.isActive = true;
  pin.rateModulus= (ioCmd[2] << 8)+ioCmd[3];
}

// ANALOG_READ_POLLING_STOP
void analogReadPollingStop() {
  pin_type& pin = pins[ioCmd[1]];
  pin.isActive = false;
}

// DIGITAL_READ_POLLING_START
void digitalReadPollingStart() {
  int pinIndex = ioCmd[1]; // + DIGITAL_PIN_COUNT / DIGITAL_PIN_OFFSET
  pin_type& pin = pins[pinIndex];
  pin.sensorIndex = 0; // FORCE ARDUINO TO BE OUR SERVICE - DUNNO IF THIS IS GOOD/BAD
  pin.sensorType = SENSOR_TYPE_DIGITAL_PIN_READER; // WIERD - mushing of roles/responsibilities
  pin.isActive = true;
  pin.rateModulus=(ioCmd[2] << 8) + ioCmd[3];
}

// PULSE
void pulse() {
  // get pin from index
  pin_type& pin = pins[ioCmd[1]];
  // FIXME - this has to unload a Long !!!
  pin.count = 0;
  //pin.target = toUnsignedLong(ioCmd, 2);// ioCmd[2];
  pin.target = ((ioCmd[2] << 24) + (ioCmd[3] << 16) + (ioCmd[4] << 8) + ioCmd[5]);
  pin.rate = ioCmd[6];
  pin.rateModulus = ioCmd[7];
  pin.isActive = true;
  pin.state = PUBLISH_SENSOR_DATA;
  //addNewValue(activePins, activePinCount, ioCmd[1]);
  //int pin = ioCmd[1];
  //addNewValue(digitalReadPin, digitalReadPollingPinCount, pin);
  // this is the same as digitalWrite except
  // we can keep track of the number of pulses
  //break;
}

// PULSE
void pulseStop() {
  pin_type& pin = pins[ioCmd[1]];
  // FIXME - this has to unload a Long !!!
  pin.state = PUBLISH_PULSE_STOP;
  //removeAndShift(activePins, activePinCount, ioCmd[1]);
}

// digital_READ_POLLING_STOP
void digitalReadPollingStop() {
  pin_type& pin = pins[ioCmd[1]];
  pin.isActive = false;
  //int pin = ioCmd[1];
  //removeAndShift(digitalReadPin, digitalReadPollingPinCount, pin);
  //break;
  // FIXME - these should just be attributes of the pin
}

// SET_TRIGGER
void setTrigger() {
  // NOT IMPLEMENTED
  // FIXME !!! - you need 1. a complete pin list !!!   analog & digital should be defined by attribute not
  // data structure !!!  if (pin.type == ??? if needed
  // TODO - if POLLING ALREADY DON'T RE-ADD - MAKE RE-ENTRANT
  //analogReadPin[analogReadPollingPinCount] = ioCmd[1]; // put on polling read list
  //++analogReadPollingPinCount;
}

// SET_DEBOUNCE
void setDebounce() {
  // default debounceDelay = 50;
  debounceDelay = ((ioCmd[1] << 8) + ioCmd[2]);
}

// SET_DIGITAL_TRIGGER_ONLY
void setDigitalTriggerOnly() {
  // NOT IMPLEMENTED
  //digitalTriggerOnly = ioCmd[1];
}

// SET_SERIAL_RATE
void setSerialRate() {
  Serial.end();
  delay(500);
  Serial.begin(ioCmd[1]);
}

// SET_SAMPLE_RATE
void setSampleRate() {
  // 2 byte int - valid range 1-65,535
  sampleRate = (ioCmd[1] << 8) + ioCmd[2];
  if (sampleRate == 0) {
    sampleRate = 1;
  } // avoid /0 error - FIXME - time estimate param
}

// SENSOR_ATTACH
void sensorAttach() {
  // THIS WILL BE THE NEW BIG-KAHUNA
  // INITIAL REQUEST - SENSOR GRABS ALL PINs IT NEEDS
  // IT THEN POPULATES each of the PINs with its sensorIndex
  // the uC (Arduino) - does not grab any - because it will
  // always take/recieve any non-reserved pin (softReset) Pin
  publishDebug("BSAM");
  int sensorIndex    = ioCmd[1];
  int sensorType     = ioCmd[2];
  int pinCount       = ioCmd[3];
  // for loop grabbing all pins for this sensor
  for (int ordinal = 0; ordinal < pinCount; ++ordinal){
    // grab the pin - assign the sensorIndex & sensorType
    pin_type& pin = pins[ioCmd[4 + ordinal]];
    pin.sensorIndex = sensorIndex;
    pin.sensorType = sensorType;
    if (pin.sensorType == SENSOR_TYPE_ULTRASONIC && ordinal == 0) {
      // pin.trigPin = ioCmd[3];
      // pin.echoPin = ioCmd[4];
      pinMode(pin.trigPin, OUTPUT); // WTF about wiring which has single pin ! :P
      pinMode(pin.echoPin, INPUT);
      //pin.ping = new NewPing(pin.trigPin, pin.echoPin, 100);
      // triggerPin's next pin is the echo pin
      pin.nextPin = ioCmd[5 + ordinal];
    } else if (pin.sensorType == SENSOR_TYPE_PULSE) {
      pin.address = ioCmd[3];
    }
  }
  publishDebug("ESAM");
}

// SENSOR_POLLING_START
void sensorPollingStart() {
  // FIXME - this is the same as DIGITAL PIN POLLING START
  int sensorIndex = ioCmd[1];
  pin_type& pin = pins[sensorIndex];
  pin.isActive = true;
  // I'm used to ms - and would need to change some
  // interfaces if i was to support inbound longs
  //pin.timeoutUS = ioCmd[2] * 1000;
  pin.timeoutUS = 20000; // 20 ms
  pin.state = ECHO_STATE_START;
}

// SENSOR_POLLING_STOP
void sensorPollingStop() {
  int sensorIndex = ioCmd[1];
  pin_type& pin = pins[sensorIndex];
  pin.isActive = false;
}

// Adafruit commands
// AF_BEGIN
void afBegin() {
  WIRE.begin();
  write8(ioCmd[1],PCA9685_MODE1, 0x0);
}

// AF_SET_PWM_FREQ
void afSetPWMFREQ() {
  //ioCmd[1] is the I2C address
  //ioCmd[2] is the freqency value
  int freq = 0.9 * ioCmd[2];  // Correct for overshoot in the frequency setting (see issue #11).
  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;
  uint8_t prescale = floor(prescaleval + 0.5);
  uint8_t oldmode = read8(ioCmd[1],PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
  write8(ioCmd[1],PCA9685_MODE1, newmode); // go to sleep
  write8(ioCmd[1],PCA9685_PRESCALE, prescale); // set the prescaler
  write8(ioCmd[1],PCA9685_MODE1, oldmode);
  delay(5);
  write8(ioCmd[1],PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
  // This is why the beginTransmission below was not working.
}

// AF_SET_PWM
void afSetPWM() {
  setPWM(ioCmd[1], ioCmd[2], ioCmd[3], ioCmd[4]);
}

// AF_SET_SERVO
void afSetServo() {
  setPWM(ioCmd[1], ioCmd[2], 0, (ioCmd[3] << 8) + ioCmd[4]);
}


void handleUltrasonicPing(pin_type& pin, unsigned long ts) {
  if (pin.state == ECHO_STATE_START) {
    // trigPin prepare - start low for an
    // upcoming high pulse
    pinMode(pin.trigPin, OUTPUT);
    digitalWrite(pin.trigPin, LOW);
    // put the echopin into a high state
    // is this necessary ???
    pinMode(pin.echoPin, OUTPUT);
    digitalWrite(pin.echoPin, HIGH);
    ts = micros();
    if (ts - pin.ts > 2) {
      pin.ts = ts;
      pin.state = ECHO_STATE_TRIG_PULSE_BEGIN;
    }
  } else if (pin.state == ECHO_STATE_TRIG_PULSE_BEGIN) {
    // begin high pulse for at least 10 us
    pinMode(pin.trigPin, OUTPUT);
    digitalWrite(pin.trigPin, HIGH);
    ts = micros();
    if (ts - pin.ts > 10) {
      pin.ts = ts;
      pin.state = ECHO_STATE_TRIG_PULSE_END;
    }
  } else if (pin.state == ECHO_STATE_TRIG_PULSE_END) {
    // end of pulse
    pinMode(pin.trigPin, OUTPUT);
    digitalWrite(pin.trigPin, LOW);
    pin.state = ECHO_STATE_MIN_PAUSE_PRE_LISTENING;
    pin.ts = micros();
  } else if (pin.state == ECHO_STATE_MIN_PAUSE_PRE_LISTENING) {
    ts = micros();
    if (ts - pin.ts > 1500) {
      pin.ts = ts;
      // putting echo pin into listen mode
      pinMode(pin.echoPin, OUTPUT);
      digitalWrite(pin.echoPin, HIGH);
      pinMode(pin.echoPin, INPUT);
      pin.state = ECHO_STATE_LISTENING;
    }
  } else if (pin.state == ECHO_STATE_LISTENING) {
    // timeout or change states..
    int value = digitalRead(pin.echoPin);
    ts = micros();
    if (value == LOW) {
      pin.lastValue = ts - pin.ts;
      pin.ts = ts;
      pin.state = ECHO_STATE_GOOD_RANGE;
    } else if (ts - pin.ts > pin.timeoutUS) {
      pin.state = ECHO_STATE_TIMEOUT;
      pin.ts = ts;
      pin.lastValue = 0;
    }
  } else if (pin.state == ECHO_STATE_GOOD_RANGE || pin.state == ECHO_STATE_TIMEOUT) {
    publishSensorDataLong(pin.address, pin.lastValue);
    pin.state = ECHO_STATE_START;
  } // end else if
}

// Helper methods to create MRLComm messages and write them back over the serial port.
// send a generic message
// TODO: this is never called
//void sendMsg() {
  // unbox
  //Serial.write(MAGIC_NUMBER);
  //Serial.write(paramBuffIndex + 2); // = param buff size + FN + paramCnt
  //Serial.write(PUBLISH_CUSTOM_MSG);
  //Serial.write(paramCnt);
  //for (int i = 0; i < paramBuffIndex; ++i) {
    //Serial.write(customParams[i]);
    //  }
  //  paramCnt = 0;
//  paramBuffIndex = 0;
//}

// send an error message/code back to MRL.
void sendError(int type) {
  Serial.write(MAGIC_NUMBER);
  Serial.write(2); // size = 1 FN + 1 TYPE
  Serial.write(PUBLISH_MRLCOMM_ERROR);
  Serial.write(type);
}

// publish a servo event.
void sendServoEvent(servo_type& s, int eventType) {
  // check type of event - STOP vs CURRENT POS
  Serial.write(MAGIC_NUMBER);
  Serial.write(5); // size = 1 FN + 1 INDEX + 1 eventType + 1 curPos
  Serial.write(PUBLISH_SERVO_EVENT);
  Serial.write(s.index); // send my index
  // write the long value out
  Serial.write(eventType);
  Serial.write(s.currentPos);
  Serial.write(s.targetPos);
}

void publishVersion() {
  Serial.write(MAGIC_NUMBER);
  Serial.write(2); // size
  Serial.write(PUBLISH_VERSION);
  Serial.write((byte)MRLCOMM_VERSION);
  Serial.flush();

}

// PUBLISH_SENSOR_DATA
void publishSensorData(int sensorIndex, int address, int value) {


  Serial.write(MAGIC_NUMBER);
  Serial.write(5); //size
  Serial.write(PUBLISH_SENSOR_DATA);
  Serial.write(sensorIndex);
  Serial.write(address);
  Serial.write(value >> 8);   // MSB
  Serial.write(value & 0xff); // LSB

}

// TODO: maybe this can be merged with above?
void publishSensorDataLong(int address, unsigned long lastValue) {
  Serial.write(MAGIC_NUMBER);
  Serial.write(6); // size 1 FN + 4 bytes of unsigned long
  Serial.write(PUBLISH_SENSOR_DATA);
  Serial.write(address);
  // write the long value out
  Serial.write((byte)(lastValue >> 24));
  Serial.write((byte)(lastValue >> 16));
  Serial.write((byte)(lastValue >> 8));
  Serial.write((byte)lastValue & 0xff);
}

void publishPulseStop(int state, int sensorIndex, int address, unsigned long count) {
  Serial.write(MAGIC_NUMBER);
  Serial.write(7); // size
  //Serial.write(PUBLISH_PULSE_STOP);  ?!?! commented out?
  Serial.write(state);
  Serial.write(sensorIndex);   // pin service
  Serial.write(address);       // Pin#
  Serial.write(count >> 24);   // MSB zoddly
  Serial.write(count >> 16);   // MSB
  Serial.write(count >> 8);    // MSB
  Serial.write(count & 0xff);  // LSB
}

void publishPulse(int state, int sensorIndex, int address, unsigned long count) {
  Serial.write(MAGIC_NUMBER);
  Serial.write(7); // size
  //Serial.write(PUBLISH_PULSE);  commented out ?!?!
  Serial.write(state);
  Serial.write(sensorIndex);// pin service
  Serial.write(address);// Pin#
  Serial.write(count >> 24);   // MSB zoddly
  Serial.write(count >> 16);   // MSB
  Serial.write(count >> 8);  // MSB
  Serial.write(count & 0xff);  // LSB
}

void publishLoadTimingEvent(unsigned long loadTime) {
  Serial.write(MAGIC_NUMBER);
  Serial.write(5); // size 1 FN + 4 bytes of unsigned long
  Serial.write(PUBLISH_LOAD_TIMING_EVENT);
  // write the long value out
  Serial.write((byte)(loadTime >> 24));
  Serial.write((byte)(loadTime >> 16));
  Serial.write((byte)(loadTime >> 8));
  Serial.write((byte)loadTime & 0xff);
}

void sendCommandAck() {
  Serial.write(MAGIC_NUMBER);
  Serial.write(1); // size 1 FN + 0 bytes
  Serial.write(PUBLISH_MESSAGE_ACK);
  // keep this synchronous
  Serial.flush();
}

// This method will publish a string back to the Arduino service
// for debugging purproses.
// NOTE:  If this method gets called excessively
// I have seen memory corruption in the arduino where
// it seems to be getting a null string passed in as "message"
// very very very very very odd..  I suspect a bug in the arduino hard/software
void publishDebug(String message) {
  //Serial.flush();
  Serial.write(MAGIC_NUMBER);
  Serial.write(1+message.length());
  Serial.write(PUBLISH_DEBUG);
  Serial.print(message);
  Serial.flush();
}
