#include "stdafx.h"
#include "Predator.h"

Predator::Predator(Vector2i loc, float radius, std::vector<std::vector<HabitatCell> > * grid)
	: gridLoc(loc), m_shape(radius), grid(grid) {
	m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
	m_shape.setFillColor(Color::Magenta);
}

void Predator::clampToGrid(Vector2i &vec, Vector2u const &gridSize) {
	if(vec.x < 0)
		vec.x = 0;
	else if(vec.x >= gridSize.x)
		vec.x = gridSize.x - 1;

	if(vec.y < 0)
		vec.y = 0;
	else if(vec.y >= gridSize.y)
		vec.y = gridSize.y - 1;
}

void Predator::Hunt(vector<Bug> &bugs, Vector2u const &gridSize) {

	vector<Vector2i> suitableCells;

	//for all possible destinations
	for( int i = -1; i <= 1; i++) {
		for ( int j = -1; j <= 1; j++) {

			//a possible position to move to
			Vector2i temp = Vector2i( i + gridLoc.x, j + gridLoc.y );

			clampToGrid( temp, gridSize );

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
		Vector2i temp = Vector2i( rand() % 3 + gridLoc.x - 1, rand() % 3 + gridLoc.y - 1);
		clampToGrid(temp, gridSize);
		suitableCells.push_back( temp );
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
	if ( grid->at(gridLoc.x).at(gridLoc.y).hasBug == true) {
		//...om nom nom
		for(auto itr = bugs.begin(); itr != bugs.end(); ++itr) {
			if(itr->getPosition() == gridLoc) {
				grid->at(gridLoc.x).at(gridLoc.y).hasBug = false;
				bugs.erase(itr);
				break;
			}
		}
	}
}

void Predator::Draw(RenderWindow &w) {
	w.draw(m_shape);
}