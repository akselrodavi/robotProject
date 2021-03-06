#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_

#include <string>

namespace std {

class ConfigurationManager {

private:
	string map;
	int startLocationX;
	int startLocationY;
	int startLocationYaw;
	int goalX;
	int goalY;
	int robotSizeX;
	int robotSizeY;
	int mapResolutionCM;
	int gridResolutionCM;
	static ConfigurationManager *single;

public:
	static ConfigurationManager* getInstance();
	bool readConfigFile(string fileName);
	string getMap();
	int getStartLocationX();
	int getStartLocationY();
	int getStartLocationYaw();
	int getGoalX();
	int getGoalY();
	int getRobotSizeX();
	int getRobotSizeY();
	double getMapResolutionCM();
	double getGridResolutionCM();
	ConfigurationManager()
	{

	}

	~ConfigurationManager()
	{
	}
};

} /* namespace std */

#endif /* CONFIGURATIONMANAGER_H_ */


