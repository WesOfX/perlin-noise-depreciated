#include "PPM.hpp"

#include <fstream>

// Constructs color with 0 red, 0 green, and 0 blue (black)
PPM::Color::Color(): r(0), g(0), b(0){}

// Construct color out of red, green, and blue values
PPM::Color::Color(unsigned char r, unsigned char g, unsigned char b):
	r(r), g(g), b(b){}

// Constructs an empty image with 0 width and 0 height
PPM::Image::Image(){}

// Constructs an image from a file
// Image size is automatically determined
PPM::Image::Image(const std::string& filename){
	read(filename);
}

// Constructs an image of the provided size
PPM::Image::Image(unsigned short width, unsigned short height){
	resize(width, height);
}

// Returns color of pixel at provided location
PPM::Color PPM::Image::get(unsigned short x, unsigned short y) const{
	// Prevent segmentation fault by returning a default color
	if(x >= width || y >= height) return Color(0, 0, 0);
	
	return pixels[(unsigned int)y * width + x];
}

// Sets pixel at provided location to provided color
void PPM::Image::set(unsigned short x, unsigned short y, Color color){
	// Prevent segmentation fault by not doing anything
	if(x >= width || y >= height) return;
	
	pixels[(unsigned int)y * width + x] = color;
}

// Resizes the pixel array (Does not preserve image)
void PPM::Image::resize(unsigned short width, unsigned short height){
	this->width = width;
	this->height = height;
	
	// Does not preserve pixel locations
	pixels.resize((unsigned int)width * height);
}

// Loads ppm image from provided file location
// Returns true if successfull
bool PPM::Image::read(const std::string& filename){
	// File to be loaded
	std::ifstream file(filename);
		
	// String to load file data into
	std::string str;
		
	// Skip magick number (P6)
	file >> str;
	
	// Get width
	file >> str;
	unsigned short collums = std::stoi(str);
	
	// Get height
	file >> str;
	unsigned short rows = std::stoi(str);
	
	// Skip trailing space
	file >> str;
	file.get();
	
	// Resize pixel array
	resize(collums, rows);
	
	// Load pixel data
	for(unsigned short y = 0; y < height; y++){
		for(unsigned short x = 0; x < width; x++){
			set(x, y, Color(file.get(), file.get(), file.get()));
		}
	}
	
	if(!file) return false;
	return true;
}

// Creates ppm image file at provided location
// Returns true if successfull
bool PPM::Image::write(const std::string& filename) const{
	std::ofstream file(filename);
	
	//Write PPM header
	file << "P6\n" // 24 bit color in binary
	     << std::to_string(width)
	     << ' ' 
	     << std::to_string(height) 
	     << std::endl 
	     << "255\n"; // 24 bit

	//Write PPM data
	for(unsigned int i = 0; i < (unsigned int)width * height; i++){
		file << pixels[i].r << pixels[i].g << pixels[i].b;
	}
	
	file.close();
	
	//Return false if anything
	if(!file) return false;
	return true;
}

// Returns estimated filesize in bytes
unsigned long PPM::Image::filesize() const{
	return width * height * 3;
}
