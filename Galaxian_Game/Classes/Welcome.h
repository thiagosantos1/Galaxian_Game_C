#ifndef WELCOME_H
#define WELCOME_H

#include <iostream>
#include "Allien.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

using std::cout;
using std::endl;
using std::string;

class Welcome
{
public:
	Welcome(const sf::Vector2u & window_size);
	void drawAllAliens(sf::RenderWindow & window);
	void moveAllAliens(float elapsedTimeSeconds, sf::RenderWindow & window);
	void drawTextWindow(string txt, float xPos, float yPos, sf::RenderWindow & window, int size, sf::Color color);
	void drawBestScores(sf::RenderWindow & window);
	~Welcome();

private:

	std::vector<Allien *> alliens;

	int numAlliens; // how many differents alliens in the game
	const int maxScoresSaved;
	float posLastAllien;
	float posFirstAllien;
	bool alliensEntranceComplete; // save if the "animation" of the alliens, in the entrance, is done
	bool showStartGameMessage; // when allien entrace is done, show the message Press space to start
};


#endif