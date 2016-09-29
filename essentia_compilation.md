## Essentia static build compilation

 Essentia and Fftw3 static builds in /libs were compiled using Essentia's source at *commit 5bfe18eca945bd2457d4a02c176f92a73df2ad6d* .

### Rebuilding

Source: https://github.com/MTG/essentia/blob/master/FAQ.md

#### Compile static lightweight version of Essentia:
* Configure compilation:

 `./waf configure --mode=release --build-static --lightweight=fftw`

- Compile (to: build/src/libessentia.a)

 `./waf`


#### Compile static Fftw:
* On Linux:

  `apt-get install yasm cmake`
* On OSX:

  `brew install yasm cmake wget`

* Compile:

  `cd packaging/debian_3rdparty`

  `build_fftw3.sh`
