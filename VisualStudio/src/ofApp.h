#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
		ofFbo fbo_curMode;
		ofFbo fbo_nextMode;
		ofShader ofs_common;
		ofShader ofs_rgbMode;
		ofShader ofs_rgbNoiseWave;
		ofShader ofs_rgbKaleido;
		ofShader ofs_rgbSpray;
		ofShader ofs_rgbRoll;
		ofShader ofs_postProcess;
		
		// Non OpenGL Test Bed
		//ofTexture texColor;
		//ofPixels colorPixels;
};