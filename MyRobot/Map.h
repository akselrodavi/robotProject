#ifndef MAP_H_
#define MAP_H_
#include <string>
#include "Node.h"

#define FREE 			0
#define OBSTACLE 		1

using namespace std;

class Map
{
	private :
		void loadImage(string filename);

	public:
		Map();

		bool isPositionInMap(float xPosition,float yPosition);

		void getMapCellByPosition(float xPosition,float yPosition,int &x,int &y);

		int getMapCellValue(int xPosition,int yPosition);

		int getHeight();

		int getWidth();

		void printParticleMap();

		void saveMapWithPathToPic(Node notesarr[]);


};

#endif /* MAP_H_ */
