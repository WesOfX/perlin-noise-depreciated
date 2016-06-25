# Perlin Noise

A small library for easily creating perlin noise. Strongly based on Ken Perlin's Improved Noise from 2002.

## Perlin::Generator

To use the Generator class, construct an object using either the
default constructor or the seed constructor. The default
constructor will initialize the permutation vector using Perlin's
reference values from his reference Java implementation. Using
the seed constructor the permutation vector is generated using a
standard library random engine seeded with the provided seed. To
get noise values, use the "get" function. Examples below.
```
// Create reference Perlin generator
Generator generator();

// Print noise at 55, 99, 0
std::cout << generator.get(55, 99, 0) << std::endl;

// Create new noise with new seed
generator = Generator(42);

// Print new noise at 55, 99, 0
std::cout << generator.get(55, 99, 0) << std::endl;
```
## Perlin::Noise

To use the Noise class, construct an object using either the
default constructor or the generation constructor. Use the
"generate" function to resize and fill the noise array. Use the
"get" function to access the noise in the array using a 2D
location. Optionally, you can use the generation constructor to
immediately call the generate function during construction. 
Examples below.
```
// Generate noise with generation constructor
Noise noise(
	800,  // width
	600,  // height
	6.5,  // frequency
	3,    // octaves
	0.75, // persistence
	42    // seed
);

// Print noise at 55, 99 using get function
std::cout << noise.get(55, 99) << std::endl;

// Generate new noise with generate function
noise.generate(
	800,  // width
	600,  // height
	6.5,  // frequency
	3,    // octaves
	0.75, // persistence
	9001  // new seed!
);

// Print new noise at 55, 99 using get function again
std::cout << noise.get(55, 99) << std::endl;
```
