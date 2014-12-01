#include "stdafx.h"
#include "Cell.h"

//CELL CTOR
Cell::Cell() : hasBug(false), hasPredator(false) { }

//HABITAT CELL CTOR
HabitatCell::HabitatCell( int xPos, int yPos, float MaxFoodProductionRate, float FoodAvailability)
	: maxFoodProductionRate( MaxFoodProductionRate ),
	foodAvailability( FoodAvailability ) {
	m_shape = sf::RectangleShape( sf::Vector2f(GridSpacing, GridSpacing));
	m_shape.setPosition(xPos*GridSpacing, yPos*GridSpacing);
	}

//Returns available food in cell
float HabitatCell::ProduceFood() {
	//generate a random float from 0.0 to maxProductionRate
	foodAvailability += static_cast<float>( rand() / (static_cast<float>(RAND_MAX / maxFoodProductionRate)) );

	//set the colour of the cell
	sf::Color color = sf::Color::Green;
	color.a = 255 * min(foodAvailability, 1.0f);
	m_shape.setFillColor((color));

	return foodAvailability;
}

void HabitatCell::Draw(sf::RenderWindow &w) {
	w.draw( m_shape );
}