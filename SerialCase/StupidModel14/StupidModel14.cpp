////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#define GridSize 100
//#define BugCount 100
#define BugVision 4
#define GridSpacing 5

#include "Bug.h"
#include "Histogram.h"

using namespace sf;
 
//overload insertion operator so they we can cleanly output the HabitatCell to the console
ostream& operator<<(ostream& os, const HabitatCell &c) {
	os << "Cell " << (c.hasBug ? "has" : "doesn't have") << " a bug.\n";
	os << "Cell has " << c.foodAvailability << " food available. \n";
	return os;
}

vector<int> getBugDistribution(vector<Bug> bugs, unsigned int groups = 10 ) {
	vector<int> distro;
	distro.reserve( groups );

	//fill the vector with ints of 0
	for ( int i = 0; i < groups; i++ ) {
		distro.push_back( 0 );
	}

	//fill the groups with the bugs' sizes
	for ( int i = 0; i < bugs.size(); i++ ) {
		//get the size of each bug
		int size = floor( bugs[i].getSize() );

		if ( size <= groups ) {
			//increment the size bracket for this size
			distro[size]++;
		}
	}

	return distro;
}

int main()
{
	srand( time(NULL) );

	unsigned int noOfBugs = 100;
	float maxBugConsumeRate = 1.0f;
	float maxCellProduceRate = 0.01f;
	bool criticalMassReached = false;
	float initialBugSizeMean = 0.1f, initialBugSizeSD = 0.3f;

	float minSize, meanSize, maxSize;	//step 8; output to a file the minimum bugsize, the average bugsize and the max bugsize;

	try {
		cout << "Input desired number of bugs: ";
		cin >> noOfBugs;

		cout << "\nInput bug consume rate: ";
		cin >> maxBugConsumeRate;

		cout << "\nInput food production rate: ";
		cin >> maxCellProduceRate;

		cout << "\nInput starting bug size average: ";
		cin >> initialBugSizeMean;

		cout << "\nInput starting bug size standard deviation: ";
		cin >> initialBugSizeSD;
	}
	catch (...) {
		cout << "Invalid arguement!\nIma just use the defaults..." << endl;
	}

	Font font;
	font.loadFromFile( "../../kenvector_future.ttf" );

	// Create the main window
	RenderWindow window(VideoMode(800, 600, 32), "Stupid model 14");
	Histogram graph( 10, font );
		 
	std::vector<std::vector<HabitatCell> > grid;
	std::vector<Bug> bugs;

	grid.reserve( GridSize );
	bugs.reserve( noOfBugs );

	bool mousePressed = Mouse::isButtonPressed( Mouse::Button::Left );

	//fill the vectors with cells
	for( int x = 0; x < GridSize; x++ ) {

		grid.push_back( std::vector<HabitatCell>() );
		grid[x].reserve( GridSize );

		for( int y = 0; y < GridSize; y++ ) {

			grid.at( x ).push_back( HabitatCell(maxCellProduceRate) );

		}
	}

	//place bugs randomly in the grid
	for (int i = 0; i < noOfBugs; i++) {
		int x = rand() % GridSize,
			y = rand() % GridSize;

		if ( grid[x][y].hasBug != true ) {
			grid[x][y].hasBug = true;
			bugs.push_back( Bug(Vector2i(x,y), 1, &grid, maxBugConsumeRate) );

			//get a random float from 0.0 to twice the SD
			float deviation = static_cast<float>( rand() / (static_cast<float>(RAND_MAX / (initialBugSizeSD*2))) );

			deviation -= initialBugSizeSD;	//allow deviation to be from -SD to +SD

			bugs.rbegin()->setSize( max(initialBugSizeMean + deviation, 0.0f) );	//don't let size be less than 0
		}
		else {
			--i;
		}
	}

	Clock clock = Clock();


	ofstream logStream;
	logStream.open( "bugSizes.log" );
	logStream << "\n---------------------- Application Started ----------------" << endl;

	//START WINDOW LOOP
	while (window.isOpen())
	{
		// Process events
		sf::Event Event;
		while (window.pollEvent(Event))
		{
		// Close window : exit
		if (Event.type == sf::Event::Closed)
			window.close();
		// Escape key : exit
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
			window.close();
		}

		//output the values of cell on mouse button down (if button wasn't down last frame)
		if ( Mouse::isButtonPressed(Mouse::Button::Left) && !mousePressed) {
			mousePressed = true;

			try {
					Vector2i mousePos = Mouse::getPosition(window);
					cout << grid.at((int)(mousePos.x / GridSpacing)).at((int)(mousePos.y / GridSpacing));
			}
			catch (...) {
				std::cout << "No cell at this position" << std::endl;
			}
		}

		mousePressed = Mouse::isButtonPressed( Mouse::Button::Left );

		if ( clock.getElapsedTime().asSeconds() >= 0 ) {

			//tell all the HabitatCells to produce food
			for(std::vector<std::vector<HabitatCell>>::iterator itr = grid.begin();
				itr != grid.end();
				itr++)
			{
				for(std::vector<HabitatCell>::iterator itr2 = itr->begin();
				itr2 != itr->end();
				itr2++)
				{
					itr2->ProduceFood();
				}
			}

			//reset min and max so that we can find the new ones below...
			minSize = FLT_MAX;
			maxSize = -FLT_MAX;
			meanSize = 0;

			//sort the bugs in descending order by size;
			std::sort(bugs.rbegin(), bugs.rend());

			for(int i = 0; i < bugs.size(); i++) {
			
				bugs[i].Move();
				bugs[i].Grow();

				//if the bug fails the survival probability, it dies
				if ( bugs[i].Mortality() ) {
					vector<Bug>::iterator nth = bugs.begin() + i--;	//decrement i so that we don't skip next bug
					bugs.erase( nth );
					continue;
				}
				//if the bugs reaches a certain size, kill it and hatch it's eggs
				else {
					if ( bugs[i].IsReproducing(&bugs) ) {
						vector<Bug>::iterator nth = bugs.begin() + i--;	//decrement i so that we don't skip next bug
						bugs.erase( nth );
						continue;
					}
				}
					

				//...find min, mean and max bug sizes
				float size = bugs[i].getSize();
				if ( size < minSize )
					minSize = size;
				else if ( size > maxSize )
					maxSize = size;

				//add this bug's size to the average...
				meanSize += size;
			}

			//...divide by the number of bugs to get the average size
			meanSize /= noOfBugs;

			logStream << "Minimum Bug Size: " << minSize;
			logStream << "\t Average Bug Size: " << meanSize;
			logStream << "\t Maximum Bug Size: " << maxSize << endl;

			//if no bugs left or 1000 iteration passed...
			static int step;
			if ( bugs.size() == 0 || ++step >= 1000) {
				cout << bugs.size() << " bugs left. " << step << " iterations processed" << endl;
				window.close();
			}

			window.setTitle("Timestep: " + to_string(step));

			//restart the clock
			clock.restart();
		}

		//prepare frame
		window.clear();
		
		for(std::vector<Bug>::iterator itr = bugs.begin(); itr != bugs.end(); itr++)
		{
			itr->Draw(window);
		}

		window.display();

		//update and draw the graph
		graph.Update( getBugDistribution(bugs) );
		graph.Draw( Vector2i(window.getPosition().x + window.getSize().x, window.getPosition().y) );

	} //loop back for next frame

	logStream << "-------------------- Application closing ---------------------" << endl;
	logStream.close();
	system("PAUSE");

	return EXIT_SUCCESS;
}







