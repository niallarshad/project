#include "Bug.h"

Bug::Bug(Vector2i loc, float radius, std::vector<std::vector<Cell> > * grid) : gridLoc(loc), m_shape(radius), grid(grid) {
		m_shape.setPosition(gridLoc.x * GridSpacing, gridLoc.y * GridSpacing);
		m_shape.setFillColor(Color::Red);
		m_shape.setPointCount(2880);
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

	//tell the render window to draw the shape
	void Bug::Draw(sf::RenderWindow &w) {
		w.draw(m_shape);
	}