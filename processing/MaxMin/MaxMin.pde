import processing.serial.*;


int start = 0, end = 200, count = 0, iMax = 0, iMin = 0;
int [] iArray;
PFont font;
Serial myPort; 
//int val;      // Data received from the serial port

 
void setup ()
{
  size (800, 450);
  smooth();
  font = createFont ("Arial Bold", 14);
  textFont (font, 14);
  iArray = new int [end-start];
  for (int i = 0; i < iArray.length; i++) iArray [i] = 0;
  //String portName = Serial.list()[0];
  //myPort = new Serial(this, portName, 9600);
}
 
void draw ()
{
  
 // if ( myPort.available() > 0) {  // If data is available,
  //  val = myPort.read();         // read it and store it in val
 // }
  
background (0);
  fill (255);
  stroke (247);
  float steps = (float) width / (float) iArray.length;
  float h = 0;
  for (int i = 0; i < iArray.length; i++)
  {
    h = map (iArray[i], iArray [iMin], iArray [iMax], 0, height*0.75);
    if (i == iMax) fill (121, 195, 229);
    else fill (180);
    beginShape ();
    vertex ((i*steps), height);
    vertex (i*steps+steps, height);
    vertex (i*steps+steps, height-h);
    vertex ((i*steps), height-h);
    endShape (CLOSE);
  }
   
  fill (121, 195, 229);
ellipse (iMin*steps + steps/2, height-steps, steps*2, steps*2);
 
  noStroke();
  fill (80);
  rect (0, 0, width, 40);
  String txt = "RANDOM: Iterations: " + nfs (count, 10) + "x10, Most chosen number: " +  str(iMax) + " (" + str (iArray [iMax]) + "x)" + ", Least chosen number: " + str (iMin) + " (" + str (iArray[iMin]) + "x)";
  fill (247);
  text (txt, 20, 25);
 
 
  for (int i = 0; i < 10; i++) doRandom();
  count++;
}
 
void doRandom ()
{
  int index = int ( random (start, end));
  iArray [index] =  iArray [index] + 1;
 
  if (iArray[index] > iArray [iMax]) iMax = index;
  checkMin();
}
 
void mousePressed ()
{
count = iMax = iMin = 0;
setup();
}
 
void checkMin ()
{
  int tempVal = count;
  int tempPos = iArray.length-1;
  for (int i = 0; i < iArray.length; i++)
  {
    if (iArray [i] < tempVal)
    {
      tempVal = iArray [i];
      tempPos = i;
    }
  }
 
  iMin = tempPos;
}
