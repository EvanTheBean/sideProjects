//https://fortunomedia.com/magick-plus-plus-config-vs2010/

#include <Magick++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace Magick;
using namespace std;

struct ColorChoice
{
	int rgb[3];
	string block;
	string * possibleBlocks;
};

struct Pixel
{
	int rgb[3];
};

void closestColor(int imageSize, ColorChoice * allColors, int totalColors, Pixel ** finalImage, Pixel ** originalImage,int * blocksToUse);
float colorDistance(int pixelColor[3], int otherColor,ColorChoice * allColors);
void setColorChoices(ColorChoice * allColors);
void outputImage(int imageSize, Pixel ** finalImage, const string fileName);
void countBlocks(int * blocksToUse, int totalColors, ColorChoice * allColors);

int main(int argc, char **argv)
{
	InitializeMagick(*argv);
    
	ColorChoice * allColors;
	int totalColors = 51;
	int *blocksToUse;
	blocksToUse = new int[totalColors];
	for (int i = 0; i < totalColors; i++)
	{
		blocksToUse[i] = 0;
	}

	int imageSize = 128;
	Pixel ** finalImage;
	finalImage = new Pixel*[imageSize];
	for (int i = 0; i < imageSize; i++)
	{
		finalImage[i] = new Pixel[imageSize];
	}

	Pixel ** originalImage;
	originalImage = new Pixel*[imageSize];
	for (int i = 0; i < imageSize; i++)
	{
		originalImage[i] = new Pixel[imageSize];
	}

	allColors = new ColorChoice[totalColors];
	setColorChoices(allColors);
	closestColor(imageSize, allColors, totalColors, finalImage,originalImage,blocksToUse);
	outputImage(imageSize,finalImage,"testResult.png");
	outputImage(imageSize, originalImage,"originalImage.png");
	countBlocks(blocksToUse,totalColors,allColors);

	string temp;
	cin >> temp;
}

void closestColor(int imageSize, ColorChoice * allColors, int totalColors, Pixel ** finalImage, Pixel ** originalImage,int * blocksToUse)
{
	int pColor[3], listColor[3];

	Image img("test.png");

	ColorRGB rgb;
	
	for (int i = 0; i < imageSize; i++)
	{
		for (int j = 0; j < imageSize; j++)
		{
			try
			{
				rgb = img.pixelColor(i, j);
				pColor[0] = rgb.red()*257; pColor[1] = rgb.green() * 257; pColor[2] = rgb.blue() * 257;
				//cout << "pixel # " << i << " " << j << endl;
				//cout << pColor[0] << ", " << pColor[1] << ", " << pColor[2] << endl;
				originalImage[i][j].rgb[0] = pColor[0]; originalImage[i][j].rgb[1] = pColor[1]; originalImage[i][j].rgb[2] = pColor[2];
				
				int k = 0, currentClosest;
				float closestDistance, currentDistance;

				closestDistance = colorDistance(pColor, k, allColors);
				currentClosest = k;
				k++;

				for (k; k < totalColors; k++)
				{
					if (allColors[k].block != "NONE")
					{
						currentDistance = colorDistance(pColor, k, allColors);
						if (currentDistance < closestDistance)
						{
							closestDistance = currentDistance;
							currentClosest = k;
						}
					}
				}

				finalImage[i][j].rgb[0] = allColors[currentClosest].rgb[0];
				finalImage[i][j].rgb[1] = allColors[currentClosest].rgb[1];
				finalImage[i][j].rgb[2] = allColors[currentClosest].rgb[2];

				blocksToUse[currentClosest] += 1;
			}
			catch (Magick::Exception & error)
			{
				cerr << "Caught Magick++ exception: " << error.what() << endl;
			}
		}
	}
}

float colorDistance(int pixelColor[3], int otherColor, ColorChoice * allColors)
{
	int redDistance = pixelColor[0] - allColors[otherColor].rgb[0];
	int greenDistance = pixelColor[1] - allColors[otherColor].rgb[1];
	int blueDistance = pixelColor[2] - allColors[otherColor].rgb[2];

	//cout << redDistance << " " << greenDistance << " " << blueDistance << endl;

	redDistance *= redDistance;
	greenDistance *= greenDistance;
	blueDistance *= blueDistance;

	//cout << redDistance << " " << greenDistance << " " << blueDistance << endl;

	float totalDistance = sqrt(redDistance + greenDistance + blueDistance);

	//cout << totalDistance << endl;

	return totalDistance;
}

