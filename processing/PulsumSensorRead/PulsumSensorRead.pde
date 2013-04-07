import processing.serial.*;

Serial port;
int val;
int[][] values;
PVector graphDim;

color[] someColors = {
  #137f5e, #628c73, #92c099, #358e44, #6d9a50, #4d6045
};

void setup() {
  size(1280, 720);
  frameRate(60);
  println(Serial.list());
  port = new Serial(this, Serial.list()[4], 57600);
  graphDim = new PVector(width/2, height/3);
  values = new int[6][(int)graphDim.x];
  smooth();
}

void draw() {
  background(20, 30, 20);

  for (int j=0; j<2; j++) {
    pushMatrix();
    translate(j*graphDim.x, 0);
    for (int i=0; i<3; i++) {
      pushMatrix();
      translate(0, i*graphDim.y);
      stroke(someColors[j*3+i]);
      fill(someColors[j*3+i]);
      drawGraphShape(values[j*3+i]);
      popMatrix();
    }
    popMatrix();
  }
}

void serialEvent(Serial p) { 
  while (p.available () > 2) {
    if (p.read() == 0xAB) {
      int u = p.read();
      int l = p.read();

      for (int i=0; i<(int)graphDim.x-1; i++) {
        values[(u>>4)&0x07][i] = values[(u>>4)&0x07][i+1];
      }
      values[(u>>4)&0x07][(int)graphDim.x-1] = ((u<<8)&0x0F00) | (l&0xFF);
    }
  }
} 

int getY(int val) {
  return (int)(graphDim.y - (val / 1023.0f * graphDim.y));
}

void drawGraphLine(int[] vs) {
  strokeWeight(2);
  for (int x=1; x<graphDim.x; x++) {
    line(x-1, getY(vs[x-1]), x, getY(vs[x]));
  }
}

void drawGraphShape(int[] vs) {
  strokeWeight(2);
  beginShape();
  vertex(0, getY(0));
  for (int x=0; x<graphDim.x; x++) {
    vertex(x, getY(vs[x]));
  }
  vertex(graphDim.x, getY(0));
  endShape(CLOSE);
}

