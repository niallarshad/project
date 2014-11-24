#ifndef _CELL_H
#define _CELL_H

#include "SFML\Graphics.hpp"

using namespace std;

struct Cell {
	bool hasBug;

	Cell();
};

struct HabitatCell : public Cell {
	float foodAvailability;
	const float maxFoodProductionRate;

	HabitatCell( float MaxFoodProductionRate = 0.01f );

	//Returns available food in cell
	float ProduceFood();
};
#endif