#include "Cell.h"

//CELL CTOR
Cell::Cell() : hasBug(false) { }

//HABITAT CELL CTOR
HabitatCell::HabitatCell( float MaxFoodProductionRate )
	: maxFoodProductionRate( MaxFoodProductionRate ),
	foodAvailability( 0.0f ) { }

//Returns available food in cell
float HabitatCell::ProduceFood() {
	//generate a random float from 0.0 to maxProductionRate
	foodAvailability += static_cast<float>( rand() / (static_cast<float>(RAND_MAX / maxFoodProductionRate)) );
	return foodAvailability;
}