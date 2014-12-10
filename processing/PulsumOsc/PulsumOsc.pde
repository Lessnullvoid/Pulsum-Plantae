import processing.serial.*;
import oscP5.*;
import netP5.*;

final static int OSC_IN_PORT = 8444;

Serial mSerial;
OscP5 mOscP5;

Sensor ss[] = new Sensor[6];
long lastOscMillis;

void setup() {
  size(600, 700, P2D);
  for (int i=0; i<ss.length; ++i) {
    ss[i] = new Sensor(new PVector(20, 10+i*110), new PVector(560, 100), "sensor"+i);
  }
  lastOscMillis = millis();
  mOscP5 = new OscP5(this, OSC_IN_PORT);
  mSerial = new Serial(this, Serial.list()[0], 57600);
}

void draw() {
  background(200);

  // read input
  while (mSerial.available () > 2) {
    if (mSerial.read() == 0xAB) {
      int upperByte = mSerial.read();
      int lowerByte = mSerial.read();

      short pinNumber = (short)((upperByte>>0x04)&0x07);
      short value = (short)(((upperByte<<8)&0x0F00) | (lowerByte&0xFF));

      ss[pinNumber].addValue(value);
    }
  }

  // write osc
  if (millis()-lastOscMillis > 100) {
    for (Sensor s:ss) {
      s.sendOsc();
    }
  }

  // draw
  for (Sensor s:ss) {
    s.draw();
  }
}

