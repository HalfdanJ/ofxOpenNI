#include "ofxImageGenerator.h"
 
ofxImageGenerator::ofxImageGenerator(){
    deviceInfoChar = nil;
    connected = false;
}


void ofxImageGenerator::generateTexture(){
    if(connected){
	xn::ImageMetaData imd;
	image_generator.GetMetaData(imd);	
	const XnUInt8* pImage = imd.Data();
	memcpy(image_pixels, pImage, sizeof(unsigned char) * imd.XRes() * imd.YRes() * 3);
	image_texture.loadData(image_pixels,imd.XRes(), imd.YRes(), GL_RGB);		
    }
}

void ofxImageGenerator::draw(float x, float y, float w, float h){
    if(connected){
	generateTexture();
	glColor3f(1,1,1);
	image_texture.draw(x, y, w, h);		
    }
}

bool ofxImageGenerator::setup(ofxOpenNIContext* pContext) {
    connected = false;
	if(!pContext->isInitialized()) {
		return false;
	}
    
    XnStatus result = XN_STATUS_OK;		
    xn::Query query;
    
    //if(deviceInstanceName == NULL){
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
                       // deviceInfoChar = info.GetCreationInfo();
                        query.AddNeededNode(deviceInfo.GetInstanceName());
                        break;
                    }
                }
            } 
        } 
   /* } else {
        query.AddNeededNode(deviceInstanceName);
    }
	*/
    
	//Create image generator
	result = image_generator.Create(pContext->getXnContext(), &query);
	
	if (result != XN_STATUS_OK){
		printf("Setup Image Camera failed: %s\n", xnGetStatusString(result));
		return false;
	} else {
		ofLog(OF_LOG_VERBOSE, "Create Image Generator inited");
		
		XnMapOutputMode map_mode; 
		map_mode.nXRes = XN_VGA_X_RES; 
		map_mode.nYRes = XN_VGA_Y_RES; 
		map_mode.nFPS = 30;
		
		result = image_generator.SetMapOutputMode(map_mode);
		
		image_texture.allocate(map_mode.nXRes, map_mode.nYRes, GL_RGBA);		
		image_pixels = new unsigned char[map_mode.nXRes * map_mode.nYRes * 3];
		
		image_generator.StartGenerating();		
        connected = true;
		return true;
	}		
}

xn::ImageGenerator& ofxImageGenerator::getXnImageGenerator(){
	return image_generator;
}
