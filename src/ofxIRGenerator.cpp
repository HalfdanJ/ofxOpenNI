#include "ofxIRGenerator.h"
 
void ofxIRGenerator::generateTexture(){
	xn::IRMetaData imd;
	ir_generator.GetMetaData(imd);	
	const XnGrayscale16Pixel* pImage = imd.Data();

    float size = levelsHigh - levelsLow;
    
	for(int i=0;i<imd.XRes()*imd.YRes();i++){
        image_pixels[i] = 255.0*ofClamp((pImage[i]- levelsLow*1022)/size, 0,1022)/1022.0;
    }
	image_texture.loadData(image_pixels,imd.XRes(), imd.YRes(), GL_LUMINANCE);		
}

void ofxIRGenerator::draw(float x, float y, float w, float h){
	generateTexture();
	glColor3f(1,1,1);
	image_texture.draw(x, y, w, h);		
}

bool ofxIRGenerator::setup(ofxOpenNIContext* pContext) {
	if(!pContext->isInitialized()) {
		return false;
	}
	
	//Create image generator
	XnStatus result = ir_generator.Create(pContext->getXnContext());
	
	if (result != XN_STATUS_OK){
		printf("Setup Image Camera failed: %s\n", xnGetStatusString(result));
		return false;
	} else {
		ofLog(OF_LOG_VERBOSE, "Create Image Generator inited");
		
		XnMapOutputMode map_mode; 
		map_mode.nXRes = XN_VGA_X_RES; 
		map_mode.nYRes = XN_VGA_Y_RES; 
		map_mode.nFPS = 30;
		
		result = ir_generator.SetMapOutputMode(map_mode);
		
		image_texture.allocate(map_mode.nXRes, map_mode.nYRes, GL_LUMINANCE);		
		image_pixels = new unsigned char[map_mode.nXRes * map_mode.nYRes];
		
		ir_generator.StartGenerating();		
		return true;
	}		

    levelsLow = 0.0;
    levelsHigh = 1.0;
}

xn::IRGenerator& ofxIRGenerator::getXnIRGenerator(){
	return ir_generator;
}

ofTexture * ofxIRGenerator::getTexture(){
    return &image_texture;
}