// number of analog pins to read
#define NUMPINS 6

// read at most 100 times a second
#define READPERIOD 10

// a header for outgoing serial messages
#define MSGHEADER 0xAB

byte serialMsg[3];
int whichPin;
unsigned long lastReadWrite;

void setup() {
  Serial.begin(57600);
  serialMsg[0] = serialMsg[1] = serialMsg[2] = 0x00;
  whichPin = 0;
  lastReadWrite = millis();
}

void loop() {
  // don't read pins more frequent than 100 times per second
  // and only read one pin per loop to avoid noise
  if(millis()-lastReadWrite > READPERIOD){
    short readValue = analogRead(whichPin);

    // send 3 bytes per value: HHHH_HHHH AAAA_VVVV VVVV_VVVV
    // where HHHH_HHHH = 8-bit fixed header
    //       AAAA = which pin [0,15]
    //       VVVV VVVV_VVVV = 12 bits of data [0,4095]

    serialMsg[0] = (MSGHEADER&0xFF);
    serialMsg[1] = ((whichPin<<4)&0xF0) | ((readValue>>8)&0x0F);
    serialMsg[2] = (readValue&0xFF);

    Serial.write(serialMsg[0]);
    Serial.write(serialMsg[1]);
    Serial.write(serialMsg[2]);

    Serial.flush();
    whichPin = (whichPin+1)%NUMPINS;
    // use millis() because flush could take longer
    lastReadWrite = millis();
  }
}

