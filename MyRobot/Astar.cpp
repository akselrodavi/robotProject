#include "Astar.h"
#include <vector>
#include <queue>
#include <ctime>
#include "Map.h"
#include "ConfigurationManager.h"
#include <cmath>
#include <limits>
#include <iostream>
using namespace std;

vector<Node*> Astar::PathPlanner(Node * startPoint,Node * endPoint)
{
	//-1-1|-1,0|-1,1
	// 0-1| 0 0| 0,1
	// 1-1| 1 0| 1,1
	//                ^
	// Vertical   =   |

	// Horizontal=   -->
	ConfigurationManager *configFile;
	configFile = ConfigurationManager::getInstance();
	Map * myMap = new Map();
	int robotSizeX = configFile->getRobotSizeX();
	int robotSizeY = configFile->getRobotSizeY();

	// Define the the first node
	vector<Node*> AllFoundNodes;
	AllFoundNodes.push_back(startPoint);

	// Set the obstical to 1
	int Obstical = 1;

	// Check if the robot get the destiny
	bool FoundTheDestiny = false;

	// While not found the destiny
	while(!FoundTheDestiny)
	{
		int minPriority = numeric_limits<int>::max();
		int minPriorityIndexFound = -1;
		// Find the lowest priority node that haven't checked
		for (int index=0; index < (int)AllFoundNodes.size(); index++)
		{
			if (!AllFoundNodes[index]->IsChecked())
			{
				if (AllFoundNodes[index]->getPriority() <= minPriority)
				{
					minPriority =AllFoundNodes[index]->getPriority();
					minPriorityIndexFound = index;
				}
			}
		}

		Node * currentNode = AllFoundNodes[minPriorityIndexFound];
		AllFoundNodes[minPriorityIndexFound]->updateChecked(true);
		// For debug
		int size = (int)AllFoundNodes.size();

//		int x = currentNode.getxPos();
//		int y = currentNode.getyPos();
//		int rzx = configFile->getRobotSizeX();
//		int rzy = configFile->getRobotSizeY();

		// Run for all Horizontal
		for (int vertical=-1; vertical<=1; vertical++)
		{
			// Run for all vertical
			for (int horizontal=-1; horizontal <=1; horizontal++)
			{
				// Check if not current position
				if (!(horizontal ==0 && vertical ==0))
				{
					// Check the position in the map
					//if (Map().isPositionInMap(currentNode.getxPos() + horizontal* robotSizeX, currentNode->getyPos() + vertical* robotSizeY))
					if (true)
					{
						int xpos = currentNode->getxPos()+ horizontal *robotSizeX;
						int ypos = currentNode->getyPos()+vertical *robotSizeY;

						// Get the current position on the map
						//Map().getMapCellByPosition(currentNode.getxPos()+ horizontal *robotSizeX, currentNode.getyPos()+vertical *robotSizeY,xpos,ypos);
						// Check if there is not obsticale
						if (!((myMap->getMapCellValue(xpos, ypos)==Obstical)))
						{
							// Set the new found node
							int newXPOS = currentNode->getxPos() + horizontal * robotSizeX;
							int newYPOS = currentNode->getyPos() + vertical * robotSizeY;
							int newLevel = currentNode->getLevel();
							Node * newNode = new Node(*currentNode,newXPOS,newYPOS,newLevel);
							newNode->UpdateData(horizontal, vertical, endPoint->getxPos(),endPoint->getyPos());
							// Check if the found Node priority is better than the old one

							// Set boolean if cell was found
							bool cellfound = false;
							// Check this cell in the vector (if it already exists)
							for (int index=0; index < (int)AllFoundNodes.size(); index++)
							{
								// Check if cell was found
								if (AllFoundNodes[index]->getxPos() == currentNode->getxPos() + horizontal * robotSizeX &&
									AllFoundNodes[index]->getyPos() == currentNode->getyPos() + vertical * robotSizeY)
								{
									// Set found
									cellfound = true;
									if (AllFoundNodes[index]->getPriority() > newNode->getPriority())
									{
										AllFoundNodes[index] = newNode;
										//break;
									}
								}
							}
							// Check if cell was not found
							if (!cellfound)
							{
								// Add new Node
								AllFoundNodes.push_back(newNode);

								// Check if the cell is the destiny
								if ((newNode->getxPos() + robotSizeX >= endPoint->getxPos()) &&
									(newNode->getxPos() <= endPoint->getxPos() + robotSizeX) &&
									(newNode->getyPos() + robotSizeY >= endPoint->getyPos()) &&
									(newNode->getyPos() <= endPoint->getyPos() + robotSizeY))
								{
									FoundTheDestiny = true;
									int newXPOS = endPoint->getxPos();
									int newYPOS = endPoint->getyPos();
									int newLevel = newNode->getLevel();
									Node * endNode = new Node(*newNode,newXPOS,newYPOS,newLevel);
									endNode->UpdateData(horizontal, vertical, endPoint->getxPos(),endPoint->getyPos());

									AllFoundNodes.push_back(endNode);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	// After Ended Create the path
	int indexFinal =-1;
	// Search in loop where is the nodes and make of them queue
	for (int index=0; index < (int)AllFoundNodes.size(); index++)
	{
		// Get the index of final node
		// if the distancnce is 0, than you found the last one
		if((AllFoundNodes[index]->getxPos() == endPoint->getxPos()) &&
		   (AllFoundNodes[index]->getyPos() == endPoint->getyPos()))
		{
			indexFinal = index;
		}
	}

	// Set the queue of the path
	queue<Node*> finalPath;
	int s = finalPath.size();
	int b = AllFoundNodes.size();
	bool start = false;
	// Set the last node
	Node * current = AllFoundNodes[indexFinal];
	finalPath.push(current);
	//Node *node = current->getLastNode();
	// While i dont get the start position
	while(!start)
	{
		current = current->getLastNode();

		// Add to the queue
		if (!((current->getxPos() != startPoint->getxPos()) &&
			(current->getyPos() != startPoint->getyPos())))
		{
			start = true;
		}
		finalPath.push(current);
	}


/*	int si = finalPath.size();
	for (int i = 0; i < si; i++)
	{
		cout << finalPath.front()->getxPos() << "," << finalPath.front()->getyPos() << endl;
		finalPath.pop();
	}
*/

	// Set the path from start to end
	int sizeOfPath = finalPath.size();
	vector<Node*> finalPathArr;

	for (int index =0; index < sizeOfPath; index++)
	{
		Node *n = finalPath.front();
		finalPath.pop();
		finalPathArr.push_back(n);

	}

	reverse(finalPathArr.begin(), finalPathArr.end());

	return finalPathArr;
}
