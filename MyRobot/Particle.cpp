#include "Particle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//C'Tor
Particle::Particle(float robotCoordinateX,float robotCoordinateY)
{
	particleInit(robotCoordinateX,robotCoordinateY);
}

// init the particle's coordinates
void Particle::particleInit(float robotCoordinateX,float robotCoordinateY)
{
	int xCellCoordinateByMap,yCellCoordinateByMap;

	particleCoordinateX = (float)rand()/((float)RAND_MAX/30) - 15 + robotCoordinateX;
	particleCoordinateY = (float)rand()/((float)RAND_MAX/30) - 15 + robotCoordinateY;

	particleCoordinateYaw = DEGREES_TO_RADIANS(rand() % MAX_DEGREE);
	particleBelief = 1.0;
}

// updates a particle's belief, position and map
void Particle::updateParticle(float deltaCoordinateX, float deltaCoordinateY, float deltaCoordinateYaw, float laserScan[], int laserCount)
{
	ConfigurationManager * cfg = ConfigurationManager::getInstance();
	particleCoordinateX   += deltaCoordinateX;// / cfg->getMapResolutionCM();
	particleCoordinateY   += deltaCoordinateY;// / cfg->getMapResolutionCM();
	particleCoordinateYaw += deltaCoordinateYaw;

	double predictedBelief = particleBelief * particleProbCalc(deltaCoordinateX, deltaCoordinateY, deltaCoordinateYaw);
	particleBelief =  predictedBelief * calcPrecisionByMap(laserScan,laserCount);
}

// calculates the particle's probability using the delta coordinates (X, Y & Yaw)
float Particle::particleProbCalc(float deltaCoordinateX, float deltaCoordinateY, float deltaCoordinateYaw)
{
	float distance = sqrt(pow(deltaCoordinateX,2)+pow(deltaCoordinateY,2));

	if ((deltaCoordinateYaw < NORMAL_ACCURATE_ANGLE_TO_MOVE) && (deltaCoordinateYaw > -NORMAL_ACCURATE_ANGLE_TO_MOVE))
	{
		if (distance <= SAFE_DISTANCE_TO_MOVE)
			return 1.0;
		else
			return (SAFE_DISTANCE_TO_MOVE + distance + 5);
	}
	else if ((deltaCoordinateYaw < MAX_ACCURATE_ANGLE_TO_MOVE) && (deltaCoordinateYaw > -MAX_ACCURATE_ANGLE_TO_MOVE))
	{
		if (distance <= SAFE_DISTANCE_TO_MOVE)
			return 1.0;
		else
			return (SAFE_DISTANCE_TO_MOVE + distance - 5);
	}

	return 1.0;
}

// calculates the precision factor (%TP / %TP + %FP) using the particle's map & laser scan
float Particle::calcPrecisionByMap(float laserScan[], int laserCount)
{
	float xObj,yObj;
	int xCellCoordinateByMap,yCellCoordinateByMap;
	int countMiss = 0;
	int countHit = 0;

	for (int i=0; i<laserCount; i+= INDEX_PER_DEGREE)
	{
		if (laserScan[i] > OPEN_PATH_RANGE)
		{
			for (int j=SENSOR_FROM_END; j<=SENSOR_DETECTION_RANGE; j+= CELL_DIMENSION)
			{
				xObj = (j * cos(DEGREES_TO_RADIANS(convLaserIdxToAngle(i,laserCount,LASER_ANGLE_RANGE)) + particleCoordinateYaw)) + particleCoordinateX;
				yObj = (j * sin(DEGREES_TO_RADIANS(convLaserIdxToAngle(i,laserCount,LASER_ANGLE_RANGE)) + particleCoordinateYaw)) + particleCoordinateY;

				particleMap.getMapCellByPosition(xObj,yObj,xCellCoordinateByMap,yCellCoordinateByMap);

				if(particleMap.getMapCellValue(xCellCoordinateByMap,yCellCoordinateByMap) == OBSTACLE)
				{
					countMiss++;
				}
				else if(particleMap.getMapCellValue(xCellCoordinateByMap,yCellCoordinateByMap) == FREE)
				{
					countHit++;
				}
			}
		}
		else
		{
			xObj = ((METER_TO_CM(laserScan[i])) * cos(DEGREES_TO_RADIANS(convLaserIdxToAngle(i,laserCount,LASER_ANGLE_RANGE)) + particleCoordinateYaw)) + particleCoordinateX;
			yObj = ((METER_TO_CM(laserScan[i])) * sin(DEGREES_TO_RADIANS(convLaserIdxToAngle(i,laserCount,LASER_ANGLE_RANGE)) + particleCoordinateYaw)) + particleCoordinateY;

			particleMap.getMapCellByPosition(xObj,yObj,xCellCoordinateByMap,yCellCoordinateByMap);

			if(particleMap.getMapCellValue(xCellCoordinateByMap,yCellCoordinateByMap) == OBSTACLE)
			{
				countHit++;
			}
			else if(particleMap.getMapCellValue(xCellCoordinateByMap,yCellCoordinateByMap) == FREE)
			{
				countMiss++;
			}
		}
	}

	return ((float)(countHit))/(countMiss + countHit);
}

// set's a newly created particle's map from a current particle's map (father -> child)
void Particle::setParticleMap(Map particleMapCopy)
{
	particleMap = particleMapCopy;
}

//set's the particle's Yaw coordinate
void Particle::setParticleYaw(float yawCoordinate)
{
	particleCoordinateYaw = yawCoordinate;
}

// get's the particle's map
Map Particle::getParticleMap()
{
	return particleMap;
}

// get's the particle's calculated belief
double Particle::getParticleBelief()
{
	return particleBelief;
}


//converts a laser index id to an angle
float Particle::convLaserIdxToAngle(int index, int xCoordinate, int radius)
{
	return ((float)index*((float)radius/xCoordinate))-radius/2.0;
}

// print's the particle's map
void Particle::printParticleMap()
{
}
