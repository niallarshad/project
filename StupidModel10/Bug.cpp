#include "Bug.h"

int Bug::CriticalMass = 100;

Bug::Bug(Vector2i loc, float radius, std::vector<std::vector<HabitatCell> > * grid,  float MaxConsumptionRate)
	: gridLoc(loc), m_shape(radius), grid(grid), maxConsumptionRate(MaxConsumptionRate) {

		m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
		m_shape.setFillColor(Color::Red);
	}
	
void Bug::Move() {

		Vector2i desiredLoc;

		do
		{
			//the location the bug wants to move to
			desiredLoc = Vector2i(rand() % 9 - 4, rand() % 9 - 4);

			desiredLoc = gridLoc - desiredLoc;

			if(desiredLoc.x < 0)
				desiredLoc.x += 100;
			else if(desiredLoc.x >= 100)
				desiredLoc.x -= 100;

			if(desiredLoc.y < 0)
				desiredLoc.y += 100;
			else if(desiredLoc.y >= 100)
				desiredLoc.y -= 100;

		} while ( grid->at(desiredLoc.x).at(desiredLoc.y).hasBug );	//while there is already a bug at the desired location

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