#pragma once

#include <vector>
#include <string>

namespace PPM{
	// A 24-bit color
	struct Color{
		// Red, green, and blue values
		unsigned char r, g, b;
		
		// Constructs a color with 0 red, 0 green, and 0 blue (black)
		Color();
		
		// Construct a color out of red, green, and blue values
		Color(unsigned char r, unsigned char g, unsigned char b);		
	};
	
	/*
	* To use this class, construct an object using either the default
	* constructor or the reading constructor. Use the "write" function
	* to write pixel data to a ppm image file. Use the "read" function
	* to read pixel data from a ppm image file and resize the pixel array
	* appropriately. Examples below.
	* 
	* // Load image using read constructor
	* PPM::Image image("Cat.ppm");
	* 
	* // Set the pixel at 55, 99 to magenta
	* image.set(55, 99, PPM::Color(255, 0, 255));
	* 
	* // Write image to new file
	* image.write("New Cat.ppm");
	*/
	
	// An in-memory ppm image that can read or write ppm image files
	class Image{
		// 24-bit image data
		std::vector<Color> pixels;
		
		// Dimensions of image
		unsigned short width, height;
	
	public:
		// Constructs an empty image with 0 width and 0 height
		Image();
		
		// Constructs an image from a file
		// Image size is automatically determined
		Image(const std::string& filename);
		
		// Constructs an image of the provided size
		Image(unsigned short width, unsigned short height);
	
		// Returns color of pixel at provided location
		Color get(unsigned short x, unsigned short y) const;
		
		// Sets pixel at provided location to provided color
		void set(unsigned short x, unsigned short y, Color color);
		
		// Resizes the pixel array (Does not preserve image)
		void resize(unsigned short width, unsigned short height);
		
		// Loads ppm image from provided file location
		// Returns true if successfull
		bool read(const std::string& filename);
		
		// Creates ppm image file at provided location
		// Returns true if successfull
		bool write(const std::string& filename) const;
		
		// Returns estimated filesize in bytes
		unsigned long filesize() const;
	};
};
