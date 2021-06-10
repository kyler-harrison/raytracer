#include <iostream>
#include <fstream>
#include "vec3d.h"
#include "sphere.h"
#include "light.h"

/*
void createPPM(int imageHeight, int imageWidth, double *pxMatrix, std::string filePath) {
	std::ofstream outFile(filePath);

	// define the "magic number" of the ppm file (see wikipedia)
	outFile << "P3\n";

	// second line in ppm is num of columns (width) and num of rows (height) 
	outFile << imageWidth << ' ' << imageHeight << "\n";

	// third line in ppm is max color value
	int maxColor = 255;
	outFile << maxColor << "\n";

	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			if (*((pxMatrix + i * imageWidth) + j)) {
				outFile << 255 << ' ' << 0 << ' ' << 0 << "\t";
			} else {
				outFile << 255 << ' ' << 255 << ' ' << 255 << "\t";
			}
			//std::cout << *((pxMatrix + i * imageWidth) + j) << "\n";
		}
		outFile << "\n";
		//std::cout << "\n";
	}

	outFile.close();
}
*/

// write ppm file given matrix of color objects
void createPPM(int imageHeight, int imageWidth, color *pxMatrix, std::string filePath) {
	std::ofstream outFile(filePath);

	// define the "magic number" of the ppm file (see wikipedia)
	outFile << "P3\n";

	// second line in ppm is num of columns (width) and num of rows (height) 
	outFile << imageWidth << ' ' << imageHeight << "\n";

	// third line in ppm is max color value
	int maxColor = 255;
	outFile << maxColor << "\n";

	// write px rgb values
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			color val = *((pxMatrix + i * imageWidth) + j);
			outFile << val.x << ' ' << val.y << ' ' << val.z << '\t';
		}
		outFile << "\n";
	}

	outFile.close();
}

