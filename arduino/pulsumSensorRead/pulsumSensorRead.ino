#define AVGSIZE 20
#define NUMPINS 6

// read at most 100 times a second
#define READPERIOD 10

// write at most 10 times a second
#define WRITEPERIOD 100

int values[NUMPINS][AVGSIZE];
int sum[NUMPINS];
int index[NUMPINS];
int whichPin;

unsigned long lastRead, lastWrite;

void setup() {
  Serial.begin(57600);
  whichPin = 0;
  lastRead = lastWrite = millis();

  // set all values, sums and indexes to 0
  for(int i=0; i<NUMPINS; i++) {
    for(int j=0; j<AVGSIZE; j++) {
      values[i][j] = 0;
    }
    sum[i] = 0;
    index[i] = 0;
  }
}

void loop() {
  unsigned long timeNow = millis();

  // don't read pins more frequent than 100 times per second
  // and only read one pin per loop to avoid noise
  if(timeNow-lastRead > READPERIOD){
    int newValue = analogRead(whichPin);

    // replace value at current index
    sum[whichPin] -= values[whichPin][index[whichPin]];
    values[whichPin][index[whichPin]] = newValue;
    sum[whichPin] += values[whichPin][index[whichPin]];
    // update index for this pin
    index[whichPin] = (index[whichPin]+1)%AVGSIZE;

    whichPin = (whichPin+1)%NUMPINS;
    lastRead = timeNow;
  }

  if(timeNow-lastWrite > WRITEPERIOD){
    // send 3 bytes per value: HHHH_HHHH AAAA_VVVV VVVV_VVVV
    // where HHHH_HHHH = 8-bit fixed header
    //       AAAA = which pin [0,15]
    //       VVVV VVVV_VVVV = 12 bits of data [0,4095]

    byte msg[3];
    byte header = 0xAB;
    for(int i=0; i<NUMPINS; i++) {
      int avg = sum[i]/AVGSIZE;

      msg[0] = (header&0xFF);
      msg[1] = ((i<<4)&0xF0) | ((avg>>8)&0x0F);
      msg[2] = (avg&0xFF);

      Serial.write(msg[0]);
      Serial.write(msg[1]);
      Serial.write(msg[2]);
    }
    Serial.flush();
    // use millis() because flush could take longer
    lastWrite = millis();
  }
}

