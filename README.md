# ofxAudioAnalyzer

##Description

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


##Compatibility

OSX & Linux - OF 0.9.0 - 64 bits compilation. 

 
##Essentia Installation

Install Essential library: http://essentia.upf.edu/documentation/installing.html
	* Last commit of ofxAudioAnalyzer was tested with [ https://github.com/MTG/essentia/tree/master — commit a440f018a04631c57df95ce54e99736a73f3117c].
	* There is no need to include the gaia flag in the Essential configuration (./waf configure —with-gaia).

##Adding ofxAudioAnalyzer to a New Project

* The examples included in the repository can be used to build new projects over them. They can be compiled with Xcode or by Terminal using the Makefiles.

* For including the addon in new projects:

  * 1- Install Essentia library.
  * 2- Project Generator: Create a new project with the ofxAudioAnalyzer addon.  
  - Makefiles by Terminal (OSX & Linux): 
	* 3- config.make -> PROJECT_LDFLAGS= -lessentia -lfftw3 -lyaml -lavcodec -lavformat -lavutil -lsamplerate -ltag -lfftw3f -lavresample.  

  - Xcode (OSX):
  	* 3- Build Settings -> Library Search Paths -> “/usr/local/lib”.
  	* 4- Drag the following library files from “/usr/local/lib” to the root of the Xcode project in the left side file hierarchy window: libavcodec.a - libavformat.a - libavresample - libavutil.a - libessentia.a - libfftw3f.a  - libsamplerate.a - libswresample.a  - libtag.dylib - libyaml.a.
  


##License

Essentia library and this addon are distributed under  Affero GPLv3 license. 



