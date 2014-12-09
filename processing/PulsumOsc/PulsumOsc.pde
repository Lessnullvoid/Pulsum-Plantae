Sensor ss[] = new Sensor[6];
long lastAddMillis;

void setup() {
  size(600, 700, P2D);
  for (int i=0; i<ss.length; i++) {
    ss[i] = new Sensor(new PVector(20, 10+i*110), new PVector(560, 100), "sensor"+i);
  }
  lastAddMillis = millis();
}

void draw() {
  background(200);
  if (millis()-lastAddMillis > 100) {
    for (Sensor s:ss) {
      s.addValue((short)random(0, 1023));
    }
    println(frameRate);
  }
  for (Sensor s:ss) {
    s.draw();
  }
}

