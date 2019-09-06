/**
 * Mouse Signals. 
 * 
 * Move and click the mouse to generate signals. 
 * The top row is the signal from "mouseX", 
 * the middle row is the signal from "mouseY",
 * and the bottom row is the signal from "mousePressed". 
 */
import processing.serial.*;

Serial mySerial;

float posx = 0;
float posy = 0;
float posz = 0;

float previousx = posx;
float previousy = posy;
float previousz= 0;

float translatex = 0;
float translatey =0;
float translatez = 0;

boolean start = false;

void setup() 
{
  size(900, 900);
  noSmooth();


  mySerial = new Serial(this, "COM10", 500000);
  mySerial.clear();

  stroke(0, 0, 240);
  strokeWeight(5);
  translate(450, 450);
}

int arrayindex = 0;

void draw()
{
  if (mySerial.available() > 0)
  {    
    String value = mySerial.readStringUntil(10); //lf
    if (value != null) {
      try {
        //value = value.substring(0, value.length()-1);
        String[] splitted = split(value, ' ');
        previousx = posx;
        previousy = posy;
        previousz = posz;
        /*
      posx = Integer.valueOf(splitted[0]);
         posy = - Integer.valueOf(splitted[1]);
         posz = Integer.valueOf(splitted[2]);*/
        posx = float(splitted[0]) / 20;
        posy = - float(splitted[1]) / 20;
        posz = (float(splitted[2]) /13000)*2*PI;
      }
      catch(Exception e) {
        print("error : ");
        print(value);
        print(e);
      }
    }
  }
  if (start) {
    line(posx - translatex + 450, posy - translatey + 450, previousx - translatex + 450, previousy - translatey +450);
    stroke(240, 0, 0);
    lineAngle(800, 800, posz, float(50));
    stroke(0,0,240);
  } else {
    posx=0;
    posy = 0;
    previousx = 0;
    previousy=0;
    posz=0;
    previousz=0;
    background(200);
  }
}

void keyPressed()
{
  start = true;
  translatex = posx;
  translatey = posy;
  translatez = posz;
  background(200);

  stroke(250, 0, 0);
  ellipse(800, 800, 50, 50);
  stroke(0,0,240);
}

void lineAngle(int x, int y, float angle, float length)
{
  line(x, y, x+cos(angle-0)*length, y-sin(angle-0)*length);
}
