# ofxAudioAnalyzer

## Description

[openFrameworks](http://openframeworks.cc/) addon for audio analysis. It provides the following algorithms :
* RMS, Instant power, Energy, Pitch frequency, Pitch Confidence, Pitch Salience, HFC, Centroid, Inharmonicity, Spectral Complexity, Dissonance, Roll Off, Odd To Even Harmonic Energy Ratio, Strong Peak, Strong Decay,  Onsets, Spectrum, Mel Bands, MFCC, Harmonic Pitch Class Profile, Tristimulus

  See [AlgorithmsReference.md](AlgorithmsReference.md)

This addon uses [Essentia](http://essentia.upf.edu/) library.

Demo video: https://vimeo.com/129795472


## Compatibility

  OSX & Linux - OF 0.9.3 - 64 bits compilation.


## Dependencies
  For playing and analyzing audio files (ofSoundPlayerExtended), [ofxAudioDecoder](https://github.com/kylemcdonald/ofxAudioDecoder) addon is needed. The examples in this repository were built using this fork of the addon: https://github.com/leozimmerman/ofxAudioDecoder


## Usage

* The examples included in the repository can be used to build new projects over them, specially the empty ones. They can be compiled with Xcode or by Terminal using the Makefiles.

* Creating a new project:
  - Create a new project using the Project Generator. Be sure to include ofxAudioAnalyzer and ofxAudioDecoder addons.

  - Include ofxAudioAnalyzer header into ofApp.h
  ```cpp
    #include "ofxAudioAnalyzer.h"
  ```
  - Create an instance of ofxAudioAnalyzer into ofApp class:
  ```cpp
  ofxAudioAnalyzer audioAnalyzer;
  ```
  - In the **setup()** function setup the audioAnalyzer's parameters (sampleRate, bufferSize, channels). Be sure that the parameters of the audioAnalyzer match the parameters of the ofSoundStream or the audio files you load to the sound player to avoid errors:
  ```cpp
  audioAnalyzer.setup(44100, 512, 2);
  ```
  - Analyze the ofSoundBuffer in ofApp::audioIn() and ofApp::audioOut() when using ofSoundStream, or in the ofApp::update() if you are using audio file player, ofSoundBuffer **parameters must always match** audioAnalyzer's:
  ```cpp
  audioAnalyzer.analyze(soundBuffer);
  ```

  - Retrieve analysis results specifying algorithm, channel and smooth amount (smooth is optional):
  ```cpp
  float rms = audioAnalyzer.getValue(RMS, channel, smoothAmount);
  ```


## History - New features

  * Added static builds of Essentia library to the addon directory, no need to install it into the system.
  * Multichannel ofSoundBuffer functionality.
  * Smoothing and Normalizing values functionality integrated into the algorithms.
  * Added audio file player functionality.
  * New examples added.
  * New algorithms added: Dissonance, Roll Off, Odd To Even Harmonic Energy Ratio, Strong Peak, Strong Decay and Tristimulus.


## License
Essentia library and this addon are distributed under  Affero GPLv3 license.
