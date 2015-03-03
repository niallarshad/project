#include "Bug.h"

//int Bug::CriticalMass = 100;
int Bug::survivalProbability = 95;	//percentage chance of bug surviving a timestep
int Bug::eggs = 5;

void wrapAroundGrid(Vector2i &vec) {
	//wrap around edges of grid
	if(vec.x < 0)
		vec.x += GridSize;
	else if(vec.x >= GridSize)
		vec.x -= GridSize;

	if(vec.y < 0)
		vec.y += GridSize;
	else if(vec.y >= GridSize)
		vec.y -= GridSize;
}

Bug::Bug(Vector2i loc, float radius, std::vector<std::vector<HabitatCell> > * grid,  float MaxConsumptionRate)
	: gridLoc(loc), m_shape(radius), grid(grid), maxConsumptionRate(MaxConsumptionRate) {

		m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
		//m_shape.setFillColor(Color::Red);
		m_shape.setFillColor(Color(rand()%255, rand()%255, rand()%255));
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
				wrapAroundGrid( temp );


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

bool Bug::IsReproducing( vector<Bug>* bugs ) {

	bool reproducing = false;

	if ( getSize() >= 10 ) {

		//allow children to spawn where parent is (parent is dying)
		grid->at(gridLoc.x).at(gridLoc.y).hasBug = false;

		reproducing = true;
		vector<Bug> children;
		children.reserve( eggs );

		//spawn children within 3 cells of current position
		for( int i = 0; i < eggs; i++ ) {

			//try (max 5 times) to find an empty cell to spawn in
			Vector2i loc; int tries= 0;
			do {
				//random location within 0-3 cells
				loc = Vector2i( rand() % 4 + gridLoc.x, rand() % 4 + gridLoc.y );
				wrapAroundGrid( loc );
				tries++;
			} while ( grid->at(loc.x).at(loc.y).hasBug == true && tries != 5 );

			if ( tries <= 5 ) {
				grid->at(loc.x).at(loc.y).hasBug = true;
				children.push_back( Bug(loc, 0, grid) );
			}//end if
		}//end for

		//add the children to the main bugs vector
		bugs->insert(bugs->end(), children.begin(), children.end());

	}//end if

	return reproducing;
	//return getSize() >= 10 ? true : false;
}

bool Bug::Mortality() {
	float survivalRoll = rand() % 101;
	return survivalRoll > survivalProbability;
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