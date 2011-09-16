#pragma once

#include "ofxOpenNIContext.h"

class ofxImageGenerator {
public:
	ofxImageGenerator();
    
    bool connected;
	
	bool setup(ofxOpenNIContext* pContext);

	void generateTexture();
	
	void draw(float x=0, float y=0, float w=640, float h=480);
	
	xn::ImageGenerator& getXnImageGenerator();
	
    //Used to specify the device specefic char (for multi kinect support)
    const char* deviceInfoChar;
    unsigned char* image_pixels;


private:
	xn::ImageGenerator image_generator;
	ofTexture image_texture;
	int image_coloring;
};

