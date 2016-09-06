#include <iostream>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include <string>

using std::cout;
using std::endl;
using std::string;
				
				// x and y positions come from the playerShip position
Bullet::Bullet(float xPos, float yPos, float xVel, float yVel, string type)
	: xVelocity(xVel),
	  yVelocity(yVel),
	  shoudBeDeleted(false)
{
	if(type == "player")
		bulletTexture.loadFromFile("../Images/Bullet/blue_bullet.png");
	else if(type == "alien")
		bulletTexture.loadFromFile("../Images/Bullet/red_bullet.png");

    bullet.setTexture(bulletTexture);
    bullet.setPosition(xPos, yPos);
} 

void Bullet::setVelocity(float vx, float vy)
{
    xVelocity = vx;
    yVelocity = vy;
}

float Bullet::getYVelocity()const 
{ 

	return yVelocity;
}

float Bullet::getXVelocity()const 
{ 

	return xVelocity;
}


bool Bullet::offScreen(const sf::RenderWindow & window) const
{

	if ( bullet.getPosition().x < 0)
		return true;
	else if ( bullet.getPosition().y < 0)
		return true;
	else if ( bullet.getPosition().x + bulletTexture.getSize().x  > window.getSize().x)
		return true;
	else if ( bullet.getPosition().y + bulletTexture.getSize().y  > window.getSize().y)
		return true;

	return false;

}

bool Bullet::shouldBeDeleted(const sf::RenderWindow & window)
{
	shoudBeDeleted = offScreen(window);
    return shoudBeDeleted;
}

void Bullet::move(float seconds, const sf::Vector2u & window_size)
{

    float next_x = seconds * xVelocity;
    float next_y = seconds * yVelocity;

  
    bullet.move(next_x, next_y);

}

void Bullet::draw(sf::RenderWindow & window) const
{
    window.draw(bullet);
}