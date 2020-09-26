/*
 Example: Control a WTV020-SD-16P module to play voices from an Arduino board.
 Created by Diego J. Arevalo, August 6th, 2012.
 Released into the public domain.
 */

#include <Wtv020sd16p.h>
#define NUMCLIPS 32
#define RECENTBUFFER 25
#define BUFFER 300

int resetPin = 6;  // The pin number of the reset pin.
int clockPin = 3;  // The pin number of the clock pin.
int dataPin = 4;  // The pin number of the data pin.
int busyPin = 5;  // The pin number of the busy pin.

int eyePin = 13;

Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

int clipLengths[] = {6200,1408,4250,4644,2119,1325,3297,2436,1066,1631,3651,1461,1273,2676,4930,3565,2596,1738,1925,2467,2161,2704,4261,2702,3608,2791,2606,1734,5746,0674,6102,3171};
long playbackStartTime = 0;
int currentlyPlaying = -1;
int lastLoop = -1;
int recent[RECENTBUFFER];
int DELAYSCALE = 1;

boolean recentlyPlayed(int i) {
  for(int j=0;j<RECENTBUFFER;j++) {
    if(recent[j]==i) {
      return true;
    }
  }
  return false;
}

void markAsPlayed(int i) {
  for(int j=RECENTBUFFER-1;j>0;j--) {
    recent[j] = recent[j-1];
  }
  recent[0] = i;
}

int pickNext() {
  int i;
  do {
    i = random(NUMCLIPS + 1);
  } while((i<=1) || (recentlyPlayed(i)));
  markAsPlayed(i);
  return i;
}

void waitToNotBeBusy(int threshold) {
  int consecutiveOffCount = 0;
  while(consecutiveOffCount < threshold) {
    digitalWrite(eyePin, HIGH);
    if (digitalRead(busyPin) == LOW) {
      consecutiveOffCount = 0;
    }
    else {
      consecutiveOffCount++;
    }
    delay(100);
  }
  digitalWrite(eyePin, LOW);
}

void setup() {
  pinMode(busyPin, INPUT);
  pinMode(eyePin, OUTPUT);
  digitalWrite(eyePin, HIGH);
  
  currentlyPlaying = -1;
  randomSeed(analogRead(0));
   
  // set up button interrupt
  attachInterrupt(0, button, LOW);
  
  //Initializes the module.
  delay(1500);
//  waitToNotBeBusy(60);
  wtv020sd16p.reset();
  
  // wait for the WTV020 to start up
  delay(1500);
//  waitToNotBeBusy(60);
  
  // play back clip 1, wait for completion
  wtv020sd16p.asyncPlayVoice(1);
  delay((clipLengths[0] * DELAYSCALE) + BUFFER);
}

void stopPlayback() {
  currentlyPlaying = -1;
  wtv020sd16p.stopVoice();
}

void button() {
  detachInterrupt(0);
  
  for (int k=0;k<7;k++) {
    digitalWrite(eyePin, LOW);
    delay(2000);
    digitalWrite(eyePin, HIGH);
    delay(2000);
  }
  
  
  // stop current playback if necessary
  if (currentlyPlaying != -1) {
    stopPlayback();
  }
  // pick the next value
  currentlyPlaying = pickNext();
}


void loop() {
  if(lastLoop != currentlyPlaying) {
    // start playback
    playbackStartTime = millis();
    wtv020sd16p.asyncPlayVoice(currentlyPlaying);
    lastLoop = currentlyPlaying;      
  }
  else {
    // mark whether enough time has elapsed; if so, set currentlyPlaying to -1
    if ((millis() - playbackStartTime) > ((clipLengths[currentlyPlaying-1] * DELAYSCALE) + BUFFER)) {
      attachInterrupt(0, button, LOW);
      stopPlayback();  
    }
  }
}


