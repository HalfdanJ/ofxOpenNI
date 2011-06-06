#pragma once

#include "ofxOpenNIContext.h"

class ofxImageGenerator {
public:
	ofxImageGenerator();
	
	bool setup(ofxOpenNIContext* pContext);

	void generateTexture();
	
	void draw(float x=0, float y=0, float w=640, float h=480);
	
	xn::ImageGenerator& getXnImageGenerator();
	
    //Used to specify the device specefic char (for multi kinect support)
    const char* deviceInfoChar;

private:
	xn::ImageGenerator image_generator;
	ofTexture image_texture;
	unsigned char* image_pixels;
	int image_coloring;
};

