#include "GoLeft.h"

//C'Tor
GoLeft::GoLeft(Robot * robot):Behaviors(robot) {}

//An overriden method that checks if the go left start condition was activated (returns true if so)
bool GoLeft::startCondition()
{
	int count = 0;
	int read = (robot->convertDegToIdx(160));

	for (int i=robot->convertDegToIdx(-10) ; i<(robot->convertDegToIdx(10)) ; i++)
	{
		if(robot->getLaserByIdx(read+i) > SAFE_RANGE)
		{
			count++;

			if (count == 10)
			{
				cout<<"SYSTEM: Go left start condition was triggered"<<endl;
				return true;
			}
		}
	}

	return false;
}

//An overriden method that checks if the go left stop condition was activated (returns true if so)
bool GoLeft::stopCondition()
{
	int freeWayAheadCount = 0;

	for (int i=robot->convertDegToIdx(-15) ; i<(robot->convertDegToIdx(15)) ; i+=3)
	{
		if(robot->getLaserByIdx(LASERREAD+i) > SAFE_RANGE)
		{
			freeWayAheadCount++;

			if (freeWayAheadCount == 30)
			{
				cout<<"SYSTEM: Go left stop condition was triggered"<<endl;
				return true;
			}
		}
	}

	return false;
}

//An overriden method that controls the go left action of the current behavior
void GoLeft::action()
{
	robot->setRobotSpeed(0.0,1.0);
}