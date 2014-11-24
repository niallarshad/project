#include "Bug.h"

int Bug::CriticalMass = 100;

Bug::Bug(Vector2i loc, float radius, std::vector<std::vector<HabitatCell> > * grid,  float MaxConsumptionRate)
	: gridLoc(loc), m_shape(radius), grid(grid), maxConsumptionRate(MaxConsumptionRate) {

		m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
		m_shape.setFillColor(Color::Red);
	}
	
void Bug::Move() {

		Vector2i desiredLoc;
		//vector<HabitatCell*> possibleLocations;
		float mostFood = 0;

		//for all possible destinations
		for( int i = -4; i <= 4; i++) {
			for ( int j = -4; j <= 4; j++) {

				//a possible position to move to
				Vector2i temp = Vector2i( i + gridLoc.x, j + gridLoc.y );

				//wrap around edges of grid
				if(temp.x < 0)
					temp.x += 100;
				else if(temp.x >= 100)
					temp.x -= 100;

				if(temp.y < 0)
					temp.y += 100;
				else if(temp.y >= 100)
					temp.y -= 100;


				HabitatCell* c = &( grid->at(temp.x).at(temp.y) );

				//if the cell is bug-free or it's our current cell...
				if( (!c->hasBug) ||  (i == 0 && j == 0) )  {
					if ( c->foodAvailability > mostFood ) {
						mostFood = c->foodAvailability;
						desiredLoc = temp;
					}
				}
			}
		}

		grid->at(gridLoc.x).at(gridLoc.y).hasBug = false;
		grid->at(desiredLoc.x).at(desiredLoc.y).hasBug = true;

		//move to the desired location
		gridLoc = Vector2i(desiredLoc);

		m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
	}

	//Returns amount of food consumed
	float Bug::Grow()
	{
		float size = m_shape.getRadius();

		HabitatCell * cell = &( grid->at(gridLoc.x).at(gridLoc.y) );

		//get the eatable food at this bug's position
		float foodEaten = std::min( cell->foodAvailability, maxConsumptionRate );

		//subtract the amount of food eaten from the food in the cell
		cell->foodAvailability -= foodEaten;

		//grow the bug by the amount of food eaten
		size += foodEaten;

		m_shape.setRadius(size);

		if(size >= 10.0f && m_shape.getFillColor() != Color::White)
		{
			m_shape.setFillColor(Color::White);
		}

		//actedThisCycle = true;
		
		return foodEaten;
	}

	//tell the render window to draw the shape
	void Bug::Draw(sf::RenderWindow &w) {
		w.draw(m_shape);
	}

//	void Bug::CanActThisCycle() { actedThisCycle = false; }
//	bool Bug::ActedThisCycle() const { return actedThisCycle; }

	//returns true if this bug's size is greater than the other bug's size
	bool Bug::operator> (Bug const &other) const {
		return this->getSize() > other.getSize();
	}

	//returns true if this bug's size is smaller than the other bug's size
	bool Bug::operator< (Bug const &other) const {
		return this->getSize() < other.getSize();
	}