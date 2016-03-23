// This #include statement was automatically added by the Particle IDE.
#include "PhoBot/PhoBot.h"
#include "blynk/blynk.h"
#include "HC_SR04/HC_SR04.h"
#include "SimpleTimer.h"

// DEBUG JUNK
//#define BLYNK_PRINT
#define BLYNK_PRINT Serial


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = " ";

// Attach virtual serial terminal to Virtual Pin V1
// Debug from Blynk App when serial port not available
WidgetTerminal terminal(V0);

// Create a Timer object called "timer"! 
SimpleTimer timer;

// No coding is required for direct pin operations!

float volts = 0.0;
float distance = 0.0;

PhoBot p = PhoBot();
HC_SR04 rangefinder = HC_SR04(p.trigPin, p.echoPin);

void setup() {
    Serial.begin(9600);
    delay(5000); // Allow board to settle
    Blynk.begin(auth);
    
    timer.setInterval(500L, sendValues); //  Here you set interval (500 msec) and which function to call
}

void loop() {
    Blynk.run();
    timer.run(); // SimpleTimer is working
    
    if (ModeBtnPressed()) {
        Blynk.notify("Mode button was pressed");
    }
    
}

BLYNK_WRITE(V3) {
  int x = param[0].asInt();
  int y = param[1].asInt();
  
  String controlString = "S";

  // Do something with x and y
  // terminal.print("X = ");
  // terminal.print(x);
  // terminal.print("; Y = ");
  // terminal.println(y);
  // terminal.flush();
  
  if (x == 128 && y == 128) {
      control(controlString);
  }
  else if (x < 55) {
      controlString = "L-40";
  }
  else if (x > 183) {
      controlString = "R-40";
  }
  else if (y < 55) {
      controlString = "B-35";
  }
  else if (y > 183) {
      controlString = "F-100";
  }
  control(controlString);
}

int control(String command) {
    // "F-100"  - Forward Speed 100 
    // "B-100"  - Back Speed 100
    // "L-100"  - Left Speed 100
    // "R-100"  - Right Speed 100
    // "S"      - Stop
    
    return p.control(command);
}

void sendValues() {
    // This function sends Phobot Sonar Range, Battery Voltage and Photon
    // Uptime every 500 milliseconds to Virtual Pin V1, V2 and V5 respectively
    // In the app, Widget's reading frequency should be set to PUSH
    // You can send anything with any interval using this construction
    // Don't send more that 10 values per second
    
    volts = p.batteryVolts();
    distance = rangefinder.getDistanceCM();
    
    //BLYNK_LOG("This is my voltage value: %d", volts);
    //BLYNK_LOG("This is my distance value: %d", distance);
    
    //terminal.println(volts);
    //terminal.println(distance);
    //terminal.flush();
    
    Blynk.virtualWrite(V1, volts);
    Blynk.virtualWrite(V2, distance);
    Blynk.virtualWrite(V5, millis() / 1000);
}

// *** Utility functions

bool ModeBtnPressed() {
    if(millis() > 5000) {
        if(BUTTON_GetDebouncedTime(BUTTON1) >= 50) {
            BUTTON_ResetDebouncedState(BUTTON1);
            return 1;
        }
    }
    return 0;
}
