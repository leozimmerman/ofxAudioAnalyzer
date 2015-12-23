#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    
    int sampleRate = 44100;
    int bufferSize = 512;
    int outChannels = 0;
    int inChannels = 2;
    int ticksPerBuffer = bufferSize/64;
    
    buffer_1 =  new float[bufferSize];
    buffer_2 =  new float[bufferSize];
    for(int i = 0; i < bufferSize; i++) { buffer_1[i]=buffer_2[i]= 0;}
    audioAnalyzer1.setup(512, 44100);
    audioAnalyzer2.setup(512, 44100);
    soundStream.setup(this, outChannels, inChannels, sampleRate, bufferSize, ticksPerBuffer);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(ofColor::seaGreen);
    float radius = 10 + 100*audioAnalyzer1.getRms();
    float xpos = ofGetWidth() *.5;
    float ypos = ofGetHeight()*.5;
    ypos -= ofGetHeight()*.5 * audioAnalyzer2.getRms();
    ofCircle(xpos, ypos, radius);
}
//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
        for (int i = 0; i < bufferSize; i++){
            buffer_1[i]	= input[i*nChannels];
            buffer_2[i]	= input[i*nChannels+1];
        }
        audioAnalyzer1.analyze(buffer_1, bufferSize);
        audioAnalyzer2.analyze(buffer_2, bufferSize);
    
}
//--------------------------------------------------------------
void ofApp::exit(){
    soundStream.stop();
    soundStream.close();
    
    audioAnalyzer1.exit();
    audioAnalyzer2.exit();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
