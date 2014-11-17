#ifndef _BUG_H
#define _BUG_H

#define GridSpacing 5

#include "SFML\Graphics.hpp"
#include "Cell.h"

using namespace sf;

class Bug {
private:
	//the shape to draw as the bug
	CircleShape m_shape;

	//the location of bug in grid
	Vector2i gridLoc;

	//pointer to main grid
	std::vector<std::vector<HabitatCell> > * grid;

	const float maxConsumptionRate;

public:
	Bug(Vector2i loc, float radius, std::vector<std::vector<HabitatCell> > * grid, float MaxConsumptionRate = 1.0f);

	//Returns amount of food consumed
	float Grow();

	void Move();

	void Draw(RenderWindow &w);

	float getSize() const { return m_shape.getRadius(); }
};
#endif