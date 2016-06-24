#include "Perlin.hpp"

#include <algorithm>

// Creates perlin noise generator with reference permutation vector
Perlin::Generator::Generator(){	
	// Initialize permutation with the Ken Perlin's reference values
	permutation = {
		151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
		7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
		203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
		20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27,
		166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
		220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
		63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
		200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173,
		186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118,
		126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 
		189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163,
		70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
		98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 
		97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
		241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
		199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150,
		254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128,
		195, 78, 66, 215, 61, 156, 180
	};
	
	// Duplicate permutation
	permutation.insert(
		permutation.end(),
		permutation.begin(),
		permutation.end()
	);
}

// Createz perlin noise generator with a custom seed
Perlin::Generator::Generator(unsigned int seed){
	permutation.resize(256);

	// Fill permutation with 0 to 255
	std::iota(permutation.begin(), permutation.end(), 0);

	// Shuffle permutation with seed
	std::default_random_engine engine(seed);
	std::shuffle(permutation.begin(), permutation.end(), engine);

	// Duplicate permutation
	permutation.insert(
		permutation.end(),
		permutation.begin(),
		permutation.end()
	);
}

// Returns noise value between -1.0 and 1.0
double Perlin::Generator::get(double x, double y, double z) const{
	// Find the unit cube that contains the point
	int X = (int) floor(x) & 255,
	    Y = (int) floor(y) & 255,
	    Z = (int) floor(z) & 255;

	// Find the offset of the point in the cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute the fade curves
	double u = fade(x), 
	       v = fade(y),
	       w = fade(z);

	// Hash coordinates of the 8 cube corners
	int A = permutation[X] + Y,
	    AA = permutation[A] + Z,
	    AB = permutation[A + 1] + Z,
	    B = permutation[X + 1] + Y,
	    BA = permutation[B] + Z,
	    BB = permutation[B + 1] + Z;

	// Blend the 8 corners of cube
	return lerp(
		w,
		lerp(
			v,
			lerp(
				u,
				grad(permutation[AA], x, y, z),
				grad(permutation[BA], x-1, y, z)
			),
			lerp(
				u,
				grad(permutation[AB], x, y-1, z),
				grad(permutation[BB], x-1, y-1, z)
			)
		),
		lerp(
			v,
			lerp(
				u,
				grad(permutation[AA+1], x, y,	z-1),
				grad(permutation[BA+1], x-1, y, z-1)
			),
			lerp(
				u,
				grad(permutation[AB+1], x, y-1, z-1),
				grad(permutation[BB+1], x-1, y-1, z-1)
			)
		)
	);
}

double Perlin::Generator::fade(double t){ 
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double Perlin::Generator::lerp(double t, double a, double b){ 
	return a + t * (b - a); 
}

double Perlin::Generator::grad(int hash, double x, double y, double z){
	int h = hash & 15;
	
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y,
		   v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// Creates empty noise map
Perlin::Noise::Noise(): width(0), height(0){}

// Constructor that resizes map and populates it with noise
Perlin::Noise::Noise(
	unsigned short width,  // Number of collums
	unsigned short height, // Number of rows
	double frequency,      // Frequency of noise features
	unsigned int octaves,  // Number of octaves (halving frequency)
	double persistence,    // Compounding weight of octaves
	unsigned int seed      // Seed for perlin noise engine
){
	generate(width, height, frequency, octaves, persistence, seed);
}

// Returns a double from the may at the desired location
// Values returned are between -1.0 and 1.0
// If the location does not exst, 0.0 is returned
double Perlin::Noise::get(unsigned short x, unsigned short y) const{
	// Prevent a segmentation fault by returning a default value
	if(x >= width || y >= height) return 0.0;
	
	return map[(unsigned int)y * width + x];
}

// Sets the double at the provided location to the value provided
void Perlin::Noise::set(unsigned short x, unsigned short y, double value){
	// Prevent a segmentation fault by doing nothing
	if(x >= width || y >= height) return;
	
	map[(unsigned int)y * width + x] = value;
}

// Resized the double array (Invalidates current data)
void Perlin::Noise::resize(unsigned short width, unsigned short height){
	this->width = width;
	this->height = height;
	
	// Does not preserve map-element locations
	map.resize((unsigned int)width * height);
}

// Resizes the noise map and populates it with noise
void Perlin::Noise::generate(
	unsigned short width,  // Number of collums
	unsigned short height, // Number of rows
	double frequency,      // Frequency of noise features
	unsigned int octaves,  // Number of octaves (halving frequency)
	double persistence,    // Compounding weight of octaves
	unsigned int seed      // Seed for perlin noise engine
){
	// Resize the noise map
	// All data will be overwritten
	resize(width, height);
	
	// Prevent divide-by-0
	if(octaves == 0) return;
	
	// Perlin noise engine for generating noise
	Perlin::Generator perlin(seed);
	
	for(unsigned short y = 0; y < width; y++){
		for(unsigned short x = 0; x < height; x++){
			// Final noise value
			double value = 0.0;
			
			// Number perlin noise octaves are multiplied by
			double amplitude = 1.0;
			
			// Average all octaves
			for(unsigned int i = 0; i < octaves; i++){
				value += amplitude * perlin.get(
					x * frequency * pow(2, i) / width,
					y * frequency * pow(2, i) / height,
					i // Every octave uses a different layer in 3D
				);
				
				// Apply persistence for next octave
				amplitude *= persistence;				
			}
			
			// Clamp value within -1.0 to 1.0 range
			value = std::max(-1.0, std::min(1.0, value));
			
			set(x, y, value);
		}
	}
}
