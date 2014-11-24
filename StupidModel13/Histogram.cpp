#include "Histogram.h"

Histogram::Histogram(unsigned int numberOfBins, sf::Font const &font) {
		window.create(sf::VideoMode(400, 600, 32), "Histogram");

		bins.reserve( numberOfBins );
		values.reserve( numberOfBins );

		for( int i = 0; i < numberOfBins; i++) {

			bins.push_back( sf::RectangleShape() );

			//space bins evently across window
			bins[i].setSize( sf::Vector2f( window.getSize().x / numberOfBins, 500 ) );

			values.push_back( sf::Text("ERROR", font) );	

			sf::Color color(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55);
			bins[i].setFillColor( color );
			values[i].setColor( color );
		}
}

void Histogram::Update(vector<int> const newValues) {

	//update the graph with new values
	for( int i = 0; i < bins.size(); i++) {
		bins[i].setSize( sf::Vector2f(bins[i].getSize().x, newValues[i]) );

		//place bins at bottom of screen
		bins[i].setPosition( i * bins[i].getSize().x , window.getSize().y - bins[i].getSize().y);

		values[i].setPosition( sf::Vector2f( bins[i].getPosition().x, bins[i].getPosition().y - 50 ) );
		values[i].setString( to_string( (int)(bins[i].getSize().y) ) );
	}
}

void Histogram::Draw(sf::Vector2i windowPosition) {
	//if the graph window is not joined to the parent window...
	if ( window.getPosition() != windowPosition) {
		//...fix that!
		window.setPosition( windowPosition );
	}

	window.clear();

	//draw each bin to the window
	for( int i = 0; i < bins.size(); i++ ) {
		window.draw( bins[i] );
		//window.draw( values[i] );
	}

	//draw each value to the window (above the bar, don't let them be covered))
	for( int i = 0; i < values.size(); i++ ) {
		//window.draw( bins[i] );
		window.draw( values[i] );
	}

	window.display();
}