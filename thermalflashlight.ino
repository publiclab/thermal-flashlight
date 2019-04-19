// Simplified and documented code for the Public Lab Thermal Flashlight
// using the MLX90614 temperature sensor and one RGB LED
// https://publiclab.org/n/625

// You'll need the Adafruit Melexis library here: 
// https://learn.adafruit.com/using-melexis-mlx90614-non-contact-sensors?view=all#download-adafruit-mlx90614-2-7

#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600); Serial.println("Adafruit MLX90614 test");  
  mlx.begin();
  delay(2000); // wait for sensor to stabilize, this could be unnecessary? randomly chose 2 seconds
  int count = 0;
}

void loop() { // runs repeatedly
  float tempF = mlx.readObjectTempF();

  // set range and colors here:
  int low = tempF;
  int high = tempF + 20; // range
  int hot = 0; // RED as an angle 0-360 degrees on the color wheel: https://publiclab.org/n/19110
  int cold = 240; // BLUE

  int hue = map(tempF, low, high, cold, hot); // convert from temp range to color range
  setLedColorHSV(hue, 1, 1); // Saturation and Value constant at 1
  
  // approach 1: show colors in comparison to ambient temp 
  //             (but we have to know what ambient temp is with a 2nd thermometer?)
  // approach 2: (used here) set start temperature with object temp in first 2-4 seconds; 
  //             light activating shows it's "finished reading"

  if (count > 500) {
    Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
    Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
    Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
    Serial.print("*F\tObject = "); Serial.print(tempF); Serial.println("*F"); Serial.println();
    count = 0; // reset
  }
  count += 1;
}

// set the current color for the RGB LED
void setLedColor(int red, int green, int blue) {
  analogWrite(4, red); // Red pin attached to 4
  analogWrite(5, green); // Green pin attached to 5
  analogWrite(7, blue); // Blue pin attached to 7
}

//Convert a given HSV (Hue Saturation Value) to RGB(Red Green Blue) and set the led to the color
//  h is hue value, integer between 0 and 360
//  s is saturation value, double between 0 and 1
//  v is value, double between 0 and 1
// http://splinter.com.au/blog/?p=29
// this is the algorithm to convert from RGB to HSV
void setLedColorHSV(int h, double s, double v) {
  double r=0; 
  double g=0; 
  double b=0;

  double hf=h/60.0;

  int i=(int)floor(h/60.0);
  double f = h/60.0 - i;
  double pv = v * (1 - s);
  double qv = v * (1 - s*f);
  double tv = v * (1 - s * (1 - f));

  switch (i) {
  case 0: //rojo dominante
    r = v;
    g = tv;
    b = pv;
    break;
  case 1: //verde
    r = qv;
    g = v;
    b = pv;
    break;
  case 2: 
    r = pv;
    g = v;
    b = tv;
    break;
  case 3: //azul
    r = pv;
    g = qv;
    b = v;
    break;
  case 4:
    r = tv;
    g = pv;
    b = v;
    break;
  case 5: //rojo
    r = v;
    g = pv;
    b = qv;
    break;
  }

  // set each component to a integer value between 0 and 255
  int red = constrain((int)255*r,0,255);
  int green = constrain((int)255*g,0,255);
  int blue = constrain((int)255*b,0,255);

  setLedColor(red, green, blue);
}

/*************************************************** 
  This incorporates code from a library example for the MLX90614 Temp Sensor

  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1747 3V version
  ----> https://www.adafruit.com/products/1748 5V version

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
 ****************************************************/
