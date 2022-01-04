#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    62
CRGB leds[NUM_LEDS];

int segment_size = 15;
int colors = 4;
int counter = 0;
int color_array[4];
String x;
void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}
void loop() {
 //int counters = random(2000);
 //for(int x = 0; x<=5;x++)
  //if(x == "on")
  //  waves();
  //else
  //  clear_all();
  while(Serial.available()>0){
    x = Serial.readStringUntil('\n');
    int commaIndex = x.indexOf(',');
    int secondCommaIndex = x.indexOf(',', commaIndex + 1);
    String firstValue = x.substring(0, commaIndex);
    String secondValue = x.substring(commaIndex + 1, secondCommaIndex);
    String thirdValue = x.substring(secondCommaIndex + 1); // To the end of the string
    int r = firstValue.toInt();
    int g = secondValue.toInt();
    int b = thirdValue.toInt();
    Serial.print(r);
    Serial.print(g);
    Serial.println(b);
    set_all(r,g,b);
  }
}
void set_all(int r, int g, int b){ 
  for(int y = 0; y<=NUM_LEDS; y++) {
  leds[y] = CRGB(r,g,b);
  FastLED.show();}
  
}
void clear_all(){
  for(int x = 0; x<NUM_LEDS; x++){
    leds[x] = CRGB(0,0,0);
  }
  FastLED.show();
}

  
