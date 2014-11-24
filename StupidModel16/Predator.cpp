#include "stdafx.h"
#include "Predator.h"

Predator::Predator(Vector2i loc, float radius, std::vector<std::vector<HabitatCell> > * grid)
	: gridLoc(loc), m_shape(ra dius), grid(grid) {
	m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
	m_shape.setFillColor(Color::Blue);
}

void wrapAroundGrid(Vector2i &vec) {
	//wrap around edges of grid
	if (vec.x < 0)
		vec.x += GridSize;
	else if (vec.x >= GridSize)
		vec.x -= GridSize;

	if (vec.y < 0)
		vec.y += GridSize;
	else if (vec.y >= GridSize)
		vec.y -= GridSize;
}

void Predator::Hunt(vector<Bug> &bugs) {

	vector<Vector2i> suitableCells;

	//for all possible destinations
	for( int i = -1; i <= 1; i++) {
		for ( int j = -1; j <= 1; j++) {

			//a possible position to move to
			Vector2i temp = Vector2i( i + gridLoc.x, j + gridLoc.y );

			//wrap around edges of grid
			wrapAroundGrid( temp );


			HabitatCell* c = &( grid->at(temp.x).at(temp.y) );

			//if the cell has a tasty bug in it, add it to our vector of suitable cells.
			if( (c->hasBug) )  {
				suitableCells.push_back( temp );
			}
		}
	}
	
	int chosenCell;
	
	//if we failed to find a suitable cell with a bug in it...
	if ( suitableCells.size() == 0 ) {
		//...pick a random neighbouring cell to move into
		suitableCells.push_back( Vector2i( rand() % 2 + gridLoc.x - 1, rand() % 2 + gridLoc.y - 1) );
		chosenCell = 0;
	}
	else {
		//pick a random cell to move to from all the suitable cells
		chosenCell = rand() % suitableCells.size();
	}
	
	//if the chosen cell already contains a predator...
	if ( grid->at(suitableCells[chosenCell].x).at(suitableCells[chosenCell].y).hasPredator ) {
		//...give up. Don't move.
		return;
	}

	grid->at(gridLoc.x).at(gridLoc.y).hasPredator = false;
	grid->at(suitableCells[chosenCell].x).at(suitableCells[chosenCell].y).hasPredator = true;

	//move to the desired location
	gridLoc = Vector2i(suitableCells[chosenCell]);	

	m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
	
	//if the cell we just moved into has a bag...
	if ( grid->at(gridLoc.x).at(gridLoc.y).hasBug() ) {
		//...om nom nom
		vector<Bug>::iterator nth = bugs.begin() + i;
		bugs.erase( nth );
	}
}