#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
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
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(ofColor::seaGreen);
    float radius = 10 + 100*audioAnalyzer1.getRms();
    float xpos = ofGetWidth() *.5;
    float ypos = ofGetHeight()*.5;
    ypos -= ofGetHeight()*.5 * audioAnalyzer2.getRms();
    ofCircle(xpos, ypos, radius);
}
//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
    
        for (int i = 0; i < bufferSize; i++){
            buffer_1[i]	= input[i*nChannels];
            buffer_2[i]	= input[i*nChannels+1];
        }
        audioAnalyzer1.analyze(buffer_1, bufferSize);
        audioAnalyzer2.analyze(buffer_2, bufferSize);
    
}
//--------------------------------------------------------------
void testApp::exit(){
    soundStream.stop();
    soundStream.close();
    
    audioAnalyzer1.exit();
    audioAnalyzer2.exit();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
