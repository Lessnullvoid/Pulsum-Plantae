
public class Sensor {
  private static final short AVGSIZE = 20;
  private static final short DISPLAYSIZE = 300;
  private static final short RAWSIZE = 30000;

  private short rawValues[] = new short[RAWSIZE+1];
  private short averageValues[] = new short[DISPLAYSIZE*2+1];
  private short currentRunningAverage[] = new short[AVGSIZE];
  private int averageSum;
  private short averageIndex;

  // begin/end indices for different things
  private short averageEnd, rawEnd;
  private short maxValue, minValue;
  private String name;
  private PVector location, dimension;

  public Sensor(PVector _location, PVector _dimension, String _name) {
    // set location/dimension of graphs and name of sensor
    location = _location;
    dimension = _dimension;
    name = _name;

    // initial end indices
    //   "end" points to one after the last value in range
    rawEnd = (short)((rawValues.length)-1);
    averageEnd = (short)((averageValues.length)-1);

    // init values
    for (int i=0; i<(rawValues.length); ++i) {
      rawValues[i] = 0;
    }
    for (int i=0; i<(averageValues.length); ++i) {
      averageValues[i] = 0;
    }
    for (int i=0; i<(currentRunningAverage.length); ++i) {
      currentRunningAverage[i] = 0;
    }

    minValue = (short)0x7fff;
    maxValue = 0;
    averageSum = 0;
    averageIndex = 0;
  }


  public short getMin() {
    return minValue;
  }
  public short getMax() {
    return maxValue;
  }
  public String getName(){
    return name;
  }

  public short getRawValue() {
    // last written value is at end-1
    // make sure index is positive
    int getFromIndex = (rawEnd > 0)?(rawEnd-1):((rawValues.length)-1);
    return rawValues[getFromIndex];
  }

  public short getAverageValue() {
    // last written value is at end-1
    // make sure index is positive
    int getFromIndex = (averageEnd > 0)?(averageEnd-1):((averageValues.length)-1);
    return averageValues[getFromIndex];
  }
  public short getAverageValueNormalized() {
    return (short)constrain((short)map(getAverageValue(), getMin(), getMax(), 0, 1023), 0, 1023);
  }


  public void addValue(short val) {
    // write value to raw array and update end index
    rawValues[rawEnd] = val;
    rawEnd = (short)((rawEnd+1)%(rawValues.length));

    // update running average
    averageSum -= currentRunningAverage[averageIndex];
    currentRunningAverage[averageIndex] = val;
    averageSum += currentRunningAverage[averageIndex];
    averageIndex = (short)((averageIndex+1)%(currentRunningAverage.length));

    // write to average values
    averageValues[averageEnd] = (short)(averageSum/(currentRunningAverage.length));
    averageEnd = (short)((averageEnd+1)%(averageValues.length));

    // find min/max of current averages
    short thisMinValue = (short)0x7fff;
    short thisMaxValue = 0;
    for (int i=0; i<(averageValues.length); ++i) {
      if (averageValues[i] > thisMaxValue) {
        thisMaxValue = averageValues[i];
      }
      if (averageValues[i] < thisMinValue) {
        thisMinValue = averageValues[i];
      }
    }

    // if there's a new min/max, update immediately
    //    else, slowly approach current min/max
    if (thisMaxValue > maxValue) {
      maxValue = thisMaxValue;
    }
    else {
      maxValue = (short)(0.99*maxValue + 0.01*thisMaxValue);
    }

    if (thisMinValue < minValue) {
      minValue = thisMinValue;
    }
    else {
      minValue += (short)(0.01*thisMinValue);
      minValue = (thisMinValue<minValue)?thisMinValue:minValue;
    }
  }

  public void draw() {
    pushMatrix();
    translate(location.x, location.y);

    // background rectangle
    fill(100);
    rect(0, 0, dimension.x*11/12, dimension.y);

    // sensor title
    fill(255);
    textSize(16);
    text(name, 10, 16);

    // raw graph
    pushMatrix();
    translate(0, dimension.y/4);
    drawGraph(rawValues, (short)(rawValues.length), rawEnd, dimension.x/2, dimension.y/2);
    popMatrix();

    // avg graph
    pushMatrix();
    translate(dimension.x/2+10, dimension.y/4);
    drawGraph(averageValues, (short)(averageValues.length), averageEnd, dimension.x*5/12-10, dimension.y/2);
    popMatrix();

    // info string
    pushMatrix();
    translate(0, dimension.y*3/4);
    fill(255);
    textSize(16);
    String ss = "current: "+getRawValue()+" min: "+minValue+" max: "+maxValue;
    text(ss, 0, 16);
    popMatrix();

    popMatrix();  // translate
  }

  void drawGraph(short values[], short sizeOfValues, short lastIndex, float gwidth, float gheight) {
    // graph
    PShape ps = createShape();
    ps.beginShape();
    ps.fill(255);
    ps.vertex(0, gheight);
    for (int x=1, i=(int)(lastIndex-gwidth); x<gwidth; ++x, ++i) {
      int yIndex = i;
      while (yIndex<0) {
        yIndex += sizeOfValues;
      }
      yIndex = yIndex%sizeOfValues;

      short y0 = (short)map(values[yIndex], 1023, 0, 0, gheight);
      ps.vertex(x, y0);
    }
    ps.vertex(gwidth, gheight);
    ps.endShape(CLOSE);

    // background rectangle
    fill(90);
    noStroke();
    rect(0, 0, gwidth, gheight);
    shape(ps, 0, 0);
  }
}

