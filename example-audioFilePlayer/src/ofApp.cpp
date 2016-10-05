#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(34, 34, 34);
    ofSetFrameRate(60);
    
    sampleRate = 44100;
    bufferSize = 512;
    int channels = 2;
    
    audioAnalyzer.setup(sampleRate, bufferSize, channels);
    
    player.load("mix-stereo.wav");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    soundBuffer = player.getCurrentSoundBuffer(bufferSize);
    
    //ANALYZE SOUNDBUFFER:
    audioAnalyzer.analyze(soundBuffer);
    
    spectrum_l = audioAnalyzer.getValues(SPECTRUM, 0);
    spectrum_r = audioAnalyzer.getValues(SPECTRUM, 1);
    
    
    //Waveform update:
    waveform_l.clear();
    waveform_r.clear();
    
    //waveform for channel 0
    int ch=0;
    for(size_t i = 0; i < soundBuffer.getNumFrames(); i++) {
        float sample = soundBuffer.getSample(i, ch);
        float x = ofMap(i, 0, soundBuffer.getNumFrames(), 0, ofGetWidth()*0.5);
        float y = ofMap(sample, -1, 1, 0, ofGetHeight()*0.5);
        waveform_l.addVertex(x, y);
    }
    
    //waveform for channel 1
    ch = 1;
    for(size_t i = 0; i < soundBuffer.getNumFrames(); i++) {
        float sample = soundBuffer.getSample(i, ch);
        float x = ofMap(i, 0, soundBuffer.getNumFrames(), 0, ofGetWidth()*0.5);
        float y = ofMap(sample, -1, 1, 0, ofGetHeight()*0.5) + ofGetHeight()*0.5;
        waveform_r.addVertex(x, y);
    }


}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //draw waveforms:
    ofSetColor(ofColor::hotPink);
    waveform_l.draw();
    waveform_r.draw();
    
    //draw spectrums:
    int mw = ofGetWidth()/2;
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, 0);
    ofSetColor(ofColor::cyan);
    float bin_w = (float) mw / spectrum_l.size();
    for (int i = 0; i < spectrum_l.size(); i++){
        float scaledValue = ofMap(spectrum_l[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * ofGetHeight()/2);
        ofDrawRectangle(i*bin_w, ofGetHeight()/2, bin_w, bin_h);
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofSetColor(ofColor::cyan);
    bin_w = (float) mw / spectrum_r.size();
    for (int i = 0; i < spectrum_r.size(); i++){
        float scaledValue = ofMap(spectrum_r[i], DB_MIN, DB_MAX, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * ofGetHeight()/2);
        ofDrawRectangle(i*bin_w, ofGetHeight()/2, bin_w, bin_h);
    }
    ofPopMatrix();
    
    
    ofSetColor(100);
    ofDrawLine(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
    ofDrawLine(0, ofGetHeight()/2, ofGetWidth(), ofGetHeight()/2);
    ofSetColor(255);
    ofDrawBitmapString("ofxAudioAnalyzer - AUDIO FILE PLAYER EXAMPLE \nPress any key to play audio file ", 350, 32);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
     player.play();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
