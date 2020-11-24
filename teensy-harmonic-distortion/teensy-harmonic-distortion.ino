#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=97,52
AudioEffectWaveshaper    waveshape1;     //xy=271,33
AudioEffectWaveshaper    waveshape2;     //xy=271,71
AudioOutputI2S           i2s2;           //xy=436,51
AudioConnection          patchCord1(i2s1, 0, waveshape1, 0);
AudioConnection          patchCord2(i2s1, 1, waveshape2, 0);
AudioConnection          patchCord3(waveshape1, 0, i2s2, 0);
AudioConnection          patchCord4(waveshape2, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=252,111
// GUItool: end automatically generated code

#define MAX_ORDER 3
float alpha[MAX_ORDER-1] = {0.05}; // First entry is second-order
float waveshape[32769];

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < 32769; i++){
    float y = 2*(i/32768.)-1;
    float x = asin(y), y_mod = sin(x), divisor = 1;
    for(int i = 0; i < MAX_ORDER-1; i++){
      int n = i+2;
      float theta = (n+1)*HALF_PI;
      y_mod += alpha[i]*sin(n*x+theta);
      if(!(n % 2)){
        y_mod += alpha[i];
        divisor += 2*alpha[i];
      }
      else divisor -= alpha[i];
    }
    waveshape[i] = y_mod / divisor;
  }

  bool error = false;
  for(int i = 0; i < 32769; i++)
    if(abs(waveshape[i]) > 1) error = true;
  if(error){
    Serial.print("Harmonics too large! Stopping...");
    while(1);
  }
  
  AudioMemory(12);
  waveshape1.shape(waveshape, 32769);
  waveshape2.shape(waveshape, 32769);
  sgtl5000_1.enable();
  sgtl5000_1.lineInLevel(3); // Optimal for 2V_p-p signal
  sgtl5000_1.lineOutLevel(25); // Under 25 causes soft clipping
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
