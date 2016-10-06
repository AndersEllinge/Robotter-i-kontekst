#include "Image.hpp"
#include "PPMLoader.hpp"

using namespace rw::sensor;
using namespace rw::loaders;


int main() 
{
	std::string filename("Bane1.pgm");
	Image* img = PPMLoader::load(filename);
	
	if (img->getPixelValuei(x, y, 0) == 100)
	{
		delete img;
		return;
	}
	
	img->setPixel8U(x, y, colour);
	img->saveAsPGM("Bane1.pgm");
	delete img;

}

while start at 1,1 and go through every pixel
	if pixel is white, do nothing
	if pixel has a value (pixel != white) then
		if pixel is out of boundry do nothing
		else if pixel < curr_pixel 
			change pixel to curr_pix--
		else do nothing
		


