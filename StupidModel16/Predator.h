#ifndef _PREDATOR_H
#define _PREDATOR_H

#include "Cell.h"
#include "Bug.h"

using namespace sf;

class Predator {
private:
	//the shape to draw as the predator
	CircleShape m_shape;
	
	//the location of bug in grid
	Vector2i gridLoc;

	void clampToGrid(Vector2i &vec, Vector2u const &gridSize);
	
	//pointer to main grid
	std::vector<std::vector<HabitatCell> > * grid;
	
public:
	Predator(Vector2i loc, float radius, std::vector<std::vector<HabitatCell> > * grid);
	
	void Hunt(vector<Bug> &bugs, Vector2u const &gridSize);
	void Draw(RenderWindow &w);
};
#endif