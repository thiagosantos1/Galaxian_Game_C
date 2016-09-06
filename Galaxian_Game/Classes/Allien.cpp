#include <iostream>
#include <SFML/Graphics.hpp>
#include "Allien.h"
#include "Bullet.h"
#include "ShipPlayer.h"
#include <vector>
#include <cmath> 

using std::cout;
using std::endl;
using std::string;

Allien::Allien(float xPos, float yPos)
	: velocity_x(0.1), // all alliens start moving to the right, in a speed of 0.1px/s
	  velocity_y(0),
	  velocity_xAttack(0.1),
	  velocity_yAttack(0.1),
	  inAttack(false),
	  atackDone(false),
	  resetRectOldPosition(false),
	  pointsForPlayer(0)

	{
	try{

		string generalPath = "../Images/Aliens/"; // general path for the imgs file
		string path = "";
		int randTexture = rand() %6 +1;  // get a random index that represents one of the allien img
		string stringRandTexture = std::to_string(randTexture);
		path = generalPath + stringRandTexture + ".png";
		originalAllienTexturePath = path;
	    allienTexture.loadFromFile(path);

	    allien.setTexture(allienTexture);

	    //allien.setScale(100, 100);
	    allien.setPosition(xPos, yPos);

	    pointsForPlayer = randTexture; // All the aliens's names is a number, and this numbers represents how many points each 
	    							   // alien will give for user, when user kill him

	}
	catch(...){

		cout<<"Exception setting images" <<endl;
	}

	
} 

Allien::Allien(float xPos, float yPos, int specificAllien)
	: velocity_x(0.1), // all alliens start moving to the right, in a speed of 0.1px/s
	  velocity_y(0),
	  velocity_xAttack(0.1),
	  velocity_yAttack(0.1),
	  inAttack(false),
	  atackDone(false),
	  resetRectOldPosition(false),
	  pointsForPlayer(0)

	{
	try{

		string generalPath = "../Images/Aliens/"; // general path for the imgs file
		string path = "";
		path = generalPath + std::to_string(specificAllien) + ".png";
		originalAllienTexturePath = path;
	    allienTexture.loadFromFile(path);

	    allien.setTexture(allienTexture);

	    //allien.setScale(100, 100);
	    allien.setPosition(xPos, yPos);

	    pointsForPlayer = specificAllien; 

	}
	catch(...){

		cout<<"Exception setting images" <<endl;
	}

	
} 

const sf::Vector2f & Allien::getPosition(){

	return allien.getPosition();
}



void Allien::setXVelocity(float vx)
{
    velocity_x = vx;
}

void Allien::setYVelocity(float vy)
{
    velocity_y = vy;
}

float Allien::getXVelocity(){
	return velocity_x;
}
float Allien::getYVelocity(){
	return velocity_y;
}

void Allien::setPosition(float x, float y)
{
    allien.setPosition(x, y);
}


int Allien::getPointsForPlayer(){

	return pointsForPlayer;
}

float Allien::getVelocity_xAttack() const{

	return velocity_xAttack;

}

float Allien::getVelocity_yAttack() const{

	return velocity_yAttack;

}

void Allien::setInAttack(bool status){

	inAttack = status;	
}

bool Allien::getInAttack(){

	return inAttack;
}

void Allien::setAttackDone(bool status){

	atackDone = status;	
}

bool Allien::getAttackDone(){

	return atackDone;
}

// reset the image to the original - without rotation
void Allien::setOriginalImage(){

	allienTexture.loadFromFile(originalAllienTexturePath); 
	allien.setTexture(allienTexture);

}
void Allien::setRotation(float angle){

	allien.rotate(angle);

}

void Allien::setRectTrackerOldPosition(int rectOption){

	rectTrackerOldPosition = rectOption;	
}


int Allien::getRectTrackerOldPosition(){

	return rectTrackerOldPosition;
}

bool Allien::getResetRectOldPosition(){

	return resetRectOldPosition;

}
void Allien::setResetRectOldPosition(bool status){

	resetRectOldPosition = status;

}

void Allien::draw(sf::RenderWindow & window, sf::Sprite & explosion, bool explosionModeOn ) 
{
    window.draw(allien);
    // draw explosion if explosionModeOn in true( an allien was killed)
    if( explosionModeOn)
    	window.draw(explosion);
}

