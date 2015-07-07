#include "Map.h"
#include "ConfigurationManager.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include "lodepng.h"

std::vector<unsigned char> image; //the raw pixels
unsigned width, height, resolution;
int** map;

//Default C'Tor
Map::Map(){
	ConfigurationManager *configFile = ConfigurationManager::getInstance();
	string mapPath = configFile->getMap();
	resolution = configFile->getGridResolutionCM();
	loadImage(mapPath);

	int count = 0;
	//Initialize the map
	map = new int*[height];
	for (int i=0; i<height;i++){
		map[i] = new int[width];
		for (int j=0; j<width;j++){
			// Check if the pixel is all white
			if((image[count]==255)&&(image[count+1]==255)&&(image[count+2]==255)&&(image[count+3]==255)){
				map[i][j] = FREE;
			// If not all white then OBSTACLE
			} else {
				map[i][j] = OBSTACLE;
/*				if(i>0  && j>0)
					try{map[i-1][j-1] = OBSTACLE;}catch (exception& e){ cout << e.what() << '\n';}
				if(j>0)
					try{map[i][j-1] = OBSTACLE;}catch (exception& e){ cout << e.what() << '\n';}
				if(i<height && j>0)
					try{map[i+1][j-1] = OBSTACLE;}catch (exception& e){ cout << e.what() << '\n';}
				if(i>0)
					try{map[i-1][j] = OBSTACLE;}catch (exception& e){ cout << e.what() << '\n';}
				if(i<height)
					try{map[i+1][j] = OBSTACLE;}catch (exception& e){ cout << e.what() << '\n';}
				if(i>0 && j<width)
					try{map[i-1][j+1] = OBSTACLE;}catch (exception& e){ cout << e.what() << '\n';}
				if(j<width)
					try{map[i][j+1] = OBSTACLE;}catch (exception& e){ cout << e.what() << '\n';}
				if(i<height && j<width)
					try{map[i+1][j+1] = OBSTACLE;}catch (exception& e){ cout << e.what() << '\n';}*/

			}
			// + 4 because we work with RBGA for each pixel
			count = count + 4;
		}
	}
}

bool Map::isPositionInMap(float xPosition,float yPosition)
{
	bool isInMap = false;
	bool xposmin  = (int)xPosition > -(int)height/2;
	bool xposmax  = (int)xPosition < (int)height/2;
	bool yposmin  = (int)yPosition > -(int)width/2;
	bool yposmax  = (int)yPosition < (int)width/2;
	// Check for X position
	if (xposmin && xposmax && yposmin && yposmax)
	{
		isInMap = true;
	}
	// return result
	return isInMap;
}

//A method which returns a map's cell by a given X & Y coordinates
void Map::getMapCellByPosition(float xPosition,float yPosition,int &x,int &y){
	x = ((int)xPosition) + height/2;
	y = ((int)yPosition) + width/2;
}

//A method which returns a map cell's value by a given X & Y coordinates
int Map::getMapCellValue(int xPosition,int yPosition){return map[xPosition][yPosition];}

//This method load the pic to map[][]
void Map::loadImage(string filename){
	//decode
	unsigned error = lodepng::decode(image, width, height, filename);
	//if there's an error, display it
	if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

//This methode save the vector back to the pic with the path
void Map::saveImage(string filename,Node notesarr[]){
	std::vector<unsigned char> newImage = image;
	int size = (sizeof(notesarr)/sizeof(*notesarr));
	//  color the path
	for (unsigned int i = 0; size; i++){
		Node curNode = notesarr[i];
		int statBitPlace = (width*(curNode.getyPos()-1)*4)+(curNode.getxPos()*4)-4;
		newImage[statBitPlace] = 8;
		newImage[statBitPlace + 1] = 255;
		newImage[statBitPlace + 2] = 2;
		newImage[statBitPlace + 3] = 4;
	}

	//Encode the image
	unsigned error = lodepng::encode(filename, newImage, width, height);
	//if there's an error, display it
	if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

//A methode which save back the map with path
void Map::saveMapWithPathToPic(Node notesarr[]){
	// Get list of dots.
	string newMapPath = "mapWithPath.png";
	saveImage(newMapPath,notesarr);
}

//return height
int Map::getHeight(){return height;}

//return width
int Map::getWidth(){return width;}

//A method which prints a particle's map
void Map::printParticleMap(){
    for (int i=0; i<height; i++){
    	for (int j=0; j<width; j++)
    		cout<<map[i][j];
    	cout<<endl;
    }
}
