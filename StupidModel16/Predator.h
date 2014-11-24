#ifndef _PREDATOR_H
#define _PREDATOR_H

#include "Cell.h"
#include "Bug.h"

using namespace sf;

class Predator {
private:
	//the shape to draw as the predator
	ConvexShape m_shape;
	
	//the location of bug in grid
	Vector2i gridLoc;
	
	//pointer to main grid
	std::vector<std::vector<HabitatCell> > * grid;
	
public:
	Predator(Vector2i loc, float radius, std::vector<std::vector<HabitatCell> > * grid);
	
	void Hunt(vector<Bug> &bugs);
};
#endif