int main() {
	// image dimensions
	double aspectRatio = 16.0 / 9.0;
	// NOTE be weary, I get a segfault when this is 800 (running out of memory?)
	int imageWidth = 1920;
	int imageHeight = (int) imageWidth / aspectRatio;

	// create sphere object
	double radius = 2.5;
	point3d sphereCenter(0, 0, -5);
	// TODO make these vals reasonable
	double ks = 0.5;
	color ia(255, 0, 0);
	double kd = 0.35;  
	double ka = 0.3;
	double alpha = 0.22;
	sphere sceneSphere(ks, ia, kd, ka, alpha, sphereCenter, radius);

	// NOTE when you do Phong calc, could split into three calcs, one for each channel of RGB (different constants based on channel)
	// define single light source
	point3d lightPosition(-2, -5, 2);
	color lightSpecInten(255, 255, 255);
	color lightDiffInten(10, 10, 10);
	light lightSource(lightPosition, lightSpecInten, lightDiffInten);

  // "eye" (camera) point at origin of xyz
	point3d eyePoint(0, 0, 0);
	
	// target point (used to determine center of plane and its orientation)
	point3d targetPoint(0, 0, -3);
	
	// vector from eyePoint to targetPoint
	vec3d targetVec = targetPoint.subtract(eyePoint);

	// normalize targetVec
	vec3d targetVecNorm = targetVec.normalize();

	// viewport sizes 
	double viewportHeight = 2.0;
	double viewportWidth = viewportHeight * aspectRatio;
	double focalLength = 1.0;

	// px shift vectors
	vec3d horizontal = vec3d(viewportWidth, 0, 0);
	vec3d vertical = vec3d(0, viewportHeight, 0);
	vec3d focalVec = vec3d(0, 0, focalLength);

	// bottom left px (increment using px shift vectors to make view plane)
	vec3d bottomLeftCorner = eyePoint.subtract(horizontal.scalarDiv(2)).subtract(vertical.scalarDiv(2)).subtract(focalVec);

	// create ppm file stream
	std::string filePath = "sphereCheck.ppm";
	std::ofstream outFile(filePath);
	
	// matrix of color objects for each px
	//color imageMatrix[imageHeight][imageWidth];

	// define the "magic number" of the ppm file (see wikipedia)
	outFile << "P3\n";

	// second line in ppm is num of columns (width) and num of rows (height) 
	outFile << imageWidth << ' ' << imageHeight << "\n";

	// third line in ppm is max color value
	int maxColor = 255;
	outFile << maxColor << "\n";

	// iterate through all pixels, finding the normalized ray of each and checking if it intersects with sphere
	for (int i = 0; i < imageHeight; i++) {
		for (int j = 0; j < imageWidth; j++) {
			double u = double(j) / (imageWidth - 1);
			double v = double(i) / (imageHeight - 1);
			vec3d pxRay = eyePoint.add(bottomLeftCorner).add(horizontal.scalarMul(u)).add(vertical.scalarMul(v));

			// normalize this ray (u), then solve ray = eye + ud for d
			// o is eye, c is sphereCenter, u is pxRayNorm
			vec3d pxRayNorm = pxRay.normalize();
				
			// the thing underneath the sqrt in quad formula
			double nabla = pow(pxRayNorm.dot(eyePoint.subtract(sphereCenter)), 2) - (pow(eyePoint.subtract(sphereCenter).magnitude(), 2) - pow(radius, 2));

			// check for intersection(s)
			if (nabla >= 0) {
				// if nabla == 0 one intersection, > 0 two intersections
				// NOTE negative val here means something else (not there yet, so shouldn't happen), also if nabla is 0 this inefficient but idfm
				double distanceSum = pxRayNorm.dot(eyePoint.subtract(sceneSphere.center)) * -1 + pow(nabla, 0.5);
				double distanceDiff = pxRayNorm.dot(eyePoint.subtract(sceneSphere.center)) * -1 - pow(nabla, 0.5);

				// take minimum as distance (closest intersection)
				double distance = distanceSum < distanceDiff ? distanceSum : distanceDiff;

				// get 3d point of intersection
				point3d intersection = eyePoint.add(pxRayNorm.scalarMul(distance));

				// for the following vectors, refer to Phong model wikipedia

				// vector from intersection to light source (L_hat_m)
				vec3d lightDirVec = lightSource.position.subtract(intersection).normalize();

				// normal at point of intersection on surface (N_hat)
				vec3d surfaceNormal = intersection.subtract(sceneSphere.center).normalize();  

				// perfectly reflected ray of light from intersection (R_hat_m)
				vec3d reflectVec = surfaceNormal.scalarMul(lightDirVec.dot(surfaceNormal) * 2).subtract(surfaceNormal);  // already normalized i think

				// vec in direction of viewer from intersection (V_hat)
				vec3d viewerVec = eyePoint.subtract(intersection).normalize();
				//vec3d viewerVec = intersection.normalize();  // TODO is this right? feel like it should be the above

				//color illumination = sceneSphere.ia.scalarMul(sceneSphere.ka).add(lightSource.id.scalarMul(lightDirVec.dot(surfaceNormal) * sceneSphere.kd)).add(lightSource.is.scalarMul(pow(reflectVec.dot(viewerVec), sceneSphere.alpha) * sceneSphere.ks));

				// implement illumination in three terms, you get nan if you don't do checks on the signs
				color term0 = sceneSphere.ia.scalarMul(sceneSphere.ka);
				// NOTE these terms would be in summation if multiple lights
				double term1dot = lightDirVec.dot(surfaceNormal);

				// based on the wikipedia it should be like this, but doing them separate looks more right
				/*
				color term1;
				color term2;
				if (term1dot > 0) {
					term1 = term1dot > 0 ? lightSource.id.scalarMul(sceneSphere.kd * term1dot) : color(0.0, 0.0, 0.0);
					double term2dot = reflectVec.dot(viewerVec);
					term2 = term2dot > 0 ? lightSource.is.scalarMul(sceneSphere.ks * pow(term2dot, sceneSphere.alpha)) : color(0.0, 0.0, 0.0);
				} else {
					term1 = color(0.0, 0.0, 0.0);
					//term2 = color(0.0, 0.0, 0.0);
				}
				*/

				color term1 = term1dot > 0 ? lightSource.id.scalarMul(sceneSphere.kd * term1dot) : color(0.0, 0.0, 0.0);
				double term2dot = reflectVec.dot(viewerVec);
				color term2 = term2dot > 0 ? lightSource.is.scalarMul(sceneSphere.ks * pow(term2dot, sceneSphere.alpha)) : color(0.0, 0.0, 0.0);

				// what all this is for
				color illumination = term0.add(term1).add(term2);

				// write to file stream
				outFile << illumination.x << ' ' << illumination.y << ' ' << illumination.z << '\t';

			} else {
				// no intersection, black background
				outFile << 0.0 << ' ' << 0.0 << ' ' << 0.0 << '\t';
			}
		}
		outFile << "\n";
	}

	outFile.close();

	return 0;
}
