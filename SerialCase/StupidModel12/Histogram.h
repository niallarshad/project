#ifndef _HISTOGRAM_H
#define _HISTOGRAM_H

#include <vector>
#include "SFML/Graphics.hpp"

using namespace std;

class Histogram {
private:
	vector<sf::RectangleShape> bins;
	vector<sf::Text> values;
	sf::RenderWindow window;

public:
	Histogram(unsigned int numberOfBins, sf::Font const &font);
	void Update(vector<int> const newValues);
	void Draw(sf::Vector2i windowPosition);
};
#endif