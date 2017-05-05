/*
 * 
 *  STARLYTE SOLAR SIMULATOR FOR ARDUINO
 *  SUBMITTED FOR 2017 NASA INTERNATIONAL SPACE APPS CHALLENGE
 *  PROJECT: STARLYTE
 *  AUTHORS: ANDREW DENIO, TILANI NEADEAU
 *  DATE: 5/5/2017
 *  
 *  HARDWARE: 
 *  ARDUINO UNO
 *  4 CHANNEL RELAY SHIELD (PINS 4/5/6/7)
 *  FOUR COLUMNS OF FIVE LEDS (CONTROLLED VIA RELAYS, BUT POWER IS SUPPLIED BY SOLAR PANEL)
 *  2 SWITCHES (PINS 8/11)
 *  SUNNYTECH SOLAR PANEL (5V / 250mW)
 *  MPU 6050 / GY-521 6 DOF SENSOR (I2C BUS)
 *  ADAFRUIT INA219 VOLTAGE AND CURRENT SENSOR (I2C BUS)
 *  ADAFRUIT MICRO USB LIPO CHARGER (HOOKED UP TO SOLAR PANEL)
 *  ADAFRUIT 500 mAh LIPO BATTERY (HOOKED UP TO CHARGER)
 *  WIRE (TO CONNECT EVERYTHING)
 *  4 51 OHM RESISTORS (HOOKED UP TO LED ARRAYS)
 *  
 *  SOME LIBRARIES HAVE BEEN USED, SEE BELOW
 *  
 *  FIND LIRBARIES AND PICTURES ON GITHUB PAGE
 * 
 */

//I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)  6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"  //#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68 specific I2C addresses may be passed as a parameter here
MPU6050 mpu;

/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw pitch
// roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
#define OUTPUT_READABLE_YAWPITCHROLL


#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

 //SET UP FOR CURRENT SENSOR
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

//Declare Variables for Current Sensor
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;

void setup() {

    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(38400);
    Serial.println("Hello!");
    
    // initialize digital pin 13 as an output.
    pinMode(13, OUTPUT);
  
    // initialize digital pins 4-8 as ouptut (LED array - 4 relays)
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);

    // initialize digital pins 8 and 11 as input (switchs for LED array)
    pinMode(8, INPUT_PULLUP);
    pinMode(11, INPUT_PULLUP);

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // wait for ready
    // Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    // while (Serial.available() && Serial.read()); // empty buffer
    // while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
    
    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
    
    #ifndef ESP8266
    while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
    #endif
    uint32_t currentFrequency;

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");

  
}

// MAIN LOOP FUNCTION
void loop() {
  // if programming failed, don't try to do anything
    if (!dmpReady) return;
    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        #endif

        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
    
  int switch1 = digitalRead(8);
  int switch2 = digitalRead(11);
  getVoltageCurrent();

  if (switch1 == LOW)
    {alwaysOn();}           //turn on all LEDs
  else if (switch2 == LOW)
    {sequentialOn();}       //turn on LEDs one column at a time
  else
    {turnOff();}
    
  delay(20);
}

void alwaysOn(){
    digitalWrite(4, HIGH);   // turn the first set of LEDs on
    digitalWrite(5, HIGH);   // turn the second set of LEDs on
    digitalWrite(6, HIGH);   // turn the third set of LEDs on
    digitalWrite(7, HIGH);   // turn the fourth set of LEDs on
}

void sequentialOn(){
    digitalWrite(4, HIGH);   // turn the first set of LEDs on
    getVoltageCurrent();     // print the voltage and current to Serial Monitor
    delay(2000);             // wait for 2 seconds
    digitalWrite(5, HIGH);   // turn the second set of LEDs on
    getVoltageCurrent();     // print the voltage and current to Serial Monitor
    delay(2000);             // wait for 2 seconds
    digitalWrite(6, HIGH);   // turn the third set of LEDs on
    getVoltageCurrent();     // print the voltage and current to Serial Monitor
    delay(2000);             // wait for 2 seconds
    digitalWrite(7, HIGH);   // turn the LED on (HIGH is the voltage level)
    getVoltageCurrent();     // print the voltage and current to Serial Monitor
    delay(2000);             // wait for 2 seconds
    digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(5, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(7, LOW);    // turn the LED off by making the voltage LOW
    getVoltageCurrent();     // print the voltage and current to Serial Monitor
    delay(1000);             // wait for a second
}

void turnOff()
{
    digitalWrite(4, LOW);    // turn the first set of LEDs off
    digitalWrite(5, LOW);    // turn the second set of LEDs off
    digitalWrite(6, LOW);    // turn the third set of LEDs off
    digitalWrite(7, LOW);    // turn the fourth set LEDs off
}

void getVoltageCurrent()
{
  //Calculate voltage and current
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  // print voltage, current, tilt to Serial Monitor
  Serial.print(busvoltage);  // Voltage
  Serial.print(",");
  Serial.print(current_mA);  // Current in milliamps
  Serial.print(",");
  Serial.print(ypr[0] * 180/M_PI);  //Tilt (pitch)
  Serial.print(",");
  Serial.println(ypr[2] * 180/M_PI); //Heading (yaw)
}