void outputImage(int imageSize, Pixel ** finalImage, const string fileName)
{
	/*
	Image image(Geometry(imageSize, imageSize), "white");
	for (int i = 0; i < imageSize; i++)
	{
		for (int j = 0; j < imageSize; j++)
		{
			image.pixelColor(i, j, Color(QuantumRange*finalImage[i][j].rgb[0], QuantumRange*finalImage[i][j].rgb[1], QuantumRange*finalImage[i][j].rgb[2], QuantumRange));
		}
	}
	image.write(fileName);
	*/

	//From the googles
	///*
	Image image(Geometry(imageSize, imageSize), Color(uint8_t(0), uint8_t(0), uint8_t(0)));
	image.type(TrueColorType);
	image.fillColor("white");
	size_t h = image.columns();
	size_t w = image.rows();
	Quantum *mpixels = image.setPixels(0, 0, w, h);
	for (int col = 0; col < h; ++col) {
		for (int row = 0; row < w; ++row) {
			size_t offset = (w * row + col);
			size_t moffset = image.channels() * offset;
			mpixels[moffset + 0] = finalImage[col][row].rgb[0] * 257;
			mpixels[moffset + 1] = finalImage[col][row].rgb[1] * 257;
			mpixels[moffset + 2] = finalImage[col][row].rgb[2] * 257;
		}
	}
	image.syncPixels();
	image.write(fileName);
	//*/
}

void countBlocks(int * blocksToUse,int totalColors, ColorChoice * allColors)
{
	int stacks, blocks;
	for (int i = 0; i < totalColors; i++)
	{
		if (blocksToUse[i] / 64 >= 1)
		{
			stacks = blocksToUse[i] / 64;
			blocks = blocksToUse[i] % 64;
		}
		else
		{
			blocks = blocksToUse[i];
		}

		cout << stacks << " stacks and " << blocks << " blocks of " << allColors[i].block << endl;
	}
}







