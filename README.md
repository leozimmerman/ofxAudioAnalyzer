# ofxAudioAnalyzer

Introduction
-------
[openFrameworks](http://openframeworks.cc/) wrapper for [Essentia](http://essentia.upf.edu/). It provides audio analysis algorithms modified to process signals in real-time:
- RMS, Instant power, Energy.
- Pitch frequency, Pitch Confidence, Pitch Salience.
- Tuning Frequency.
- Onsets.
- FFT, Mel Bands, MFCC
- Harmonic Pitch Class Profile
- HFC, Centroid, Inharmonicity, Spectral Complexity. 
Algorithm reference: http://essentia.upf.edu/documentation/algorithms_reference.html

Demo video: https://vimeo.com/129795472


Compatibility
---------
Only OSX and Linux
 - OSX (!): needs the OF 64 bits version of Nick Hardeman - https://github.com/NickHardeman/openframeworks_osx_64.  (Tested in OS X 10.9.5) 
 - Linux 64 bits. (Tested with Ubuntu 12.04)
 
Installation
---------
The two examples included in the repo can be used to build new projects over them.
For including the addon in new projects:
- OSX - Xcode:
  * Install Essentia library: http://essentia.upf.edu/documentation/installing.html
  * Project Generator -> New project (openframeworks_osx_64)
  * Build Settings->Architectures->64-bit Intel 
  * Add ofxAudioAnalyzer directories to the project src/ and libs/
  * Build Seetings -> Library Search Paths -> “/usr/local/lib”
  * Build Settings -> User Header Search Paths -> “../../../addons/ofxAudioAnalyzer/libs/essentia/include” (non-recursive)
  * Dragg the following library files from “/usr/local/lib” to the root of the Xcode project in the left side file hierarchy window: libavcodec.a - libavformat.a - libavutil.a - libessentia.a - libfftw3f.a  - libsamplerate.a - libswresample.a  - libtag.dylib -libyaml.a
	

- Linux:
  * Install Essentia library: http://essentia.upf.edu/documentation/installing.html
  * Creat New Project
  * addons.make -> ofxAudioAnalyzer
  * config.make -> PROJECT_LDFLAGS= -lessentia -lfftw3f -ltag -lavcodec -lyaml -lsamplerate -lavformat  


License
------------
Essentia library and this addon are distributed under  Affero GPLv3 license. 



