#include "stdafx.h"

#include "SFML/OpenGL.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

//#define GridSize 100
////#define BugCount 100
//#define BugVision 4
//#define GridSpacing 5

#include "Bug.h"
#include "Predator.h"
#include "Histogram.h"
#include "tree.hh"
#include <omp.h>

using namespace sf;
 
//overload insertion operator so they we can cleanly output the HabitatCell to the console
ostream& operator<<(ostream& os, const HabitatCell &c) {
	os << "Cell " << (c.hasBug ? "has" : "doesn't have") << " a bug.\n";
	os << "Cell " << (c.hasPredator ? "has" : "doesn't have") << " a predator.\n";
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

//splits a string by delimiter
vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
	vector<string> elems;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

//create a 2D grid of cells from a file
void createGridFromFile( string path, vector<vector<HabitatCell>> &grid, Vector2u &size) {

	
	ifstream f( path );	//open the file
	string line;
		
	//for each line in the file
	for (int i = 0; getline(f, line); ++i) {
		
		//ignore header info
		if(line.at(0) == '#')
			continue;
		
		//grid.push_back( vector<HabitatCell>() );
		vector<string> s = split(line, '\t');	//split the line

		if(s[0] == "uGridSize") {
			size = Vector2u(stoi(s[1]), stoi(s[2]));

			//make an empty grid
			for( int x = 0; x < size.x; x++ ) {

				grid.push_back( std::vector<HabitatCell>() );
				grid[x].reserve( size.x );

				for( int y = 0; y < size.y; y++ ) {

					grid.at( x ).push_back( HabitatCell(x, y, NULL) );

				}
			}
			continue;
		}

		int x = stoi( s[0] );
		int y = stoi( s[1] );
		float food = stof(s[2]);

		//don't allow entries outside of the grid!
		if(x == size.x || y == size.y)
			continue;

		grid[x][y] = HabitatCell(x, y, food);
			
		//for ( int j = 0; j < s.size(); ++j) {
		//	grid.at(j).push_back(HabitatCell(stof(s.at(i))));	//std::stof converts a string to a float
		//}
	}
}

int main()
{
	srand( time(NULL) );
	Vector2u GridSize;

	unsigned int noOfBugs = 100;
	unsigned int noOfPreds = 200;
	float maxBugConsumeRate = 1.0f;
	//float maxCellProduceRate = 0.01f;
	bool criticalMassReached = false;
	float initialBugSizeMean = 0.1f, initialBugSizeSD = 0.3f;

	float minSize, meanSize, maxSize;	//step 8; output to a file the minimum bugsize, the average bugsize and the max bugsize;

	try {
		cout << "Input desired number of bugs: ";
		cin >> noOfBugs;

		cout << "\nInput bug consume rate: ";
		cin >> maxBugConsumeRate;

		cout << "\nInput desired number of predators: ";
		cin >> noOfPreds;

		//cout << "\nInput food production rate: ";
		//cin >> maxCellProduceRate;

		cout << "\nInput starting bug size average: ";
		cin >> initialBugSizeMean;

		cout << "\nInput starting bug size standard deviation: ";
		cin >> initialBugSizeSD;
	}
	catch (...) {
		cout << "Invalid arguement!\nIma just use the defaults..." << endl;
	}

	Font font;
	font.loadFromFile( "../kenvector_future.ttf" );

	// Create the main window
	RenderWindow window(VideoMode(1000, 600, 32), "Stupid model 16");
	Histogram graph( 10, font );
	
		 
	std::vector<std::vector<HabitatCell> > grid;
	std::vector<Bug> bugs;
	std::vector<Predator> predators;

	bugs.reserve( noOfBugs );
	predators.reserve( noOfPreds );

	bool mousePressed = Mouse::isButtonPressed( Mouse::Button::Left );

	createGridFromFile("../Stupid_Cell.DATA", grid, GridSize);
	grid.reserve( GridSize.x );

	tree<pair<Vector2u, Vector2u>> myTree;
	auto root = myTree.begin();
	myTree.insert(root, make_pair(Vector2u(0,0), GridSize));

	//place bugs randomly in the grid
	for (int i = 0; i < noOfBugs; i++) {
		int x = rand() % GridSize.x,
			y = rand() % GridSize.y;

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

	//place predators randomly in the grid
	for (int i = 0; i < noOfPreds; i++) {
		int x = rand() % GridSize.x,
			y = rand() % GridSize.y;

		if ( grid[x][y].hasPredator != true ) {
			grid[x][y].hasPredator = true;
			predators.push_back( Predator(Vector2i(x,y), GridSpacing, &grid) );
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

			//tell all the HabitatCells to produce food (and draw them to the window)
			for(std::vector<std::vector<HabitatCell>>::iterator itr = grid.begin();
				itr != grid.end();
				itr++)
			{
				for(std::vector<HabitatCell>::iterator itr2 = itr->begin();
				itr2 != itr->end();
				itr2++)
				{
					itr2->ProduceFood();
					//itr2->Draw( window );
				}
			}

			//reset min and max so that we can find the new ones below...
			minSize = FLT_MAX;
			maxSize = -FLT_MAX;
			meanSize = 0;

			//sort the bugs in descending order by size;
			std::sort(bugs.rbegin(), bugs.rend());
// start 4 threads to run the move command


			int maxT = omp_get_max_threads();

			if(GridSize.x / 16 < maxT)
			{
				maxT = GridSize.x / 16;
			}

			if(maxT % 2 == 1)
			{
				maxT = maxT - 1;
			}
			cout << maxT << endl;
#pragma omp parallel num_threads(maxT)
			{
				int xStart = 0, yStart = 0, xEnd = GridSize.x / (maxT/2), yEnd = GridSize.y / 2;
				int id = omp_get_thread_num();
				// if its the first thread
				if(id == 0)
				{
					cout << "Your on first thread "<<endl;
				}
	
				// if its the last thread
				else if(id == maxT - 1)
				{
					xStart = xEnd;
					yStart = yEnd;
					xEnd = GridSize.x;
					yEnd = GridSize.y;
					cout << id << " is doing stuff "<<endl;
				}
	
				// if its on the top row
				else if(id < maxT/2)
				{
					xStart = xEnd * id;
					xEnd = (GridSize.x / (maxT/2)) * (id + 1);
					cout << id << " is doing stuff "<<endl;
				}
	
				//on bottom row
				else if(id >= (maxT/2))
				{
					xStart = 0;
					yStart = GridSize.y / 2;
					xEnd = GridSize.x / (maxT/2);
					yEnd = GridSize.y;
		
					//first element
					if(id == (maxT/2))
					{
						cout << "Your on first thread second line "<<endl;
					}
					else //if(id >= (maxT/2))
					{
						xStart = (GridSize.x / (maxT/2)) * ((id - (maxT/2)));
						xEnd = (GridSize.x / (maxT/2)) * ((id - (maxT/2)) + 1);
						cout << id << " is doing stuff "<<endl;
					}
		
				}
				 /////////////////////////////////////////////////////////////////////////////
				// Work out if the cell has a bug in it or not and move the bug if it does //
			   /////////////////////////////////////////////////////////////////////////////
   
			   for(int x = xStart; x < xEnd; ++x) 
			   { 
					for(int y = yStart; y < yEnd; ++y)
					{
						if(grid[x][y].hasBug)
						{
							for(int j= 0;j < bugs.size(); ++j) 
							{
								Vector2i bPos = bugs[j].getPosition();
								if(bPos.x == x && bPos.y == y && bugs[j].canMove())
								{
									bugs[j].Move(GridSize);
								}//end if not paralyzed
							}//end for bugsies
						}//end if has bug
						#pragma omp barrier
					}//end suby for
				}//end subx for
	
				cout << "Thread " << id << " end doing stuff" << endl;
			}//end omp parrellogram

		
			for(int i = 0; i < bugs.size(); i++) {
				
				//bugs[i].Move(GridSize);
					
				bugs[i].Grow();
				
				//if the bug fails the survival probability, it dies
				if ( bugs[i].Mortality() ) {
					Vector2i loc = bugs[i].getPosition();
					grid[loc.x][loc.y].hasBug = false;
					vector<Bug>::iterator nth = bugs.begin() + i--;	//decrement i so that we don't skip next bug
					bugs.erase( nth );
					continue;
				}
				//if the bugs reaches a certain size, kill it and hatch it's eggs
				else {
					if ( bugs[i].IsReproducing(&bugs, GridSize)) {
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

				bugs[i].setCanMove(true);
			}
		
			
			for(auto itr = predators.begin(); itr != predators.end(); ++itr) {
				itr->Hunt(bugs, GridSize);
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

		for (auto x = grid.begin(); x != grid.end(); x++) {
			for (auto y = x->begin(); y != x->end(); y++) {
				y->Draw(window);
			}
		}
		
		for(auto itr = bugs.begin(); itr != bugs.end(); itr++)
		{
			itr->Draw(window);
		}
		for(auto itr = predators.begin(); itr != predators.end(); itr++)
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