void Allien::draw(sf::RenderWindow & window) 
{
    window.draw(allien);
}

void Allien::move(float seconds, const sf::Vector2u & window_size,sf::RectangleShape & rectanglePrimary, sf::RectangleShape & rectangleSecondary)
{
    // move if it's not trying to be over wall
    
    // if allien attack is done, time to he back to the fleet
    if (atackDone){

    	sf::RectangleShape temp;
    	temp.setSize(sf::Vector2f(64, 64));

    	// rectanglePrimary has his old position
    	if(rectTrackerOldPosition ==1)
		    temp.setPosition(rectanglePrimary.getPosition().x, rectanglePrimary.getPosition().y);
		
		else
			temp.setPosition(rectangleSecondary.getPosition().x, rectangleSecondary.getPosition().y);
    	
		/*
	    // if allien x position is greather than his old x position, we set the x speed negative
	    if(allien.getPosition().x > temp.getPosition().x + 3){ // 2 is just to make easir to match the position

	        velocity_x = -std::abs(velocity_x);
	    }
	    // it's less, move positive
	    else if(allien.getPosition().x < temp.getPosition().x + -3){
	    	velocity_x = std::abs(velocity_x);
	    }
	    */
		// check if y position match as well

		if( (allien.getPosition().y > temp.getPosition().y)){//} and (allien.getPosition().y > 0) ){ // 2 is just to make easir to match the position

	        velocity_y = 0;
	        atackDone = false; // save the status of the allien attack
	        inAttack = false; // allien is no longer in attack anymore
	        resetRectOldPosition = true; // time to reset the rectangle that hold him
	       	//cout<<"HA  - "<<allien.getPosition().y<<endl;
	    }
	    else{

	    	resetRectOldPosition = false;
	    }

	    /*
	    // x and y position already matches
	    if( (allien.getPosition().x > temp.getPosition().x + 3) && (allien.getPosition().y > temp.getPosition().y)) { 


	        velocity_y = 0;
	        atackDone = false; // save the status of the allien attack
	        inAttack = false; // allien is no longer in attack anymore
	        resetRectOldPosition = true; // time to reset the rectangle that hold him
	        cout<<"haha"<<endl;
	    	

	    }
	    else {

	    	resetRectOldPosition = false;
	    }
		*/
	}

	float deltaX = seconds * velocity_x;
    float deltaY = seconds * velocity_y;

    allien.move(deltaX, deltaY);

    
    //setXVelocity(0); // has to get back to 0 to don't move the ship forever

}

int Allien::isCloseToWall(float xPos, const sf::Vector2u & window_size){

    
    // check if the allien is close to the left edge
    if (xPos < window_size.x * 0.05)  // if x position is less than 5% of the screen, it's time to change the moviment to the right
    	return 1;

    if (xPos > window_size.x - window_size.x * 0.08)
    	return 2;


    return false;

}



bool Allien::shouldBeDeleted(std::vector<Bullet *> & bullets){

	try{
		for (int i = 0; i < bullets.size(); i++)
	    {	
	    	// check if allien colide with any of the bullets
			if(allien.getGlobalBounds().intersects(bullets[i]->bullet.getGlobalBounds())){

				
				delete bullets[i]; // delete the bullet
	            bullets.erase(bullets.begin() + i);  // order the array again
	            i--;
				return true;
			}
		}
	}
	catch(...){
		cout<<"Error deleting bullet and allien"<<endl;
	}
        
	return false;
	
	
}

bool Allien::shouldBeDeleted(ShipPlayer * player){

	try{
		
    	// check if allien colide with any of the bullets
		return allien.getGlobalBounds().intersects(player->shipPlayer.getGlobalBounds());		
	
	}
	catch(...){
		cout<<"Error checking if allien hits player in attack mode"<<endl;
	}
        
	return false;


}


bool Allien::shouldReturnToFleet(const sf::Vector2u & window_size){

	if ( allien.getPosition().x < 0)
		return true;
	else if ( allien.getPosition().y < 0)
		return true;
	else if ( allien.getPosition().x + allienTexture.getSize().x  > window_size.x)
		return true;
	else if ( allien.getPosition().y + allienTexture.getSize().y  > window_size.y)
		return true;

	return false;
}










