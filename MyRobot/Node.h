#ifndef NODE_H_
#define NODE_H_

class Node {

	// From Where the node came from
	Node* fromWhereGetNode;
	// current position
	int xPos;
	int yPos;
	// total distance already travelled to reach the node
	int level;
	// Total distance that need to step over to get to the end node
	int estimateDist;
	// priority=distance already travelled +remaining distance estimate
	int priority;  // smaller: higher priority
	bool checked;


public:
	Node(Node &lastNode,int CurrXPosition, int CurrYPosition,int Level);
	int getxPos() const {return xPos;}
	int getyPos() const {return yPos;}
	int getLevel() const {return level;}
	int getEstimate() const {return estimateDist;}
	int getPriority() const {return priority;}
	Node* getLastNode() {return fromWhereGetNode;}
	bool IsChecked() {return checked;}

	void updateChecked(bool check);
	void UpdateData(int horizontal, int vertical,int xDest,int yDest);
	void updatePriority(int xDest,int yDest);
	void nextLevel(int horizontal, int vertical);
	int estimate(int xDest,int yDest);
};

#endif /* NODE_H_ */
