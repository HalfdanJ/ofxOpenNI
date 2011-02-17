#pragma once

#include "ofxOpenNIContext.h"


class ofxDepthGenerator {
public:
	ofxDepthGenerator();
	
	bool setup(ofxOpenNIContext* pContext);
	void setMirrored(bool mirror);
	
	void generateTexture();
	
	void draw(float x=0, float y=0, float w=640, float h=480);
	
	void update();
	
	xn::DepthGenerator& getXnDepthGenerator();
	
	unsigned char * depth_pixels;

private:
	xn::DepthGenerator depth_generator;
	ofTexture depth_texture;
	int depth_coloring;
	float max_depth;
	
	bool updateTexture;
};

