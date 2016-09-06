#ifndef SHIPPLAYER_H
#define SHIPPLAYER_H

#include "Bullet.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class ShipPlayer 
{
public:
	//ShipPlayer();
	ShipPlayer(float xPos, float yPos);
	//~ShipPlayer();

	void setPosition(float xPos, float yPos);
	void setXVelocity(float vx);
	const sf::Vector2f & getPosition();
	void move(float seconds, const sf::Vector2u & window_size);
	void draw(sf::RenderWindow & window, sf::Sprite & explosion, bool explosionModeOn);
	float getElapsedTime() const;
	void setElapsedTime(float elapsedTime);
	void hitWall(const sf::Vector2u & window_size);
	void setLives(int livesRemain);
	int getLives();
	void setScore(int points);
	int getScore();

	void setFleetKilled(int fleets);
	int getFleetKilled();

	sf::Sound & getSoundShoot(); // get the shoot sound

	sf::Sound & getSoundLoseLife();

	// check if any of the alliens bullets hit player, then remove a bullet and take of the player live
	bool allienShootHit(Bullet  * bullet);


	sf::Sprite shipPlayer;
protected:

	
	sf::Texture playerTexture;

	sf::SoundBuffer bufferShoot;
    sf::Sound soundShoot;

    sf::SoundBuffer bufferLoseLife;
    sf::Sound soundLoseLife;

    
    sf::SoundBuffer bufferGameOver;
    sf::Sound soundGameOver;


	float elapsedTimeShoot; // how long has pass since last shoot
	float velocity_x;  // move just on x Direction
	float velocity_y;
	int lives;
	int score;
	int fleetKilled;  // how many alliens fleet player killed
	
};




#endif