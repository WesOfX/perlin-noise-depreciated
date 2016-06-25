#pragma once

#include <vector>

namespace Perlin{
	/*
	* To use the Generator class, construct an object using either the
	* default constructor or the seed constructor. The default
	* constructor will initialize the permutation vector using Perlin's
	* reference values from his reference Java implementation. Using
	* the seed constructor the permutation vector is generated using a
	* standard library random engine seeded with the provided seed. To
	* get noise values, use the "get" function. To reseed the noise, use the
	* "reseed" function. Examples below.
	* 
	* // Create reference Perlin generator
	* Generator generator();
	* 
	* // Print noise at 55, 99, 0
	* std::cout << generator.get(55, 99, 0) << std::endl;
	* 
	* // Reseed noise with new seed
	* generator.reseed(42);
	* 
	* // Print new noise at 55, 99, 0
	* std::cout << generator.get(55, 99, 0) << std::endl;
	*/
	
	// A reference Perlin noise generator
	class Generator{
		// Permutation vector for pseudo-random numbers
		std::vector<unsigned char> permutation;
		
	public:
		// Creates Perlin noise generator with reference permutation vector
		Generator();
		
		// Createz Perlin noise generator with a custom seed
		Generator(unsigned int seed);
		
		// Returns noise value between -1.0 and 1.0
		double get(double x, double y, double z) const;
		
		// Repopulates permutation vector using new seed
		void reseed(unsigned int seed);
		
	private:
		// Functions for interpolating noise
		static double lerp(double t, double a, double b);
		static double fade(double t);
		static double grad(int hash, double x, double y, double z);
	};
	
	/*  
	* To use the Noise class, construct an object using either the
	* default constructor or the generation constructor. Use the
	* "generate" function to resize and fill the noise array. Use the
	* "get" function to access the noise in the array using a 2D
	* location. Optionally, you can use the generation constructor to
	* immediately call the generate function during construction. 
	* Examples below.
	*  
	* // Generate noise with generation constructor
	* Noise noise(
	* 	800,  // width
	* 	600,  // height
	* 	6.5,  // frequency
	* 	3,    // octaves
	* 	0.75, // persistence
	* 	42    // seed
	* );
	* 
	* // Print noise at 55, 99 using get function
	* std::cout << noise.get(55, 99) << std::endl;
	* 
	* // Generate new noise with generate function
	* noise.generate(
	* 	800,  // width
	* 	600,  // height
	* 	6.5,  // frequency
	* 	3,    // octaves
	* 	0.75, // persistence
	* 	9001  // new seed!
	* );
	* 
	* // Print new noise at 55, 99 using get function again
	* std::cout << noise.get(55, 99) << std::endl;
	*/
	
	// A 2D array of doubles containing noise with easy access methods
	class Noise{
		// 2D Array of doubles containing noise
		std::vector<double> map;
		
		// Dimensions of array
		unsigned short width, height;

	public:
		// Creates empty noise map
		Noise();
		
		// Constructor that resizes map and populates it with noise
		Noise(
			unsigned short width,  // Number of collums
			unsigned short height, // Number of rows
			double frequency,      // Frequency of noise features
			unsigned int octaves,  // Number of octaves (halving frequency)
			double persistence,    // Compounding weight of octaves
			unsigned int seed      // Seed for perlin noise engine
		);
		
		// Returns a double from the map at the desired location
		// Values returned are between -1.0 and 1.0
		// If the location does not exst, 0.0 is returned
		double get(unsigned short x, unsigned short y) const;
		
		// Resizes the noise map and populates it with noise
		void generate(
			unsigned short width,  // Number of collums
			unsigned short height, // Number of rows
			double frequency,      // Frequency of noise features
			unsigned int octaves,  // Number of octaves (halving frequency)
			double persistence,    // Compounding weight of octaves
			unsigned int seed      // Seed for perlin noise
		);
		
	private:
		// Sets the double at the provided location to the value provided
		void set(unsigned short x, unsigned short y,  double value);
		
		// Resized the double array (Invalidates current data)
		void resize(unsigned short width, unsigned short height);
	};
};
