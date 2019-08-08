/*
  PS2 Pinout
  1 : Data
  3 : GND
  4 : VCC
  5 : CLK
*/

//representation : mouse moves x and y from normal view -> 2D polar coordinates (r = sqrt(x²+y²), theta = atan(y/x))
// mouse turns around = phi -> angular offset of polar in its reference

#include "PS2Mouse.h"
#include "coordinates.h"

#define PITCH 0.00318 //resolution in cm of the optical mouse
              // we can get an 8-bit (127 values) per communication. 
              //max speed therefore depends on communication speed -> oscilloscope clock 12.85kHz, actual data rate in this code: 59Hz
              // max speed = 59*127*0.00318 = 24cm/sec

PS2Mouse mouse(6, 5); //clk, data
PS2Mouse mouse2(3, 2); //clk data for mouse 2

float posx = 0;
float posy = 0;
float posy2 = 0;
float posz = 0;

float previousx = posx;
float previousy = posy;
float previousy2 = posy2;
float previousz = posz;

float vector[2];

int x, y, x2, z;

bool rotation = true;
bool polar = true;

Coordinates polarCoord = Coordinates();

void setup() {
  Serial.begin(500000);
  while (!Serial);
  Serial.print("Setup...");

  mouse.begin();
  mouse2.begin();
  Serial.println("complete!");
}

void loop() {
  uint8_t stat, stat2;
  mouse.getPosition(stat, x, y);
  mouse2.getPosition(stat2, x2, z);
  
  previousx = posx;
  previousy = posy;
  previousy2 = posy2;
  previousz = posz;

  posz = previousz + (float)z;

  if (rotation) {
    toVector(x, y, posz); //rotation of x and y depending on z reference
    x = vector[0];
    y = vector[1];
  }
  posx = previousx + (float)x;
  posy = previousy + (float)y;
  posy2 = previousy2 + (float)x2; //in its reference, x of mouse2 is y in mouse1.
  
  Serial.print(posx);
  Serial.print(" ");
  Serial.print(posy);
  Serial.print(" ");
  Serial.print(posz);
  Serial.print(" ");
  Serial.println("end");

/*
  Serial.print("distance parcourue : ");
  Serial.println(PITCH*posx);
*/

  if (polar) {
    //go from cart to polar coordinates
    polarCoord.fromCartesian(x, y);
    /*
    Serial.print("polar coord : ");
    Serial.print(polarCoord.getR());
    Serial.print(" ");
    Serial.println(polarCoord.getAngle() * (360 / (2 * PI)));
  */
  }
}


void toVector(float x, float y, float z) {
  //rotates the vector around the z axis to follow the path
  float rotatedX = (float) (x * cos(z / 360) - y * sin(z / 360));
  float rotatedY = (float) (x * sin(z / 360) + y * cos(z / 360));
  float rotatedVector[2];
  vector[0] = rotatedX;
  vector[1] = rotatedY;
}



