#include "ofxIRGenerator.h"
 
ofxIRGenerator::ofxIRGenerator(){
    deviceInfoChar = NULL;
    deviceInstanceName = NULL;
}

void ofxIRGenerator::generateTexture(){
	xn::IRMetaData imd;
	ir_generator.GetMetaData(imd);	
	const XnGrayscale16Pixel* pImage = imd.Data();

    float size = levelsHigh - levelsLow;
    
    float scale1 = 255.0/1022.0;
    float scale2 = levelsLow*1022.0;
    
//#pragma omp parallel for 
	for(int i=imd.XRes()*imd.YRes()-1;i>=0;i--){
        if(pImage[i] > 255)
            image_pixels[i] = 0;
        else 
            image_pixels[i] = pImage[i];
//        image_pixels[i] = scale1*ofClamp((pImage[i]- scale2)/size, 0,1022);
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
    
    levelsLow = 0.0;
    levelsHigh = 1.0;

    XnStatus result = XN_STATUS_OK;		
    xn::Query query;
    
    if(deviceInstanceName == NULL){
        //The instance name is not set, so lets search for one
        xn::NodeInfoList device_node_info_list;         
        result = pContext->getXnContext().EnumerateProductionTrees(XN_NODE_TYPE_DEVICE, NULL, device_node_info_list); 
        if (result != XN_STATUS_OK) { 
            printf("enumerating depth generators failed. Reason: %s\n", xnGetStatusString (result)); 
            return -1; 
        } else { 
            for (xn::NodeInfoList::Iterator nodeIt =device_node_info_list.Begin(); nodeIt != device_node_info_list.End(); ++nodeIt) { 
                xn::NodeInfo deviceInfo = *nodeIt;
                const xn::NodeInfo& info = *nodeIt; 
                if(deviceInfoChar == NULL || std::strcmp(info.GetCreationInfo(), deviceInfoChar) == 0){
                    result = pContext->getXnContext().CreateProductionTree(deviceInfo);
                    if(result == XN_STATUS_OK){
                        deviceInfoChar = info.GetCreationInfo();
                        query.AddNeededNode(deviceInfo.GetInstanceName());
                        break;
                    }
                }
            } 
        } 
    } else {
        query.AddNeededNode(deviceInstanceName);
    }
    
	//Create image generator
	result = ir_generator.Create(pContext->getXnContext());
	
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


}



ofTexture * ofxIRGenerator::getTexture(){
    return &image_texture;
}