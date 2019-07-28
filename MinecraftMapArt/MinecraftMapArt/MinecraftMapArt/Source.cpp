#include <Magick++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace Magick;
using namespace std;

void closetColor(int imageSize, ColorChoice * allColors, int totalColors);
float colorDistance(int pixelColor[3], int listColor[3]);
float colorDistance(int pixelColor[3], int listColor[3]);
void setColorChoices(ColorChoice * allColors);


struct ColorChoice
{
	int rgb[3];
	string block;
	string * possibleBlocks;
};

int main(int argc, char **argv)
{
	InitializeMagick(*argv);
	int imageSize = 128;
    ColorChoice * allColors;
	int totalColors = 2;
	allColors = new ColorChoice[totalColors]();
	setColorChoices(allColors);
	closestColor(imageSize, allColors,totalColors);
}

void closestColor(int imageSize, ColorChoice * allColors, int totalColors)
{
	int pixelColor[3], listColor[3];

	Image img("test.bmp");
	
	for (int i = 0; i < imageSize; i++)
	{
		for (int j = 0; j < imageSize; j++)
		{
			try
			{
				ColorRGB rgb(img.pixelColor(0, 0));
				pixelColor[0] = rgb.red(); pixelColor[1] = rgb.green(); pixelColor[2] = rgb.blue();
				int k = 0, currentClosest;
				float closestDistance, currentDistance;

				closestDistance = colorDistance(pixelColor, allColors[k].rgb);
				currentClosest = k;
				k++;

				for (k; k < totalColors; k++)
				{
					currentDistance = colorDistance(pixelColor, allColors[k].rgb);
					if (currentDistance < closestDistance)
					{
						closestDistance = currentDistance;
						currentClosest = k;
					}
				}

				convert / img - fill yellow - draw 'color i, j point' /Users/anders/Desktop/teest3.png
			}
			catch (Magick::Exception & error)
			{
				cerr << "Caught Magick++ exception: " << error.what() << endl;
			}
		}
	}
}

float colorDistance(int pixelColor[3], int listColor[3])
{
	int redDistance = pixelColor[0] - listColor[0];
	int greenDistance = pixelColor[1] - listColor[1];
	int blueDistance = pixelColor[2] - listColor[2];

	float totalDistance = sqrt((pow(redDistance, 2)) - (pow(greenDistance, 2)) - (pow(blueDistance, 2)));

	return totalDistance;
}

void setColorChoices(ColorChoice * allColors)
{
	allColors[0].rgb[0] = 127; allColors[0].rgb[1] = 178; allColors[0].rgb[2] = 56;
	allColors[0].block = "Green";

	allColors[0].rgb[0] = 247; allColors[0].rgb[1] = 233; allColors[0].rgb[2] = 163;
	allColors[0].block = "Tan?";
}