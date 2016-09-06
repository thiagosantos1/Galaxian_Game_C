#ifndef MANAGEALLIEN_H
#define MANAGEALLIEN_H
 
#include "Bullet.h"
#include "ShipPlayer.h"
#include "Allien.h"
#include <vector>
#include <SFML/Graphics.hpp>

class ManageAlliens
{
public:
	ManageAlliens(const sf::Vector2u & window_size);
	~ManageAlliens();

	// everytime a allien moves, it checks if one of the bullets hits the allien
	void moveAll(float time, sf::RenderWindow & window, std::vector<Bullet *>  & bullets, ShipPlayer* player);
    void drawAll(sf::RenderWindow & window);
    void drawExplosion(); // draw the explosion when allien dies 
    //void newBullet(float timeElapsedTime);
    bool shouldCreateNewFleetOfAliens(); // check if alliens vector is empty, so then we need to create a new fleet of aliens
    void createNewFleetOfAliens(const sf::Vector2u & window_size);  // create the new fleet of aliens, getting down smoodly
    				// random choose the on of the allien to shoot

    // Alien randomly shoot, in a chance of 1/4, if the time since last shot is at least 3 seconds
    void attack(float elapsedTimeSeconds, const sf::RenderWindow & window, ShipPlayer* player);

    void newBullet(Allien * allien); // create a new bulle(one of the alliens had shoot)

    std::vector<Bullet *> & getBullets();
    std::vector<Allien *> & getAlliens();


private:


	std::vector<Allien *> alliens;
	std::vector<Bullet *> bullets;

	sf::RectangleShape rectanglePrimary; // rectangle to save the alien 1 in mode attack down
	sf::RectangleShape rectangleSecondary; // rectangle to save the alien 2 in mode attack down

	
    sf::SoundBuffer bufferExplosion; 
    sf::Sound soundAlienExplosion;

    sf::SoundBuffer bufferShoot;
    sf::Sound soundShoot;

    // save the explosion images
    std::vector<sf::Texture > explosionTextures;
	sf::Sprite explosion;


	// save position in the alliens vector that represents currently allien in attack down
	float  allienInAttackPosition;
	// save if there's any allien in attack down(allien can attack down just one allien by one, not two aliens at the same time)
	bool allienInAttack;
	std::string allienAttackDirection; // save with direction allien is pointing, in down attack(left, center, right)
	float elapsedTimeAttackShot; // how long has passed since last alliens attack shoot(alliens attack in )
	float elapsedTimeAttackDown; // how long has passed since last alliens attack down, trying to hit the player 
	float elapsedTimeExplosionDuration; // calculate the time of each explosion
	float xPosExplosion;
	float yPosExplosion;
	bool explosionModeOn;
	bool primaryRectangleModeOn; // save if the primary rectangle is beeing used, if so, we then use the secondary, for next allien
	bool secondaryRectangleModeOn;
	int numAlliens;
	float velocity_x_alliens;
};

#endif