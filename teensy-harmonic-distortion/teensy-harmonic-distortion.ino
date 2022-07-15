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

#define MAX_ORDER 2
float alpha[MAX_ORDER-1] = {0.05}; // First entry is second-order
float waveshape[32769] = {0};

void setup() {
  // put your setup code here, to run once:
  float *x = (float*)malloc(sizeof(float)*32769),
        *cheby_poly_lut_0 = (float*)malloc(sizeof(float)*32769),
        *cheby_poly_lut_1 = (float*)malloc(sizeof(float)*32769);
  
  for(int i = 0; i < 32769; i++){
    x[i] = 2.0*(i/32768.0)-1.0; // map from index to value
    cheby_poly_lut_0[i] = 1.0; // 0th chebyshev, DC
    cheby_poly_lut_1[i] = x[i]; // 1st chebyshev, pass-through

    waveshape[i] = cheby_poly_lut_1[i]; // init with the 1st chebyshev polynomial
  }

  for(int k = 2; k <= MAX_ORDER; k++){ // k is the cheby_order
    for(int i = 0; i < 32769; i++){
      // generate the next chebyshev polynomial then add it to waveshape
      float next_cheby_poly = 2.0*x[i]*cheby_poly_lut_1[i]-cheby_poly_lut_0[i];
      waveshape[i] += alpha[k-2]*next_cheby_poly;

      cheby_poly_lut_0[i] = cheby_poly_lut_1[i];
      cheby_poly_lut_1[i] = next_cheby_poly;
    }
  }

  // remove the offset at zero
  float zero_offset = waveshape[16384];
  for(int i = 0; i < 32769; i++) waveshape[i] -= zero_offset;

  // normalize the waveshaper to -1dB Vpk
  float max_abs_val = 0;
  for(int i = 0; i < 32769; i++){
    float abs_val = abs(waveshape[i]);
    if(abs_val > max_abs_val) max_abs_val = abs_val;
  }
  for(int i = 0; i < 32769; i++) waveshape[i] *= pow(10.0, -1.0/20.0)/max_abs_val;

  free(x);
  free(cheby_poly_lut_0);
  free(cheby_poly_lut_1);
  
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
