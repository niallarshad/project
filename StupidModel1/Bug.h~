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
	std::vector<std::vector<Cell> > * grid;

public:
	Bug(Vector2i loc, float radius, std::vector<std::vector<Cell> > * grid);
	void Move();
	void Draw(RenderWindow &w);
};
#endif