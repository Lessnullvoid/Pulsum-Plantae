import oscP5.*;
import netP5.*;

final static int OSC_IN_PORT = 8444;
final static int OSC_OUT_PERIOD = 100;
final static int OSC_OUT_PORT = 8666;
final static String OSC_OUT_HOST = "localhost";
final static String OSC_OUT_PATTERN = "/pulsum-osc/";

OscP5 mOscP5;
NetAddress oscOutAddress = new NetAddress(OSC_OUT_HOST, OSC_OUT_PORT);
OscMessage mMessage = new OscMessage(OSC_OUT_PATTERN);

Sensor ss[] = new Sensor[6];
long lastOscMillis;

void setup() {
  size(600, 700, P2D);
  for (int i=0; i<ss.length; i++) {
    ss[i] = new Sensor(new PVector(20, 10+i*110), new PVector(560, 100), "sensor"+i);
  }
  lastOscMillis = millis();
  mOscP5 = new OscP5(this, OSC_IN_PORT);
}

void draw() {
  background(200);
  if (millis()-lastOscMillis > 100) {

    for (Sensor s:ss) {
      s.addValue((short)random(0, 1023));
    }

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
  for (Sensor s:ss) {
    s.draw();
  }
}

