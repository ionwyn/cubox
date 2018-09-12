/*
   This code enables playback of a .wav audio file from an SD card.
   It uses the SdFat library to minimize program space on the very limited pro mini memory.
   Serial transmission can be used when debugging, otherwise it can be commented out for regular operation.
   This code is based on some example code found in the TMRpcm library.
   Currently, only the playback function can be used.
   For recording capabilities, please consult the TMRpcm library and GitHub repository.
   Further information can be found in project documentation PDF ("Music Box Overview.pdf")
   Ionwyn I. Sean, Diane A. Wolf, 2017.7.27
*/

// libraries, files, and global variables
#include <SdFat.h>              // SD FAT library
#include <SPI.h>                // SPI library
#include <TMRpcm.h>             // .wav audio playback library
#include <pcmConfig.h>            // audio library advanced configuration file
#include <pcmRF.h>                // another file for audio library
#define SD_ChipSelectPin 8          // SD slave control line on pin 8
#define led 7                   // indication LED on pin 7
#define pl 2                     // pushbutton switch on pin 2
#define swBounce 256              // time delay in ms for switch

// objects and variables
SdFat fat;                      // SdFat object
TMRpcm snd;                    // TMRpcm object (snd=sound)
char file [] = "paradise.wav";  // test audio
int plState = 0;                // create pushbutton switch state variable (off/LOW)

void setup() {
  snd.speakerPin = 9;           // set pin 9 as audio output to speaker
  pinMode(pl, INPUT);           // set pin 2 (switch) as input for plau function
  pinMode(led, OUTPUT);         // set pin 7 (LED) as output
//  srlOK();                      // uncomment if monitoring serial data
  sdOK();                       // comment if monitoring serial data
}

void loop(void) {
  plState = digitalRead(pl);                    // read switch state (high/low) from pin 2
  if (plState == HIGH && !(snd.isPlaying())) {  // check if switch is depressed and no audio is playing
    plybck();
  }
  else if (plState == HIGH && snd.isPlaying()) { // check if switch is depressed and audio is playing
    stpsnd();
  }
}

void srlOK() // check serial comms with SD card
{
  Serial.begin(115200);                // begin serial transmission
  if (!fat.begin(8, SPI_FULL_SPEED)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");
    return;                             // don't do anything more if not
  }
  else {
    Serial.println("SD OK");
  }
}
void sdOK() // check SD card
{
  if (!fat.begin(8, SPI_FULL_SPEED)) {
    return;
  }
}
void plybck() // play .wav sound file
{
  snd.play(file);                            // play audio
  digitalWrite(led, HIGH);                   // turn LED on to indicate audio is playing
//  Serial.println("Playing audio...");       // uncomment if monitoring serial data
  delay(swBounce);                           // short delay before looping to prevent wrong readings
}
void stpsnd() // stop .wav sound file playback
{
  snd.disable();                                // stop audio entirely
  digitalWrite(led, LOW);                       // turn LED off to indicate audio playback has stopped
//  Serial.println("Stopping audio...");         // uncomment if monitoring serial data
  delay(swBounce);                              // short delay before looping to prevent wrong readings
}
