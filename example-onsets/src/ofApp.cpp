#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(34);
    
    
    sampleRate = 44100;
    bufferSize = 512;
    int channels = 1;
    
    audioAnalyzer.setup(sampleRate, bufferSize, channels);
    
    player.load("beatTrack.wav");
    cout<<"FILE INFO:"<<endl;
    cout<<"Duration: "<< player.getDuration()<<endl;
    cout<<"Num Channels: "<< player.getNumChannels()<<endl;
    
    gui.setup();
    gui.add(alpha.setup("alpha", 0.1, 0.0, 1.0));
    gui.add(silenceTreshold.setup("silence treshold", 0.02, 0.0, 1.0));
    gui.add(timeTreshold.setup("time treshold (ms.)", 100.0, 0.0, 1000.0));
    gui.add(useTimeTreshold.setup("use time treshold", false));
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    soundBuffer = player.getCurrentSoundBuffer(bufferSize);
    
    audioAnalyzer.setOnsetsParameters(0, alpha, silenceTreshold, timeTreshold, useTimeTreshold);
    audioAnalyzer.analyze(soundBuffer);
    
    onset = audioAnalyzer.getIsOnset(0);

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (onset){
        ofSetColor(ofColor::cyan);
        ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, 200);
    }
    
    gui.draw();
    
    ofSetColor(255);
    ofDrawBitmapString("ofxAudioAnalyzer - ONSETS EXAMPLE \n\nPress spacebar to play audio file \n'r' : resets onsets ", 350, 32);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    switch (key) {
        case ' ':
            player.play();
            break;
        case 'r':
            audioAnalyzer.resetOnsets(0);
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    audioAnalyzer.exit();
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
