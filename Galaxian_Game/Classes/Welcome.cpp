#include <iostream>
#include "Allien.h"
#include "Welcome.h"
#include <SFML/Graphics.hpp>
#include <cmath>  
#include <string>
#include <SFML/Audio.hpp>
#include <vector>
#include <fstream>


using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

Welcome::Welcome(const sf::Vector2u & window_size)
    : numAlliens(6), // there are 6 differents alliens
      posLastAllien(0),
      posFirstAllien(0),
      maxScoresSaved(4),
      alliensEntranceComplete(false),
      showStartGameMessage(false)
{	
	posLastAllien =window_size.x - 200; // save where is the last position from the last allien
	posFirstAllien = 100;
	float distanceBetteenAllien = posLastAllien/numAlliens;
	float xPos=-64, yPos = 200; // first allien starts behind the window

	for(int i=0; i<numAlliens; i++){

		int specificAllien = numAlliens - i; // create an specific allien, with an specific img
		Allien * temp = new Allien(xPos,yPos, specificAllien);
		temp->setXVelocity(270);
    	alliens.push_back(temp);
    	xPos -= distanceBetteenAllien;
	}

}

void Welcome::moveAllAliens(float elapsedTimeSeconds , sf::RenderWindow & window)
{    		


	// first, check if any of the allien is already in the last available position in the screen
	if(not alliensEntranceComplete){
		for (int i = 0; i < alliens.size(); i++){

			if(alliens[i]->getPosition().x >=posLastAllien){
				alliensEntranceComplete = true;
				showStartGameMessage = true;
			}
		}
		// one of the allien is in the last available x position
		if(alliensEntranceComplete){

			// change direction then
			for (int i = 0; i < alliens.size(); i++){
				alliens[i]->setXVelocity( -std::abs(100) );
			}
		}
	}

	else{

		for (int i = 0; i < alliens.size(); i++){

			if(alliens[i]->getPosition().x <=posFirstAllien){
				alliensEntranceComplete = false;
				showStartGameMessage = true;
			}
		}

		// one of the allien is in the first available x position
		if(not alliensEntranceComplete){

			// change direction then
			for (int i = 0; i < alliens.size(); i++){
				alliens[i]->setXVelocity( std::abs(alliens[i]->getXVelocity()) );
			}
		}


	}

	// move all alliens
    for (int i = 0; i < alliens.size(); i++){
        alliens[i]->allien.move(elapsedTimeSeconds * alliens[i]->getXVelocity() , 0);
    }
	
}
void Welcome::drawAllAliens(sf::RenderWindow & window)
{
	// draw all alliens in their currently position
    for (int i = 0; i < alliens.size(); i++){
     
        string Alienpoints = std::to_string(alliens[i]->getPointsForPlayer());
        if(alliens[i]->getPointsForPlayer() ==1)
        	Alienpoints +=" Point";
        else
        	Alienpoints +=" Points";

        alliens[i]->draw(window);
        drawTextWindow(Alienpoints, alliens[i]->getPosition().x, alliens[i]->getPosition().y + 130 , window, 28,sf::Color::Red);

    }

    drawTextWindow("Super Galaxian Extreme", window.getSize().x /2 - 230, window.getSize().y /14, window, 32,sf::Color::Green);

    drawTextWindow("By: Thiago Santos.", window.getSize().x - 270, window.getSize().y -100, window, 22,sf::Color::White);
    drawTextWindow("Copyright. All rights reserved.", window.getSize().x - 330, window.getSize().y -50, window, 22,sf::Color::White);

    // if alliens if already set
	if(showStartGameMessage){

		drawTextWindow("Press Spacebar to start the game", window.getSize().x /2 - 230, window.getSize().y /2, window, 28, sf::Color::White);     

	}
    
}

void Welcome::drawBestScores(sf::RenderWindow & window){


	ofstream outfile;	
	ifstream infile;

	string *namesScores = new string[maxScoresSaved*2]; // an array of size == of the maximun scores saved, in this case, 4(4 names and 4 scores)
	float xNamePos = window.getSize().x /2 - 600;
	float yNamePos = window.getSize().y /1.3;

	float xScorePos = window.getSize().x /2 - 200;
	float yScorePos = window.getSize().y /1.3;

	int index =0;
	
	infile.open("scores.txt");
	

	if(!infile){

		//throw "File was create";
		outfile.open("scores.txt", ios::out);
		outfile.close();
	}

	else if (infile.is_open() && infile.good()){

		getline(infile, namesScores[index]);

		while(infile && index < maxScoresSaved *2){  // reading from the file

			index++;
			getline(infile, namesScores[index]);
		}

		if(index > 0){ // there's a name and score in the file
			drawTextWindow("Top Scores:", window.getSize().x /2 - 450, window.getSize().y /1.55, window, 34, sf::Color::Red);  
			drawTextWindow("Name:", window.getSize().x /2 - 600, window.getSize().y /1.4, window, 30, sf::Color::Green);  
			drawTextWindow("Score:", window.getSize().x /2 - 200, window.getSize().y /1.4, window, 30, sf::Color::Green);  
		}

		
		for(int i=0; i<index; i++){

			if( (i+1)%2 !=0){ // it's the name
				drawTextWindow(namesScores[i], xNamePos, yNamePos, window, 27, sf::Color::White);  
				yNamePos +=50;
				
			}
			else{

				drawTextWindow(namesScores[i], xScorePos, yScorePos, window, 27, sf::Color::White);  
				yScorePos +=50;

			}
		}

	}

	delete [] namesScores;

}

void Welcome::drawTextWindow(string txt, float xPos, float yPos, sf::RenderWindow & window, int size, sf::Color color){

    try{
        // Declare and load a font
        sf::Font font;
        if (!font.loadFromFile("../Fonts/Arial.ttf"))
        {
            cout<<"Erro"<<endl;
        }
        // Create a text
        sf::Text text(txt, font);
        text.setCharacterSize(size);
        text.setStyle(sf::Text::Bold);
        text.setColor(color);
        text.setPosition(xPos,yPos);
        // Draw it
        window.draw(text);
    }
    catch(std::exception e){

        cout<<"Error loading Font"<<endl;

    }
}


Welcome::~Welcome(){

	for (int i = 0; i < alliens.size(); i++)
        delete alliens[i];
}