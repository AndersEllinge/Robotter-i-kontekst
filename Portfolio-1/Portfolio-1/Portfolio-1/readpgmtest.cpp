#include <iostream>
#include "Image.hpp"
#include "PPMLoader.hpp"
#include "Dd_robot.h"

#define PI 3.14159265

using namespace rw::sensor;
using namespace rw::loaders;

int main()
{
	Dd_robot lort(1, 0, 0, 1, 0, 0);
	
	lort.move(cos(30*PI/180), sin(30*PI/180), 0-sin(30*PI/180), cos(30*PI/180), 10, 0);

	lort.move(cos(60*PI/180), sin(60*PI/180), 0-sin(60*PI/180), cos(60*PI/180), 0, 10);
	
	lort.whereAreYou();
	
	
}




//int main(int argc, char** argv) {
//    //std::string filename(argv[1]);
//	std::string filename("Bane1.pgm");
//    std::cout << "loading image..." << std::endl;
//    Image* img = PPMLoader::load(filename);
//
//    // do stuff here
//    int channel = 0; // allways 0 on grayscale image
//
//    std::cout << "inverting image..." << std::endl;
//    // change all 0 pixels to 255 and all 255 pixels to 0
//    for(int x=0;x<img->getWidth();x++){
//        for(int y=0;y<img->getHeight();y++){
//            int val = img->getPixelValuei( x, y, channel );
//            if(val==0){
//                img->setPixel8U(x,y,255);
//            } else if( val==255 ) {
//                img->setPixel8U(x,y,0);
//            }
//        }
//    }
//
//    std::cout << "saving image..." << std::endl;
//    // save image
//    img->saveAsPGM("testout.pgm");
//        
//    // cleanup
//    delete img;
//}
