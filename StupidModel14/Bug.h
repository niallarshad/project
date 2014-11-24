#ifndef _BUG_H
#define _BUG_H

#define GridSpacing 5
#define GridSize 100

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

	static int survivalProbability;
	static int eggs;	//the number of children to spawn when reproducing

	//bool actedThisCycle;	//true if the bug has already moved and grown in this loop iteration.

	float maxConsumptionRate;

public:
	Bug(Vector2i loc, float radius, std::vector<std::vector<HabitatCell> > * grid, float MaxConsumptionRate = 1.0f);

	//Returns amount of food consumed
	float Grow();

	void Move();

	//returns true if bug's size is >= 10
	bool IsReproducing( vector<Bug>* bugs );

	//returns true if bug fails at life
	bool Mortality();

	void Draw(RenderWindow &w);

	float getSize() const { return m_shape.getRadius(); }
	void setSize(float newSize) { m_shape.setRadius(newSize); }

	//void CanActThisCycle();
	//bool ActedThisCycle() const;

	bool operator> (Bug const &other) const;
	bool operator< (Bug const &other) const;

	//static int CriticalMass;
};
#endif