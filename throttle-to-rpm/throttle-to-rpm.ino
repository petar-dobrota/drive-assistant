#include <Keyboard.h>

#include <Arduino.h>
#include <Wire.h>
#include <OBD.h>
#include <SPI.h>
#include <SD.h>
#include "Print.h"

// obd adapter pinout:
// rx - white
// tx - green

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTANTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// PINS
const unsigned ALARM_PIN = 13;

const unsigned SD_CS_PIN = 10;
// miso 12
// mosi 11
// sck  13
const unsigned THROTTLE_POS_PIN = A0;

// TEST: DELETE
const unsigned TEST_0_PIN = 2;
const unsigned TEST_1_PIN = 3;
const unsigned TEST_2_PIN = 4;

const unsigned F_NAME_LEN = 13;

void signalError() {
  digitalWrite(TEST_0_PIN, LOW);
  while(true);
}
    
class DataLogger {
  private:
  
    char currLog[F_NAME_LEN];
    
    void makeFileName(char* fname, int logNum) {
      sprintf(fname, "RUN_%04d.LOG", logNum);
    }

  public:

    DataLogger() {
    }

    const char* getCurrLog() {
      return currLog;
    }

    void begin() {
      char *max="RUN_0000.LOG";
      
      File dir = SD.open("/");
      dir.rewindDirectory();
      while (true) {
    
        File entry =  dir.openNextFile();

        if (!entry) {
          // no more files
          break;
        }
            
        if (!entry.isDirectory()) {
          if(strcmp(entry.name(), max) > 0) {
            strcpy(max, entry.name());
          }
        }

        entry.close();    
      }
      dir.close();   

      // find next num
      max[8] = 0;
      int val = atoi(max + 4) + 1;
      
      makeFileName(currLog, val);
    }

    void log(int rpm, int throttlePos) {
      File f = SD.open(currLog, FILE_WRITE);

      if (!f) {
        signalError();
      }

      f.print(millis());
      f.print(", ");
      f.print(throttlePos);
      f.print(", ");
      f.print(rpm);

      f.print('\n');
      f.flush();
      f.close();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

COBD obd;
int currRpm;
DataLogger logger;

void setup()
{

  pinMode(ALARM_PIN, OUTPUT);

  pinMode(TEST_0_PIN, OUTPUT);
  pinMode(TEST_1_PIN, OUTPUT);
  pinMode(TEST_2_PIN, OUTPUT);
  
  digitalWrite(TEST_0_PIN, HIGH);
  digitalWrite(TEST_1_PIN, HIGH);
  digitalWrite(TEST_2_PIN, HIGH);

  if (!SD.begin(SD_CS_PIN)) {
    signalError();
  }
  logger.begin();

  obd.begin();
  while (!obd.init());  

  digitalWrite(TEST_1_PIN, LOW);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// General functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int obdRead(byte pid) {
  int value;
  if (obd.read(pid, value)) {
    return value;
  } else {
    // TODO: Stop exec
    return -1;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN CORUTINES: 
// Will be called from main loop.
// Return true if no other co-routines should be called per a this loop
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void fMonitoring() {
  int throttlePos = analogRead(THROTTLE_POS_PIN);
  logger.log(currRpm, throttlePos);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{

  // TODO: OBD READS
  currRpm = obdRead(PID_RPM);

  fMonitoring();

  delay(50);
}