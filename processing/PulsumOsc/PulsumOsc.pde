import processing.serial.*;
import oscP5.*;
import netP5.*;

final static int OSC_IN_PORT = 8444;
final static int OSC_OUT_PERIOD = 100;
final static int OSC_OUT_PORT = 8666;
final static String OSC_OUT_HOST = "localhost";
final static String OSC_OUT_PATTERN = "/pulsum-osc/";

Serial mSerial;
OscP5 mOscP5;
NetAddress oscOutAddress = new NetAddress(OSC_OUT_HOST, OSC_OUT_PORT);
OscMessage mMessage = new OscMessage(OSC_OUT_PATTERN);

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
    for (int i=0; i<ss.length; ++i) {
      String mAddrPatt = OSC_OUT_PATTERN+ss[i].getName()+"/";

      // min
      mMessage.clear();
      mMessage.setAddrPattern(mAddrPatt+"min");
      mMessage.add(ss[i].getMin());
      mOscP5.send(mMessage, oscOutAddress);

      // max
      mMessage.clear();
      mMessage.setAddrPattern(mAddrPatt+"max");
      mMessage.add(ss[i].getMax());
      mOscP5.send(mMessage, oscOutAddress);

      // filtered
      mMessage.clear();
      mMessage.setAddrPattern(mAddrPatt+"filtrado");
      mMessage.add(ss[i].getAverageValueNormalized());
      mOscP5.send(mMessage, oscOutAddress);

      // raw
      mMessage.clear();
      mMessage.setAddrPattern(mAddrPatt+"crudo");
      mMessage.add(ss[i].getRawValue());
      mOscP5.send(mMessage, oscOutAddress);
    }
  }

  // draw
  for (Sensor s:ss) {
    s.draw();
  }
}

