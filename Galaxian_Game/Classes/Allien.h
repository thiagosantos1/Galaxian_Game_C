#ifndef ALLIEN_H
#define ALLIEN_H

#include <iostream>
#include "Bullet.h"
#include "ShipPlayer.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio.hpp>

class Allien
{
public:
	Allien(float xPos, float yPos);
	Allien(float xPos, float yPos, int specificAllien);
	void setPosition(float xPos, float yPos);
	void setXVelocity(float vx);
	void setYVelocity(float vy);
	float getXVelocity();
	float getYVelocity();
	const sf::Vector2f & getPosition();
	void move(float seconds, const sf::Vector2u & window_size, sf::RectangleShape & rectanglePrimary, sf::RectangleShape & rectangleSecondary);
	void draw(sf::RenderWindow & window, sf::Sprite & explosion, bool explosionModeOn);
	void draw(sf::RenderWindow & window);
	float getElapsedTime() const;
	void setElapsedTime(float elapsedTime);
	int isCloseToWall(float xPos, const sf::Vector2u & window_size); // return 1 if alien is close to left edge and 2 if to the right
	bool shouldBeDeleted(std::vector<Bullet *>  & bullets);  // allien should be delete because player shoot hited him
	bool shouldBeDeleted(ShipPlayer * player);  // allien is delete if hits player when he goes down, in attack mode
	bool shouldReturnToFleet(const sf::Vector2u & window_size); // allien returns to fleet when he cross the edges
	int getPointsForPlayer(); // get how many points user gives for player when player kill him

	float getVelocity_xAttack() const;
	float getVelocity_yAttack() const;
	void setInAttack(bool status);
	bool getInAttack();
	void setAttackDone(bool status);
	bool getAttackDone();
	void setOriginalImage();
	void setRotation(float angle);
	void setRectTrackerOldPosition(int rectOption);
	int getRectTrackerOldPosition();
	bool getResetRectOldPosition();
	void setResetRectOldPosition(bool status);

	sf::Sprite allien;
protected:



	sf::Texture allienTexture; // use to rotate img
	std::string originalAllienTexturePath; // save the original img with the original rotation
	
	float velocity_x;
	float velocity_y;
	int pointsForPlayer; // save how many points the specific allien will give for player, if he kill him
	bool inAttack; // save if allien is in attack
	bool atackDone; // save if alien atack is done
	bool resetRectOldPosition; // save if manage should reset the rect that had his old position recored
	// to release to next allien to use the same rectangle

	// save the speed of the allien attack, when he goes down, tryig to catch player
	float velocity_xAttack;
	float velocity_yAttack;
	int rectTrackerOldPosition; // save each of the 2 rectangles is saving his old position. 1 to rectanglePrimary, 2 to rectangleSecondary
	
};

#endif