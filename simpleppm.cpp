#include <iostream>
#include <fstream>
using namespace std;

class PPM {
	public:
		int width;
		int height;
		int maxColor;

		// allows you to initialize when defining object without making things more complicated than they already are (i.e. not defining a bunch of
		// getters and setters and then using out of scope constructor functions that call said getters and setters to define params) 
		PPM(int width, int height, int maxColor) : width(width), height(height), maxColor(maxColor) {};

		void writeAltPxFile();
		void writeGradFile(string fileName);
};

// using the set params, create a simple ppm (alternates color in each px), output to stdout so redirect to .ppm file
void PPM::writeAltPxFile() {
	// define the "magic number" of the ppm file (see wikipedia)
	cout << "P3\n";

	// second line in ppm is num of columns (width) and num of rows (height) 
	cout << width << ' ' << height << "\n";

	// third line in ppm is max color value
	cout << maxColor << "\n";

	// create rgb vals for each column in each row (hacky loops to alternate red and blue)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i % 2 == 0) {
				if (j % 2 == 0) {
					cout << "255 0 0\t";
				} else {
					cout << "0 0 255\t";
				}
			} else {
				if (j % 2 == 1) {
					cout << "255 0 0\t";
				} else {
					cout << "0 0 255\t";
				}
			}
		}
		cout << "\n";
	}
}

// writes ppm that has a simple gradient background
void PPM::writeGradFile(string fileName) {
	// initialize file stream to write to 
	ofstream outFile(fileName);

	// define the "magic number" of the ppm file (see wikipedia)
	outFile << "P3\n";

	// second line in ppm is num of columns (width) and num of rows (height) 
	outFile << width << ' ' << height << "\n";

	// third line in ppm is max color value
	outFile << maxColor << "\n";

	int r;
	int g = 0;
	int b;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// scale based on dimensions and max color value
			r = (int) j / (float) width * maxColor;
			b = (int) i / (float) height * maxColor;
			outFile << r << ' ' << g << ' ' << b << "\t"; 
		}
		outFile << "\n";
	}

	outFile.close();
}

int main() {
	int width = 1920;  // columns
	int height = 1080;  // rows
	int maxColor = 255;
	string fileName = "gradient.ppm";

	PPM mypp(width, height, maxColor);
	mypp.writeGradFile(fileName);
	
	return 0;
}
