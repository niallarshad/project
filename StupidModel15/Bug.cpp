#include "stdafx.h"
#include "Bug.h"

//int Bug::CriticalMass = 100;
int Bug::survivalProbability = 95;	//percentage chance of bug surviving a timestep
int Bug::eggs = 5;

//void wrapAroundGrid(Vector2i &vec, Vector2u const &GridSize) {
//	//wrap around edges of grid
//	if(vec.x < 0)
//		vec.x += GridSize.x;
//	else if(vec.x >= GridSize.x)
//		vec.x -= GridSize.x;
//
//	if(vec.y < 0)
//		vec.y += GridSize.y;
//	else if(vec.y >= GridSize.y)
//		vec.y -= GridSize.y;
//}

void clampToGrid(Vector2i &vec, Vector2u const &gridSize) {
	if(vec.x < 0)
		vec.x = 0;
	else if(vec.x >= gridSize.x)
		vec.x = gridSize.x - 1;

	if(vec.y < 0)
		vec.y = 0;
	else if(vec.y >= gridSize.y)
		vec.y = gridSize.y - 1;
}

Bug::Bug(Vector2i loc, float radius, std::vector<std::vector<HabitatCell> > * grid,  float MaxConsumptionRate)
	: gridLoc(loc), m_shape(radius/2.0f), grid(grid), maxConsumptionRate(MaxConsumptionRate) {

		m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
		m_shape.setFillColor(Color::Red);
		//m_shape.setFillColor(Color(rand()%255, rand()%255, rand()%255));
	}
	
void Bug::Move(Vector2u const &gridSize) {

		//Vector2i desiredLoc;
		//vector<HabitatCell*> possibleLocations;
		float mostFood = 0;
		vector<Vector2i> suitableCells;

		//for all possible destinations
		for( int i = -4; i <= 4; i++) {
			for ( int j = -4; j <= 4; j++) {

				//a possible position to move to
				Vector2i temp = Vector2i( i + gridLoc.x, j + gridLoc.y );

				//wrap around edges of grid
				//wrapAroundGrid( temp, gridSize );

				//cgrid is no longer toroidal
				clampToGrid(temp, gridSize);

				HabitatCell* c = &( grid->at(temp.x).at(temp.y) );

				//if the cell is bug-free or it's our current cell...
				if( (!c->hasBug) ||  (i == 0 && j == 0) )  {
					if (c->foodAvailability > mostFood) {
						mostFood = c->foodAvailability;
						suitableCells.clear();
						suitableCells.push_back(temp);
					}
					else if (c->foodAvailability == mostFood){
						suitableCells.push_back(temp);
					}
				}
			}
		}
		
		//pick a random cell to move to from all the suitable cells
		int chosenCell = rand() % suitableCells.size();

		grid->at(gridLoc.x).at(gridLoc.y).hasBug = false;
		grid->at(suitableCells[chosenCell].x).at(suitableCells[chosenCell].y).hasBug = true;

		//move to the desired location
		gridLoc = Vector2i(suitableCells[chosenCell]);

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
	size += foodEaten/2.0f;

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

bool Bug::IsReproducing( vector<Bug>* bugs, Vector2u const &gridSize ) {

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

				//wrapAroundGrid(loc, gridSize);
				clampToGrid(loc, gridSize);

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