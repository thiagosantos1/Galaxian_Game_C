#ifndef MANAGERPLAYER_H 
#define MANAGERPLAYER_H 

#include "Bullet.h"
#include "ShipPlayer.h"
#include "Allien.h"
#include <vector>
#include <SFML/Graphics.hpp>

class ManagerPlayer
{
public:
	ManagerPlayer(float playerXposition, float playerYposition);
	~ManagerPlayer();

	void moveAll(float time, const sf::RenderWindow & window);
    void drawAll(sf::RenderWindow & window);
    ShipPlayer * getPlayer() ;
    // method will be called everytime player hits space to shoot a new buller
    void newBullet(float timeElapsedTime);
    std::vector<Bullet *> & getPlayerBullets();
    void drawExplosion();

    // check all the time if player lost a live - by get hit from allien bullet, or allien hit player by it self
    void checkLives(float elapsedTime, std::vector<Bullet *>  & bullets, std::vector<Allien *>  & allien);

private:

	std::vector<Bullet *> bullets;

    // save the explosion images
    std::vector<sf::Texture > explosionTextures;
    sf::Sprite explosion;

    float elapsedTimeExplosionDuration;
    bool explosionModeOn;

	ShipPlayer * player;

	
};

#endif