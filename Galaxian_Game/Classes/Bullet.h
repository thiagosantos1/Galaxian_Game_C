#ifndef BULLET_H
#define BULLET_H
#include <SFML/Graphics.hpp>

class Bullet
{
public:
	Bullet(float xPos, float yPos, float xVel, float yVel, std::string type); // type means, who is this bullet from, player or allien
	bool offScreen(const sf::RenderWindow & window) const;
	void draw(sf::RenderWindow & window) const;
	void move(float seconds, const sf::Vector2u & window_size);
	void setVelocity(float vx, float vy);
	float getYVelocity() const;
	float getXVelocity() const;

	bool shouldBeDeleted(const sf::RenderWindow & window);


	//~Bullet();

	sf::Sprite bullet;

protected:

	sf::Texture bulletTexture;
	float xVelocity;
	float yVelocity;
	bool shoudBeDeleted;
	float speed;
	
};


#endif