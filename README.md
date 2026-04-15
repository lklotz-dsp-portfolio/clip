# dependencies

- cmake
- portaudio

# build

- cmake -B build
- cmake --build build

# run

./build/callback


# Description and overview

I wanted to challenge myself by writing this in C instead of python, but I do want to try python
in future assignments.

I was mostly curious about writing the .wav file becuase I've only ever written text files.
For writing the wave file, I used code from stack overflow which I modified slightly.
Once the header information was setup to be written, it was fairly straight forward 
to generate the audio. I messed up the audio pretty bad a number of times due to type casting
from float to int16_t, which sucked for my ears and my speakers. 

Audio playback was also a bit tricky. First of all, the audio callback function I used from
portaudio does a lot behind the hood, so that made it slightly confusing to use. Second, the
example I found from their documentation was written for stereo output, and so I had to do a bit more
than expected to convert it to mono. Additionally, they were using 32 bit float instead of int16_t,
which I didnt catch until my playback made the most abrasively harsh sound I've ever heard in my life.

All in all, this was a really fun assignment and I'm excited to expand on it from here.

