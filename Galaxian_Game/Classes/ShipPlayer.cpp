#include <iostream>
#include <SFML/Graphics.hpp>
#include "ShipPlayer.h"
#include <SFML/Audio.hpp>
 
using std::cout;
using std::endl;

ShipPlayer::ShipPlayer(float xPos, float yPos)
	: velocity_x(0),
	  velocity_y(0),
      lives(2), // player always starts with 3 lives
      score(0), // start with 0 score
      fleetKilled(0),
	  elapsedTimeShoot(0)
{
	
    playerTexture.loadFromFile("../Images/Ship_player/ship_player_blue.png");
    shipPlayer.setTexture(playerTexture);
    //shipPlayer.setScale(100, 100);
    shipPlayer.setPosition(xPos, yPos);

    try{

        bufferShoot.loadFromFile("../Audio/shipShoot.wav");  
        bufferLoseLife.loadFromFile("../Audio/player_lose_life.wav");  
        bufferGameOver.loadFromFile("../Audio/gameOver.wav"); 
    
    }
    catch(...){

        cout<<"Error loading Player Shoot  audio"<<endl;
    }

    try{

        
        soundShoot.setBuffer(bufferShoot);
        soundLoseLife.setBuffer(bufferLoseLife);
        soundGameOver.setBuffer(bufferGameOver);
    }
    catch(...){

        cout<<"Error setting Player Shoot audio"<<endl;
    }
} 

const sf::Vector2f & ShipPlayer::getPosition(){

	return shipPlayer.getPosition();
}


void ShipPlayer::setXVelocity(float vx)
{
    velocity_x = vx;
}

void ShipPlayer::setPosition(float x, float y)
{
    shipPlayer.setPosition(x, y);
}

float ShipPlayer::getElapsedTime() const{

    return elapsedTimeShoot;
}

void ShipPlayer::setElapsedTime(float elapsedTime){

    elapsedTimeShoot = elapsedTime;

}

void ShipPlayer::setLives(int livesRemain){

    lives = livesRemain;
    if(lives == 0){

        soundGameOver.setVolume(10);
        soundGameOver.play();
    }
}

int ShipPlayer::getLives(){

    return lives;
}

void ShipPlayer::setScore(int points){

    score +=points;

}
int ShipPlayer::getScore(){

    return score;

}

sf::Sound & ShipPlayer::getSoundShoot() {

    return soundShoot;
}

sf::Sound & ShipPlayer::getSoundLoseLife() {

    return soundLoseLife;
}


void ShipPlayer::setFleetKilled(int fleets){

    fleetKilled = fleets;
}
int ShipPlayer::getFleetKilled(){
    return fleetKilled;

}

void ShipPlayer::draw(sf::RenderWindow & window, sf::Sprite & explosion, bool explosionModeOn)
{

    // draw explosion if explosionModeOn in true( player lost 1 life) 
    if(explosionModeOn){
        window.draw(shipPlayer);
        window.draw(explosion);
    }
    else if (lives >0){
        window.draw(shipPlayer);
    }
}

void ShipPlayer::move(float seconds, const sf::Vector2u & window_size)
{
    // move if it's not trying to be over wall
    
    float deltaX = seconds * velocity_x;
    float deltaY = seconds * velocity_y;

    shipPlayer.move(deltaX, deltaY);
    setXVelocity(0); // has to get back to 0 to don't move the ship forever

    hitWall(window_size); // check if hits the wall, and set the position to the edge 
    


}

void ShipPlayer::hitWall(const sf::Vector2u & window_size){

    
    bool hit_a_wall = false;

    float left = shipPlayer.getPosition().x;
    if (left < 0)
    {
        shipPlayer.setPosition(0,shipPlayer.getPosition().y);
    }

   

    float right = left + shipPlayer.getGlobalBounds().width;
    if (right > window_size.x)
    {
        shipPlayer.setPosition(window_size.x - shipPlayer.getGlobalBounds().width,shipPlayer.getPosition().y);
    }



}



bool ShipPlayer::allienShootHit(Bullet  * bullet){

    try{
         
        // check if player colide with any of the bullets
        return shipPlayer.getGlobalBounds().intersects(bullet->bullet.getGlobalBounds());
        
    }
    catch(...){
        cout<<"Error checking bullet and player collision"<<endl;
    }
        
    return false;


}







