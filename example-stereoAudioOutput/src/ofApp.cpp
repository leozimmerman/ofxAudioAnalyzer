#include "ofApp.h"

/*
    Example built over https://github.com/openframeworks/openFrameworks/tree/master/examples/sound/audioOutputExample
    Modified to work with ofSoundBuffer and ofxAudioAnalyzer
 */

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(34, 34, 34);
    ofSetFrameRate(60);
    
    sampleRate = 44100;
    bufferSize = 512;
    int nInChannels = 0;
    int nOutChannels = 2;
    

    // setup the sound stream
    ofSoundStreamSetup(nOutChannels, nInChannels, sampleRate, bufferSize, 3);

    //setup ofxAudioAnalyzer with the SAME PARAMETERS
    audioAnalyzer.setup(sampleRate, bufferSize, nOutChannels);

    
    //oscillator variables setup
    phase 				= 0;
    phaseAdder 			= 0.0f;
    phaseAdderTarget 	= 0.0f;
    volume				= 0.1f;
    bNoise 				= false;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //get the analysis values
    rms_l = audioAnalyzer.getSingleValue(RMS, 0);
    rms_r = audioAnalyzer.getSingleValue(RMS, 1);
    
    pitchFreq_l = audioAnalyzer.getSingleValue(PITCH_FREQ, 0);
    pitchFreq_r = audioAnalyzer.getSingleValue(PITCH_FREQ, 1);
    
    spectrum_l = audioAnalyzer.getVectorValues(SPECTRUM, 0);
    spectrum_r = audioAnalyzer.getVectorValues(SPECTRUM, 1);
    
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(225);
    ofDrawBitmapString("ofxAudioAnalyzer - STEREO AUDIO OUTPUT EXAMPLE", 32, 32);
   
    //-WAVEFORM DISPLAY:

    ofNoFill();
    
    // draw the left channel:
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 150, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Left Channel", 4, 18);
    
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, 400, 200);
    
    
    ofSetColor(ofColor::hotPink);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (unsigned int i = 0; i < lastBuffer.getNumFrames(); i++){
        float x =  ofMap(i, 0, lastBuffer.getNumFrames(), 0, 400, true);
        ofVertex(x, 100 -lastBuffer.getSample(i, 0)*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
    // draw the right channel:
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 350, 0);
    
    ofSetColor(225);
    ofDrawBitmapString("Right Channel", 4, 18);
    
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, 400, 200);
    
    ofSetColor(ofColor::hotPink);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (unsigned int i = 0; i < lastBuffer.getNumFrames(); i++){
        float x =  ofMap(i, 0, lastBuffer.getNumFrames(), 0, 400, true);
        ofVertex(x, 100 -lastBuffer.getSample(i, 1)*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
    //-ANALYSIS METERS:
    ofPushStyle();
    ofPushMatrix();
    
    int mw = 300;
    
    ofFill();
    
    ofDrawLine(ofGetWidth()/2, 0 , ofGetWidth()/2, ofGetHeight());
    
    ofTranslate(ofGetWidth()/2, 0, 0);
    
    ofDrawBitmapString("ANALYSIS METERS", 50, 32);
    
    ofSetColor(255);
    string strRmsL = "RMS Left Channel: " + ofToString(rms_l);
    ofDrawBitmapString(strRmsL, 50, 100);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(50, 110, rms_l*mw, 10);
    
    string strRmsR = "RMS Right Channel: " + ofToString(rms_r);
    ofSetColor(255);
    ofDrawBitmapString(strRmsR, 50, 150);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(50, 160, rms_r*mw, 10);
    
    string strPitchL = "PITCH Left Channel: " + ofToString(pitchFreq_l);
    ofSetColor(255);
    ofDrawBitmapString(strPitchL, 50, 250);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(50, 260, ofClamp(pitchFreq_l/MAX_FREQUENCY, 0.0, 1.0)*mw, 10);
    
    string strPitchR = "PITCH Right Channel: " + ofToString(pitchFreq_r);
    ofSetColor(255);
    ofDrawBitmapString(strPitchR, 50, 300);
    ofSetColor(ofColor::cyan);
    ofDrawRectangle(50, 310, ofClamp(pitchFreq_r/MAX_FREQUENCY, 0.0, 1.0)*mw, 10);
    
    ofSetColor(255);
    ofDrawBitmapString("SPECTRUM Left Channel: ", 50, 400);
    ofPushMatrix();
    ofTranslate(50, 410);
    ofSetColor(ofColor::cyan);
    float bin_w = (float) mw / spectrum_l.size();
    for (int i = 0; i < spectrum_l.size(); i++){
        float scaledValue = ofMap(spectrum_l[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * 100);
        ofDrawRectangle(i*bin_w, 100, bin_w, bin_h);
    }
    ofPopMatrix();
    
    ofSetColor(255);
    ofDrawBitmapString("SPECTRUM Right Channel: ", 50, 600);
    ofPushMatrix();
    ofTranslate(50, 610);
    ofSetColor(ofColor::cyan);
    bin_w = (float) mw / spectrum_r.size();
    for (int i = 0; i < spectrum_r.size(); i++){
        float scaledValue = ofMap(spectrum_r[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * 100);
        ofDrawRectangle(i*bin_w, 100, bin_w, bin_h);
    }
    ofPopMatrix();
    
    
    
    ofPopStyle();
    ofPopMatrix();
    
    
    //-KEYS COMMANDS:
    ofSetColor(225);
    string infoString = "click mouse for noise\nvolume: ("+ofToString(volume, 2)+") modify with -/+ keys\npan: ("+ofToString(pan, 2)+") modify with mouse x\nsynthesis: ";
    if( !bNoise ){
        infoString += "sine wave (" + ofToString(targetFrequency, 2) + "hz) modify with mouse y";
    }else{
        infoString += "noise";
    }
    ofDrawBitmapString(infoString, 32, 579);
    
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    if (key == '-' || key == '_' ){
        volume -= 0.05;
        volume = MAX(volume, 0);
    } else if (key == '+' || key == '=' ){
        volume += 0.05;
        volume = MIN(volume, 1);
    }

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    
    //pan = 0.5f;
    float leftScale = 1 - pan;
    float rightScale = pan;
    
    // sin (n) seems to have trouble when n is very large, so we
    // keep phase in the range of 0-TWO_PI like this:
    while (phase > TWO_PI){
        phase -= TWO_PI;
    }
    
    if ( bNoise == true){
        // ---------------------- noise --------------
        for (int i = 0; i < outBuffer.getNumFrames(); i++){
            outBuffer.getSample(i, 0) = ofRandom(0, 1) * volume * leftScale;
            outBuffer.getSample(i, 1) = ofRandom(0, 1) * volume * rightScale;
        }
    } else {
        phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
        for (int i = 0; i < outBuffer.getNumFrames(); i++){
            phase += phaseAdder;
            float sample = sin(phase);
            outBuffer.getSample(i, 0)  = sample * volume * leftScale;
            outBuffer.getSample(i, 1)  = sample * volume * rightScale;
        }
    }
    
    //----------------------------------------------
    
    unique_lock<mutex> lock(audioMutex);
    lastBuffer = outBuffer;
    
    //ANALYZE SOUNDBUFFER:
    audioAnalyzer.analyze(lastBuffer);

}


//--------------------------------------------------------------
void ofApp::exit(){
    ofSoundStreamStop();
    audioAnalyzer.exit();
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    int width = ofGetWidth();
    pan = (float)x / (float)width;
    float height = (float)ofGetHeight();
    float heightPct = ((height-y) / height);
    targetFrequency = MAX_FREQUENCY * heightPct;
    phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    int width = ofGetWidth();
    pan = (float)x / (float)width;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bNoise = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bNoise = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
