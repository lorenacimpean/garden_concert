
#include <MIDI.h>

// analog pin 0
const byte outputPin = A0;
const int readDelay = 200;
MIDI_CREATE_DEFAULT_INSTANCE();
// value from reading
int sensorValue;
// maximum sensor value
int sensorMax = 1023;
// minimum sensor value
int sensorMin = 0;

void setup() {
  // initialize serial communication at 9600 bits per second (debug):
  Serial.begin(9600);
  // Set MIDI baud rate:
  // Serial.begin(31250);
  // check if async calibrate with interval runs forever
}

void loop() {
  readSensor();

}

// WIP

void readSensor() {
  // read output pin
  sensorValue = analogRead(outputPin);
  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, sensorMin, sensorMax);
  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);
  // print output
  Serial.println(sensorValue);
  // delay in between reads for stability
  delay(readDelay);
}

void sendNote() {
  // Send a Note (pitch 42, velo 127 on channel 1)
  MIDI.sendNoteOn(42, 127, 1);
  // Wait for a second -> change interval later
  delay(1000);
  // Stop the note
  MIDI.sendNoteOff(42, 0, 1);
}

void calibrate() {
  // change to async and call at 5s(?) intervals
  while (millis() < 5000) {
    sensorValue = analogRead(sensorPin);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }
  // set notes based on min max
}

void computeVelocity() {
  // async reading in between readings - 15? 20? millis
  // sort values
  // count duplicates
  // set velocity based on duplicate count
  // max velocity = 127; min ?? 0;
  // step = 127 / 15 (this read interval + maybe add some random to spice things up);
  // velocity = step * duplicates count;
  // add values to map: {note, velocity}
}
