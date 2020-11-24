# teensy-harmonic-distortion

A project for digitally adding harmonic distortion (tube sound?) to audio, built in Arduino for the Teensy 4.0 with the Audio Adapater Board. Full writeup [here](https://kenny-peng.com/2020/11/23/teensy_harmonic_distortion.html). Upload as-is to get a ratio of 0.05 between the fundemental and the second harmonic, basically <=5% THD. To adjust it, configure these two lines of code:

```C++
#define MAX_ORDER 2
float alpha[MAX_ORDER-1] = {0.05}; // First entry is second-order
```

like this (example: second harmonic ratio of 0.05 and a third harmonic ratio of 0.005)

```C++
#define MAX_ORDER 3
float alpha[MAX_ORDER-1] = {0.05, 0.005}; // First entry is second-order
```

This code will not start if the desired harmonics are large enough to break the math, so check the Serial Monitor for an error before using it.

The code currently inputs 2V (measured) peak-to-peak line signals and outputs 1.62V ([quoted](https://github.com/PaulStoffregen/Audio/blob/master/control_sgtl5000.cpp#L666)) peak-to-peak line signals. Configure the in and out levels however you need, but be warned that outputting any higher appears to cause soft clipping (additional third harmonics).