#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	song.load("song.mp3");
	song.play();

	/*
	bgrVideo.load("video.mov");
	bgrVideo.setVolume(0);
	bgrVideo.play();
	*/
	width = ofGetWidth();
	height = ofGetHeight();

	vidGrabber.setVerbose(true);
	vidGrabber.setup(width, height);

	fftSmooth = new float[8192];

	for (int i = 0; i < 8192; i++)
	{
		fftSmooth[i] = 0;
	}

	bands = 64;

	colorImg.allocate(width, height);
	grayImage.allocate(width, height);
	grayBg.allocate(width, height);
	grayDiff.allocate(width, height);

	bLearnBakground = true;
	threshold = 10;

	screenSelector = 0;

}

//--------------------------------------------------------------
void ofApp::update()
{

	bool bNewFrame = false;

	/*
	bgrVideo.update();
	bNewFrame = bgrVideo.isFrameNew();
	*/

	vidGrabber.update();
	bNewFrame = vidGrabber.isFrameNew();

	if (bNewFrame) {


		//colorImg.setFromPixels(bgrVideo.getPixels());
		colorImg.setFromPixels(vidGrabber.getPixels());



		grayImage = colorImg;

		if (bLearnBakground == true) {
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (width * height) / 3, 10, true);	// find holes
	}

	playbackPosition = song.getPosition();
	float* value = ofSoundGetSpectrum(bands);

	for (int i = 0; i < bands; i++)
	{
		fftSmooth[i] *= 0.9;

		if (fftSmooth[i] < value[i])
		{
			fftSmooth[i] = value[i];
		}
	}

	bgrVideo.update();

}

//--------------------------------------------------------------
void ofApp::draw()
{
	//bgrVideo.draw(0, 0, ofGetWidth(), ofGetHeight());
	//ofSetColor(ofColor::blue);

	// then draw the contours:

	ofFill(); 
	ofSetColor(ofColor::black); //Background color
	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	ofSetColor(ofColor::yellow);

	
	contourFinder.draw();

	if (contourFinder.nBlobs > 0) {
		for (int k = 0; k < contourFinder.nBlobs; k++) {
			for (int i = 0; i < contourFinder.blobs[k].pts.size(); i++) {
				ofVec3f point(contourFinder.blobs[k].pts[i]);
				line.curveTo(point);
			}
		}
		line.close();
		line.draw();
	}

	

	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 't')
	{
		threshold += 10;
	}

	if (key == 'y')
	{
		threshold -= 10;
	}
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
