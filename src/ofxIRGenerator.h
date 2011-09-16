#pragma once

#include "ofxOpenNIContext.h"

class ofxIRGenerator {
public:
	ofxIRGenerator();
	
	bool setup(ofxOpenNIContext* pContext);

	void generateTexture();
	
	void draw(float x=0, float y=0, float w=640, float h=480);
	
	xn::IRGenerator& getXnIRGenerator();

    unsigned char* image_pixels;
    ofTexture * getTexture();
    
    float levelsLow, levelsHigh;
    
    //Used to specify the device specefic char (for multi kinect support)
    const char* deviceInfoChar;
    const char* deviceInstanceName;
    
    bool connected;

private:
	xn::IRGenerator ir_generator;
	ofTexture image_texture;

	int image_coloring;
};