void setColorChoices(ColorChoice * allColors)
{
	allColors[0].rgb[0] = 127; allColors[0].rgb[1] = 178; allColors[0].rgb[2] = 56;
	allColors[0].block = "green";
	allColors[0].possibleBlocks = new string[]{ "Grass Block", "Slime Block" };

	allColors[1].rgb[0] = 247; allColors[1].rgb[1] = 233; allColors[1].rgb[2] = 163;
	allColors[1].block = "tan?";
	allColors[1].possibleBlocks = new string[]{ "Sand", "Sandstone plain" ,"Sandstone","Sandstone Smooth", "Sandstone Chiseled", "Sandstone Stairs","Sandstone Slab", "Birch Wood vertical Log","Birch Planks", "Birch Stairs","Birch Slab", "Mushroom Block with pores texture facing up", "Bone Block", "Glowstone", "End Stone", "End Stone Bricks", "Birch Wood", "Birch Door","Birch Fence","Birch Fence Gate" };

	allColors[2].rgb[0] = 199; allColors[2].rgb[1] = 199; allColors[2].rgb[2] = 199;
	allColors[2].block = "lightish gray";
	allColors[2].possibleBlocks = new string[]{ "Mushroom Stem with stem texture facing up", "Cobweb", "Bed head" };

	allColors[3].rgb[0] = 255; allColors[3].rgb[1] = 0; allColors[3].rgb[2] = 0;
	allColors[3].block = "RED";
	allColors[3].possibleBlocks = new string[]{ "TNT", "Block of Redstone", "Lava", "Fire" };

	allColors[4].rgb[0] = 160; allColors[4].rgb[1] = 160; allColors[4].rgb[2] = 160;
	allColors[4].block = "light blue";
	allColors[4].possibleBlocks = new string[]{ "Ice", "Frosted Ice", "Packed Ice" };

	allColors[5].rgb[0] = 167; allColors[5].rgb[1] = 167; allColors[5].rgb[2] = 167;
	allColors[5].block = "darker gray";
	allColors[5].possibleBlocks = new string[]{ "Block of Iron", "Iron Door", "Iron Trapdoor", "Iron Bars", "Weighted Pressure Plate(Heavy)", "Anvil", "Brewing Stand" };

	allColors[6].rgb[0] = 0; allColors[6].rgb[1] = 124; allColors[6].rgb[2] = 0;
	allColors[6].block = "leaves, thats an accurate description";
	allColors[6].possibleBlocks = new string[]{ "Leaves","Sapling", "Tall Grass", "Flower", "Lily Pad", "Wheat", "Carrots", "Potatoes", "Beetroots", "Sugar Canes", "Pumpkin Stem", "Melon Stem", "Cactus", "Vines", "Cocoa", "small red mushroom", "small brown Mushroom" };

	allColors[7].rgb[0] = 255; allColors[7].rgb[1] = 255; allColors[7].rgb[2] = 255;
	allColors[7].block = "$10 says this color doesnt wooooooooork!!";
	allColors[7].possibleBlocks = new string[]{ "White Wool", "Snow Block","Snow Layer", "White Carpet", "White Stained Glass", "White Stained Glass Pane", "White Bed foot", "White Glazed Terracotta", "White Concrete", "White Concrete Powder" };

	allColors[8].rgb[0] = 164; allColors[8].rgb[1] = 168; allColors[8].rgb[2] = 184;
	allColors[8].block = "blueish gray #1";
	allColors[8].possibleBlocks = new string[]{ "Clay", "Infested Stone","Infested Cobblestone","Infested Stone Brick","Infested Mossy Stone Brick","Infested Cracked Stone Brick","Infested Chiseled Stone Brick" };

	allColors[9].rgb[0] = 151; allColors[9].rgb[1] = 109; allColors[9].rgb[2] = 77;
	allColors[9].block = "Dirt, literally just dirt";
	allColors[9].possibleBlocks = new string[]{ "Jungle Wood vertical Log" ,"Jungle Planks","Jungle Stairs","Jungle Slab", "Brown Mushroom Block with cap texture facing up", "Dirt", "Coarse Dirt", "Jukebox", "Granite plain", "Polished Granite", "Farmland", "Grass Path","Jungle Door", "Jungle Fence","Jungle Fence Gate" };

	allColors[10].rgb[0] = 112; allColors[10].rgb[1] = 112; allColors[10].rgb[2] = 112;
	allColors[10].block = "Stone gray";
	allColors[10].possibleBlocks = new string[]{ "Cobblestone block","Cobblestone Stairs","Cobblestone Slab", "Stone block", "Stone Slab", "Stone Brick","Cracked Stone Brick", "Chiseled Stone Brick", "Stone Brick Stairs", "Stone Brick Slab", "Moss Stone", "Mossy Stone Bricks", "Gravel", "Andesite", "Polished Andesite","Coal  Ore", "Iron Ore", "Gold Ore", "Redstone Ore", "Lapis Lazuli Ore", "Diamond Ore", "Emerald Ore", "Bedrock", "Furnace", "Dispenser", "Dropper", "Hopper", "Observer", "horizontal Acacia Log","Cobblestone Wall", "Mossy Cobblestone Wall", "Stone Pressure Plate", "Cauldron", "Piston Base" , "Piston Head", "Piston Extenstion","Sticky Piston Base" , "Sticky Piston Head", "Sticky Piston Extenstion", "Ender Chest", "Spawner" };

	allColors[11].rgb[0] = 64; allColors[11].rgb[1] = 64; allColors[11].rgb[2] = 255;
	allColors[11].block = "melt your eyes BLUE";
	allColors[11].possibleBlocks = new string[]{ "Water" };

	allColors[12].rgb[0] = 143; allColors[12].rgb[1] = 119; allColors[12].rgb[2] = 72;
	allColors[12].block = "turd";
	allColors[12].possibleBlocks = new string[]{ "Oak vertical Log", "Oak Planks","Oak Stairs","Oak Slab", "Crafting Table", "Bookshelf", "Note Block" };

	allColors[13].rgb[0] = 255; allColors[13].rgb[1] = 255; allColors[13].rgb[2] = 245;
	allColors[13].block = "not quite broken white";
	allColors[13].possibleBlocks = new string[]{ "Horizontal Birch Log", "Diorite" , "Polished Diorite", "Quartz Block", "Chiseled Quartz", "Quartz Pillar","Quartz Stairs","Quartz Slab", "Sea Lantern" };

	allColors[14].rgb[0] = 216; allColors[14].rgb[1] = 127; allColors[14].rgb[2] = 51;
	allColors[14].block = "dull orange";
	allColors[14].possibleBlocks = new string[]{ "Acacia vertical Log","Acacia Planks","Acacia Stairs","Acacia Slab", "Orange Wool", "Orange Carpet", "Pumpkin", "Jack o'Lantern", "Red Sand", "Red Sandstone", "Smooth Red Sandstone", "Chiseled Red Sandstone", "Red Sandstone Stairs","Red Sandstone Slab", "Terracotta","Hardened Clay","Orange Glazed Terracotta", "Orange Concrete", "Orange Concrete Powder","Acacia Door","Acacia Fence","Acacia Fence Gate", "Orange Stained Glass", "Orange Stained Glass Pane","Orange Bed foot" };

	allColors[15].rgb[0] = 178; allColors[15].rgb[1] = 76; allColors[15].rgb[2] = 216;
	allColors[15].block = "purple, they call it magnenta, they are wrong";
	allColors[15].possibleBlocks = new string[]{ "Purpur Block","Purpur Pillar","Purpur Slab", "Magenta Wool","Magenta Glazed Terracotta", "Magenta Concrete", "Magenta Concrete Powder","Magenta Carpet", "Magenta Stained Glass", "Magenta Stained Glass Pane","Magenta Bed foot" };

	allColors[16].rgb[0] = 102; allColors[3].rgb[16] = 153; allColors[16].rgb[2] = 216;
	allColors[16].block = "'turqoise'";
	allColors[16].possibleBlocks = new string[]{ "Light Blue Wool","Light Blue Glazed Terracotta", "Light Blue Concrete", "Light Blue Concrete Powder","Light Blue Carpet", "Light Blue Stained Glass", "Light Blue Stained Glass Pane","Light Blue Bed foot" };

	allColors[17].rgb[0] = 229; allColors[17].rgb[1] = 229; allColors[17].rgb[2] = 51;
	allColors[17].block = "sunshine";
	allColors[17].possibleBlocks = new string[]{ "Hay Bale", "Sponge", "Wet Sponge", "Yellow Wool","Yellow Glazed Terracotta", "Yellow Concrete", "Yellow Concrete Powder","Yellow Carpet", "Yellow Stained Glass", "Yellow Stained Glass Pane", "Yellow Bed foot" };

	allColors[18].rgb[0] = 127; allColors[18].rgb[1] = 204; allColors[18].rgb[2] = 25;
	allColors[18].block = "they are right, this is lime";
	allColors[18].possibleBlocks = new string[]{ "Melon", "Lime Wool", "Lime Glazed Terracotta", "Lime Concrete", "Lime Concrete Powder", "Lime Carpet", "Lime Stained Glass", "Lime Stained Glass Pane", "Lime Bed foot" };

	allColors[19].rgb[0] = 242; allColors[19].rgb[1] = 127; allColors[19].rgb[2] = 165;
	allColors[19].block = "gender binary pink - ew";
	allColors[19].possibleBlocks = new string[]{ "Pink Wool","Pink Glazed Terracotta", "Pink Concrete", "Pink Concrete Powder","Pink Carpet", "Pink Stained Glass", "Pink Stained Glass Pane","Pink Bed foot" };

	allColors[20].rgb[0] = 76; allColors[20].rgb[1] = 76; allColors[20].rgb[2] = 76;
	allColors[20].block = "ashes of my soul";
	allColors[20].possibleBlocks = new string[]{ "Gray Wool","Gray Glazed Terracotta", "Gray Concrete", "Gray Concrete Powder","Gray Carpet", "Gray Stained Glass", "Gray Stained Glass Pane","Gray Bed foot" };

	allColors[21].rgb[0] = 153; allColors[21].rgb[1] = 153; allColors[21].rgb[2] = 153;
	allColors[21].block = "silver";
	allColors[21].possibleBlocks = new string[]{ "Structure Block", "Light Gray Wool","Light Gray Glazed Terracotta", "Light Gray Concrete", "Light Gray Concrete Powder","Light Gray Carpet", "Light Gray Stained Glass", "Light Gray Stained Glass Pane","Light gray bed foot" };

	allColors[22].rgb[0] = 76; allColors[22].rgb[1] = 127; allColors[22].rgb[2] = 153;
	allColors[22].block = "'cyan' ooooooooooh";
	allColors[22].possibleBlocks = new string[]{ "Prismarine", "Cyan Wool","Cyan Glazed Terracotta", "Cyan Concrete", "Cyan Concrete Powder","Cyan Carpet", "Cyan Stained Glass", "Cyan Stained Glass Pane","Cyan Bed Foot" };

	allColors[23].rgb[0] = 127; allColors[23].rgb[1] = 63; allColors[23].rgb[2] = 178;
	allColors[23].block = "royal / dull purple";
	allColors[23].possibleBlocks = new string[]{ "Repeating Command Block", "Mycelium", "Purple Wool","Purple Glazed Terracotta", "Purple Concrete", "Purple Concrete Powder","Chorus Flower", "Chorus Plant", "Purple Carpet", "Purple Stained Glass", "Purple Stained Glass Pane","Purple Bed foot" };

	allColors[24].rgb[0] = 51; allColors[24].rgb[1] = 76; allColors[24].rgb[2] = 178;
	allColors[24].block = "appropriatly toned down blue";
	allColors[24].possibleBlocks = new string[]{ "Blue Wool"," Blue Glazed Terracotta", "Blue Concrete", "Blue Concrete Powder","Blue Carpet", "Blue Stained Glass", "Blue Stained Glass Pane","Blue Bed Foot" };

	allColors[25].rgb[0] = 102; allColors[25].rgb[1] = 76; allColors[25].rgb[2] = 51;
	allColors[25].block = "boring brown";
	allColors[25].possibleBlocks = new string[]{ "Dark Oak vertical log", "Dark Oak horizontal Log","Dark Oak Planks","Dark Oak Stairs","Dark Oak Slab", "horizontal Spruce Log", "Impulse Command Block", "Soul Sand", "Brown Wool","Brown Glazed Terracotta", "Brown Concrete", "Brown Concrete Powder", "Dark Oak Door","Dark Oak Fence","Dark Oak Fence Gate", "Brown Carpet", "Brown Stained Glass", "Brown Stained Glass Pane","Brown Bed Foot" };

	allColors[26].rgb[0] = 102; allColors[26].rgb[1] = 127; allColors[26].rgb[2] = 51;
	allColors[26].block = "money bags";
	allColors[26].possibleBlocks = new string[]{ "Chain Command Block", "Green Wool","Green Glazed Terracotta", "Green Concrete", "Green Concrete Powder","End Portal Frame", "Green Carpet", "Green Stained Glass", "Green Stained Glass Pane","Green Bed Foot" };

	allColors[27].rgb[0] = 153; allColors[27].rgb[1] = 51; allColors[27].rgb[2] = 51;
	allColors[27].block = "red";
	allColors[27].possibleBlocks = new string[]{ "Brick block","Brick Slab", "Brick Stairs", "Nether Wart Block", "Red Wool", "Red Glazed Terracotta", "Red Concrete", "Red Concrete Powder", "Nether Wart", "Enchantment Table", "Red Carpet", "Red Stained Glass", "Red Stained Glass Pane","Red Bed Foot" };

	allColors[28].rgb[0] = 25; allColors[28].rgb[1] = 25; allColors[28].rgb[2] = 25;
	allColors[28].block = "black, thats it, basically black right there";
	allColors[28].possibleBlocks = new string[]{ "Block of Coal", "Obsidian", "Black Wool","Black Glazed Terracotta", "Black Concrete", "Black Concrete Powder","Dragon Egg", "End Gateway", "End Portal", "Black Carpet", "Black Stained Glass", "Black Stained Glass Pane","Black Bed Foot" };

	allColors[29].rgb[0] = 250; allColors[29].rgb[1] = 238; allColors[29].rgb[2] = 77;
	allColors[29].block = "gold with no shine :(";
	allColors[29].possibleBlocks = new string[]{ "Block of Gold","Weighted pressure plate (light)" };

	allColors[30].rgb[0] = 92; allColors[30].rgb[1] = 219; allColors[30].rgb[2] = 213;
	allColors[30].block = "DIAMOND DIAMOND DIAMOND";
	allColors[30].possibleBlocks = new string[]{ "Block of Diamond", "Prismarine Bricks", "Dark Prismarine","Beacon" };

	allColors[31].rgb[0] = 74; allColors[31].rgb[1] = 128; allColors[31].rgb[2] = 255;
	allColors[31].block = "another Burn your eyes Blue";
	allColors[31].possibleBlocks = new string[]{"Lapis Lazuli Block"};

	allColors[32].rgb[0] = 0; allColors[32].rgb[1] = 217; allColors[32].rgb[2] = 58;
	allColors[32].block = "cartoon grass";
	allColors[32].possibleBlocks = new string[]{"Block of Emerald"};

	allColors[33].rgb[0] = 129; allColors[33].rgb[1] = 86; allColors[33].rgb[2] = 49;
	allColors[33].block = "brown thats apparently obsidian??????";
	allColors[33].possibleBlocks = new string[]{"Spruce vertical Log","Spruce Planks","Spruce Stairs","Spruce Slab", "horizontal Oak Log", "horizontal Jungle Log", "Podzol","Spruce Door","Spruce Fence","Spruce Fence Gate"};

	allColors[34].rgb[0] = 112; allColors[34].rgb[1] = 2; allColors[34].rgb[2] = 0;
	allColors[34].block = "dullllllllllll red";
	allColors[34].possibleBlocks = new string[]{"Netherrack", "Nether Bricks", "Nether Brick Stairs", "Red Nether Bricks", "Nether Quartz Ore", "Magma Block","Nether Brick Fence"};

	allColors[35].rgb[0] = 209; allColors[35].rgb[1] = 177; allColors[35].rgb[2] = 161;
	allColors[35].block = "off white #312";
	allColors[35].possibleBlocks = new string[]{"White Terracotta"};

	allColors[36].rgb[0] = 159; allColors[36].rgb[1] = 133; allColors[36].rgb[2] = 36;
	allColors[36].block = "i approve of this orange";
	allColors[36].possibleBlocks = new string[]{ "Orange Terracotta" };

	allColors[37].rgb[0] = 149; allColors[37].rgb[1] = 87; allColors[37].rgb[2] = 108;
	allColors[37].block = "gross magenta";
	allColors[37].possibleBlocks = new string{ "Magenta Terracotta" };

	allColors[38].rgb[0] = 112; allColors[38].rgb[1] = 108; allColors[38].rgb[2] = 138;
	allColors[38].block = "stormy blue";
	allColors[38].possibleBlocks = new string{ "Light Blue Terracotta" };

	allColors[39].rgb[0] = 189; allColors[39].rgb[1] = 133; allColors[39].rgb[2] = 36;
	allColors[39].block = "FUCKING SUPERB you funky little orange";
	allColors[39].possibleBlocks = new string{ "Yellow Terracotta" };

	allColors[40].rgb[0] = 103; allColors[40].rgb[1] = 117; allColors[40].rgb[2] = 53;
	allColors[40].block = "i can only imagine that this is the literal color of weed";
	allColors[40].possibleBlocks = new string{ "Lime Terracotta" };

	allColors[41].rgb[0] = 160; allColors[41].rgb[1] = 77; allColors[41].rgb[2] = 78;
	allColors[41].block = "cherry(ish)";
	allColors[41].possibleBlocks = new string{ "Pink Terracotta" };

	allColors[42].rgb[0] = 57; allColors[42].rgb[1] = 41; allColors[42].rgb[2] = 35;
	allColors[42].block = "somehow off black????";
	allColors[42].possibleBlocks = new string{ "Gray Terracotta" };

	allColors[43].rgb[0] = 135; allColors[43].rgb[1] = 107; allColors[43].rgb[2] = 98;
	allColors[43].block = "more tan";
	allColors[43].possibleBlocks = new string{ "Light Gray Terracotta" };

	allColors[44].rgb[0] = 87; allColors[44].rgb[1] = 92; allColors[44].rgb[2] = 92;
	allColors[44].block = "they call this one 'cyan' as well......";
	allColors[44].possibleBlocks = new string{ "Cyan Terracotta" };

	allColors[45].rgb[0] = 122; allColors[45].rgb[1] = 73; allColors[45].rgb[2] = 88;
	allColors[45].block = "idk how i feel about this purple...";
	allColors[45].possibleBlocks = new string{ "Purple Terracotta" };

	allColors[46].rgb[0] = 76; allColors[46].rgb[1] = 62; allColors[46].rgb[2] = 92;
	allColors[46].block = "sure, blue";
	allColors[46].possibleBlocks = new string{ "Blue Terracotta" };

	allColors[47].rgb[0] = 76; allColors[47].rgb[1] = 50; allColors[47].rgb[2] = 35;
	allColors[47].block = "oooooh ANOTHER turddddddd :)";
	allColors[47].possibleBlocks = new string{ "Brown Terracotta" };

	allColors[48].rgb[0] = 76; allColors[48].rgb[1] = 82; allColors[48].rgb[2] = 42;
	allColors[48].block = "dulllllll af green";
	allColors[48].possibleBlocks = new string{ "Green Terracotta" };

	allColors[49].rgb[0] = 142; allColors[49].rgb[1] = 60; allColors[49].rgb[2] = 46;
	allColors[49].block = "gross af red";
	allColors[49].possibleBlocks = new string{ "Red Terracotta" };

	allColors[50].rgb[0] = 37; allColors[50].rgb[1] = 22; allColors[50].rgb[2] = 16;
	allColors[50].block = "black (kinda)";
	allColors[50].possibleBlocks = new string{ "Black Terracotta" };
}