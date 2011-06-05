#pragma once

#include "ofxOpenNIContext.h"

class ofxIRGenerator {
public:
	ofxIRGenerator(){}
	
	bool setup(ofxOpenNIContext* pContext);

	void generateTexture();
	
	void draw(float x=0, float y=0, float w=640, float h=480);
	
	xn::IRGenerator& getXnIRGenerator();

    unsigned char* image_pixels;
    
    float levelsLow, levelsHigh;
    
private:
	xn::IRGenerator ir_generator;
	ofTexture image_texture;

	int image_coloring;
};

