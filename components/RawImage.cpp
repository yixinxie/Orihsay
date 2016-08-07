#include "RawImage.h"

#include "../gameplay/GameObject.h"
#include "../gameplay/G.h"
//using namespace lodepng;
void RawImage::awake(void){
	selfRectTransform = gameObject->rectTransform();
}
void RawImage::loadPNG(const IntVector2& _dim, const char* fileName){
	textureDim = _dim;
	std::vector<unsigned char> pngData; //the raw pixels
	unsigned int width = textureDim.x;
	unsigned int height = textureDim.y;
	unsigned int error = lodepng::load_file(pngData, fileName);
	assert(error == 0);
	error = lodepng::decode(image, width, height, pngData);
	assert(error == 0);
	imageBuffer = image.data();
	// create texture in GPU.
	resourceHandle = G::instance()->renderer->createTexture(width, height);
	assert(resourceHandle >= 0);
}