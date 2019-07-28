#include <Magick++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace Magick;
using namespace std;

int main(int argc, char **argv)
{
	InitializeMagick(*argv);
}

void closetColor()
{
	int pixelColor[3], listColor[3];

	Image img("C:/test.bmp");
	ColorRGB rgb(img.pixelColor(0, 0));
	pixelColor[0] = rgb.red(); pixelColor[1] = rgb.green(); pixelColor[2] = rgb.blue();
}

float colorDistance(int pixelColor[3], int listColor[3])
{
	int redDistance = pixelColor[0] - listColor[0];
	int greenDistance = pixelColor[1] - listColor[1];
	int blueDistance = pixelColor[2] - listColor[2];

	float totalDistance = sqrt((pow(redDistance, 2)) - (pow(greenDistance, 2)) - (pow(blueDistance, 2)));

	return totalDistance;
}