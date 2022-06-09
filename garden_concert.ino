
#include <MIDI.h>
#include <AsyncTimer.h>

#define outputPin A0
#define readDelay 200
// http://www.phys.unsw.edu.au/jw/notes.html
// midi notes from 21 - 108 - adjust later
// min 12, max 87 - adjust later
#define midiMinNote 21
#define midiNotes 87

MIDI_CREATE_DEFAULT_INSTANCE();
// default minimum sensor value
int sensorMin = 0;
// default maximum sensor value
int sensorMax = 1023;
// value from reading
int sensorValue;
AsyncTimer timer;

// WIP
void calibrate() {
  int instantRead = analogRead(outputPin);
  // record the maximum sensor value
  if (instantRead > sensorMax) {
    sensorMax = instantRead;
  }
  // record the minimum sensor value
  if (instantRead < sensorMin) {
    sensorMin = instantRead;
  }
  delay(100);
}

int getPitchFromReading() {
  int diff = sensorValue - sensorMin;
  if (diff < 0) {
    diff = - diff;
  }
  int note = diff % midiNotes + midiMinNote;
  return note;
}

void readSensor() {
  // read output pin
  sensorValue = analogRead(outputPin);
  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, sensorMin, sensorMax);
  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 1023);
  // print output
  Serial.println(sensorValue);
  // delay in between reads for stability
}

void sendNote() {
  // Send a Note (pitch 42, velo 127 on channel 1)
  int pitch = getPitchFromReading();
  int velocity = (int) random(0, 128);
  int channel = 1;
  MIDI.sendNoteOn(pitch, velocity, channel);
  //we can make this random
  delay(100);
  // Stop the note
  MIDI.sendNoteOff(42, 0, 1);
}

void setup() {
  // initialize serial communication at 9600 bits per second (debug):
  Serial.begin(9600);
  // Set MIDI baud rate:
  // Serial.begin(31250);
  // sets a timer and calls calibrate() at 5s intervals hopefully forever
  timer.setInterval([]() {
    calibrate();
  }, 5000);
}

void loop() {
  timer.handle();
  // sets a timer and calls calibrate() with 200milli timeout
  timer.setTimeout([]() {
    readSensor();
    sendNote();
  }, 200);
}
