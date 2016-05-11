//#define SFML_STATIC

#include <iostream>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include <string>

class Object
{
public:
	sf::RectangleShape body;
	float bottom, right, left, top;
	sf::String tag;

	Object()
	{
		
	}

	//Sets the object's properties
	void Set(sf::Vector2f position, sf::Vector2f size, sf::Color color)
	{
		body.setPosition(position);
		body.setSize(size);
		body.setFillColor(color);
	}

	//Check for collision and return a boolean
	bool Collision(Object &hit)
	{
		if (hit.body.getGlobalBounds().intersects(body.getGlobalBounds()))
		{
		return true;
		}
		
		return false;
	}

};

class Key : public Object
{
public:
	bool isCollected = false;
};

class Door : public Object
{
public: 
	bool isUnlocked = false;
};

class Player : public Object
{
public:
	Animation walking;
	Animation idle;
	AnimatedSprite playerAnim;
	bool isWalking = false;
	bool isDead = false;

	//Handle player input and update animation
	void Tick(sf::Time deltaTime, float deltaFloat, float speed) 
	{
		//Check for keyboard input and move the player
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			body.move(-speed * deltaFloat, 0);
			isWalking = true;
		}
		else 
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				body.move(speed * deltaFloat, 0);
				isWalking = true;
			}
			else 
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					body.move(0, -speed * deltaFloat);
					isWalking = true;
				}
				else 
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					{
						body.move(0, speed * deltaFloat);
						isWalking = true;
					}
					else 
					{
						isWalking = false;
					}
				}
			}
		}
		//Update animation states
		if (isWalking)
		{
			playerAnim.setAnimation(walking);
			playerAnim.play();
		}
		else
		{
			playerAnim.setAnimation(idle);
		}
		//Sets animation sprite to player position
		playerAnim.setPosition(body.getPosition());
	}

	//Kills the player 
	void Kill()
	{
		isDead = true;
	}
	//Check for a collision with key objects
	bool KeyCollision(Key &hit)
	{
		if (hit.body.getGlobalBounds().intersects(body.getGlobalBounds()))
		{
			return true;
		}

		return false;
	}

	//Check for a collision with door objects
	bool DoorCollision(Door &hit)
	{
		if (hit.body.getGlobalBounds().intersects(body.getGlobalBounds()))
		{
			return true;
		}

		return false;
	}

	//Load and set up frames for the animations
	void LoadAnimation(sf::Texture &walkingTexture, sf::Texture &idleTexture) 
	{
		walking.setSpriteSheet(walkingTexture);
		idle.setSpriteSheet(idleTexture);
		walking.addFrame(sf::IntRect(1, 0, 5, 8));
		walking.addFrame(sf::IntRect(9, 0, 5, 8));
		walking.addFrame(sf::IntRect(17, 0, 5, 8));
		walking.addFrame(sf::IntRect(25, 0, 5, 8));
		idle.addFrame(sf::IntRect(1, 0, 5, 8));
		idle.addFrame(sf::IntRect(9, 0, 5, 8));
		idle.addFrame(sf::IntRect(17, 0, 5, 8));
		idle.addFrame(sf::IntRect(25, 0, 5, 8));
		playerAnim.setScale(6.25, 4);
	}

	//The player constructor
	Player(sf::Vector2f position, sf::Vector2f size, sf::Color color) /*: Object(position, size, color)*/
	{
		body.setPosition(position);
		body.setSize(size);
		body.setFillColor(color);
	}
};

class Enemy : public Object
{
public: 
	void Move(sf::Vector2f magnitude) 
	{
		body.move(magnitude);
	}
};


int main()
{

	//game setup and initialization
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	sf::Clock deltaClock;
	int gameState = 2;
	sf::Vector2f lastPos;
	sf::Time timesinceFrame;
	float deltaTime;

	//Instantiate the walls of the level into an array
	Object walls[72];
	//Set up all the walls of the game 
	walls[0].Set(sf::Vector2f(0, 0), sf::Vector2f(850, 50), sf::Color::White);
	walls[1].Set(sf::Vector2f(0, 50), sf::Vector2f(50, 800), sf::Color::White);
	walls[2].Set(sf::Vector2f(50, 150), sf::Vector2f(600, 50), sf::Color::White);
	walls[3].Set(sf::Vector2f(650, 150), sf::Vector2f(50, 250), sf::Color::White);
	walls[4].Set(sf::Vector2f(800, 50), sf::Vector2f(50, 350), sf::Color::White);
	walls[5].Set(sf::Vector2f(450, 350), sf::Vector2f(200, 50), sf::Color::White);
	walls[6].Set(sf::Vector2f(450, 400), sf::Vector2f(50, 300), sf::Color::White);
	walls[7].Set(sf::Vector2f(0, 850), sf::Vector2f(500, 50), sf::Color::White);
	walls[8].Set(sf::Vector2f(850, 350), sf::Vector2f(600, 50), sf::Color::White);
	walls[9].Set(sf::Vector2f(750, 600), sf::Vector2f(700, 50), sf::Color::White);
	walls[10].Set(sf::Vector2f(700, 600), sf::Vector2f(50, 100), sf::Color::White);
	walls[11].Set(sf::Vector2f(1000, 650), sf::Vector2f(50, 350), sf::Color::White);
	walls[12].Set(sf::Vector2f(750, 1000), sf::Vector2f(300, 50), sf::Color::White);
	walls[13].Set(sf::Vector2f(700, 850), sf::Vector2f(50, 1200), sf::Color::White);
	walls[14].Set(sf::Vector2f(450, 900), sf::Vector2f(50, 1150), sf::Color::White);
	walls[15].Set(sf::Vector2f(700, 2050), sf::Vector2f(200, 50), sf::Color::White);
	walls[16].Set(sf::Vector2f(0, 2050), sf::Vector2f(500, 50), sf::Color::White);
	walls[17].Set(sf::Vector2f(0, 2100), sf::Vector2f(50, 650), sf::Color::White);
	walls[18].Set(sf::Vector2f(900, 2050), sf::Vector2f(50, 400), sf::Color::White);
	walls[19].Set(sf::Vector2f(0, 2750), sf::Vector2f(950, 50), sf::Color::White);
	walls[20].Set(sf::Vector2f(900, 2650), sf::Vector2f(50, 100), sf::Color::White);
	walls[21].Set(sf::Vector2f(200, 2250), sf::Vector2f(500, 50), sf::Color::White);
	walls[22].Set(sf::Vector2f(700, 2250), sf::Vector2f(50, 300), sf::Color::White);
	walls[23].Set(sf::Vector2f(200, 2300), sf::Vector2f(50, 250), sf::Color::White);
	walls[24].Set(sf::Vector2f(200, 2550), sf::Vector2f(250, 50), sf::Color::White);
	walls[25].Set(sf::Vector2f(600, 2550), sf::Vector2f(150, 50), sf::Color::White);
	walls[26].Set(sf::Vector2f(1400, 250), sf::Vector2f(50, 100), sf::Color::White);
	walls[27].Set(sf::Vector2f(1450, 250), sf::Vector2f(1400, 50), sf::Color::White);
	walls[28].Set(sf::Vector2f(2800, 300), sf::Vector2f(50, 400), sf::Color::White);
	walls[29].Set(sf::Vector2f(1450, 700), sf::Vector2f(1400, 50), sf::Color::White);
	walls[30].Set(sf::Vector2f(1400, 650), sf::Vector2f(50, 100), sf::Color::White);
	walls[31].Set(sf::Vector2f(950, 2400), sf::Vector2f(650, 50), sf::Color::White);
	walls[32].Set(sf::Vector2f(1750, 2400), sf::Vector2f(650, 50), sf::Color::White);
	walls[33].Set(sf::Vector2f(1550, 2250), sf::Vector2f(50, 150), sf::Color::White);
	walls[34].Set(sf::Vector2f(1750, 2250), sf::Vector2f(50, 150), sf::Color::White);
	walls[35].Set(sf::Vector2f(950, 2650), sf::Vector2f(1450, 50), sf::Color::White);
	walls[36].Set(sf::Vector2f(1800, 2250), sf::Vector2f(150, 50), sf::Color::White);
	walls[37].Set(sf::Vector2f(1450, 2250), sf::Vector2f(100, 50), sf::Color::White);
	walls[38].Set(sf::Vector2f(1400, 2000), sf::Vector2f(50, 300), sf::Color::White);
	walls[39].Set(sf::Vector2f(1900, 1950), sf::Vector2f(50, 300), sf::Color::White);
	walls[40].Set(sf::Vector2f(1400, 1950), sf::Vector2f(500, 50), sf::Color::White);
	walls[41].Set(sf::Vector2f(2400, 2050), sf::Vector2f(50, 450), sf::Color::White);
	walls[42].Set(sf::Vector2f(2400, 2600), sf::Vector2f(50, 550), sf::Color::White);
	walls[43].Set(sf::Vector2f(2450, 2050), sf::Vector2f(1400, 50), sf::Color::White);
	walls[44].Set(sf::Vector2f(2450, 2450), sf::Vector2f(150, 50), sf::Color::White);
	walls[45].Set(sf::Vector2f(2550, 2500), sf::Vector2f(50, 500), sf::Color::White);
	walls[46].Set(sf::Vector2f(2450, 3100), sf::Vector2f(600, 50), sf::Color::White);
	walls[47].Set(sf::Vector2f(2600, 2950), sf::Vector2f(150, 50), sf::Color::White);
	walls[48].Set(sf::Vector2f(2600, 2800), sf::Vector2f(150, 50), sf::Color::White);
	walls[49].Set(sf::Vector2f(2700, 2700), sf::Vector2f(50, 100), sf::Color::White);
	walls[50].Set(sf::Vector2f(2850, 2750), sf::Vector2f(50, 350), sf::Color::White);
	walls[51].Set(sf::Vector2f(2850, 2700), sf::Vector2f(50, 100), sf::Color::White);
	walls[52].Set(sf::Vector2f(3000, 2850), sf::Vector2f(50, 250), sf::Color::White);
	walls[53].Set(sf::Vector2f(3050, 2400), sf::Vector2f(50, 500), sf::Color::White);
	walls[54].Set(sf::Vector2f(2900, 2550), sf::Vector2f(150, 50), sf::Color::White);
	walls[55].Set(sf::Vector2f(2850, 2450), sf::Vector2f(50, 150), sf::Color::White);
	walls[56].Set(sf::Vector2f(2850, 2700), sf::Vector2f(100, 50), sf::Color::White);
	walls[57].Set(sf::Vector2f(2700, 2350), sf::Vector2f(50, 250), sf::Color::White);
	walls[58].Set(sf::Vector2f(2600, 2300), sf::Vector2f(550, 50), sf::Color::White);
	walls[60].Set(sf::Vector2f(3000, 2350), sf::Vector2f(50, 100), sf::Color::White);
	walls[61].Set(sf::Vector2f(2550, 2200), sf::Vector2f(50, 150), sf::Color::White);
	walls[62].Set(sf::Vector2f(2700, 2100), sf::Vector2f(50, 100), sf::Color::White);
	walls[63].Set(sf::Vector2f(2850, 2200), sf::Vector2f(50, 100), sf::Color::White);
	walls[64].Set(sf::Vector2f(3000, 2100), sf::Vector2f(50, 100), sf::Color::White);
	walls[65].Set(sf::Vector2f(3800, 2100), sf::Vector2f(50, 850), sf::Color::White);
	walls[66].Set(sf::Vector2f(3650, 2950), sf::Vector2f(200, 50), sf::Color::White);
	walls[67].Set(sf::Vector2f(3650, 2650), sf::Vector2f(50, 300), sf::Color::White);
	walls[68].Set(sf::Vector2f(3650, 2200), sf::Vector2f(50, 300), sf::Color::White);
	walls[69].Set(sf::Vector2f(3150, 2200), sf::Vector2f(500, 50), sf::Color::White);
	walls[70].Set(sf::Vector2f(3150, 2250), sf::Vector2f(50, 550), sf::Color::White);
	walls[71].Set(sf::Vector2f(3150, 2800), sf::Vector2f(500, 50), sf::Color::White);

	//Instantiating the keys
	Key keys[4];
	//Setting up all the keys
	keys[0].Set(sf::Vector2f(900, 900), sf::Vector2f(50, 50), sf::Color(255, 192, 0, 255));
	keys[1].Set(sf::Vector2f(550, 250), sf::Vector2f(50, 50), sf::Color(146, 208, 80, 255));
	keys[2].Set(sf::Vector2f(300, 2350), sf::Vector2f(50, 50), sf::Color::Red);
	keys[3].Set(sf::Vector2f(3250, 2500), sf::Vector2f(50, 50), sf::Color::Cyan);

	//Instantiating the doors
	Door doors[4];
	//Setting up all the doors
	doors[0].Set(sf::Vector2f(500, 1050), sf::Vector2f(200, 50), sf::Color(255, 192, 0, 255));
	doors[1].Set(sf::Vector2f(950, 2450), sf::Vector2f(50, 200), sf::Color(146, 208, 80, 255));
	doors[2].Set(sf::Vector2f(450, 700), sf::Vector2f(50, 150), sf::Color::Red);
	doors[3].Set(sf::Vector2f(1000, 400), sf::Vector2f(50, 200), sf::Color::Cyan);

	//Instantiating enemies
	Enemy enemies[4];
	float enemyMagnitudes[4];
	sf::Vector2f enemyLastPos[4];
	//Setting up all enemies
	enemies[0].Set(sf::Vector2f(200, 350), sf::Vector2f(50, 50), sf::Color::Red);
	enemies[1].Set(sf::Vector2f(3350, 2500), sf::Vector2f(50, 50), sf::Color::Red);
	enemies[2].Set(sf::Vector2f(3450, 2500), sf::Vector2f(50, 50), sf::Color::Red);
	enemies[3].Set(sf::Vector2f(400, 2400), sf::Vector2f(50, 50), sf::Color::Red);
	//Setting up enemy movement speed
	enemyMagnitudes[0] = 500;
	enemyMagnitudes[1] = 500;
	enemyMagnitudes[2] = -500;
	enemyMagnitudes[3] = 500;

	//Instantiate and set up the player
	Player player(sf::Vector2f(60, 60), sf::Vector2f(30, 30), sf::Color::Red);
	
	//Instantiate and load the spritesheets
	sf::Texture walkingSpriteSheet;
	sf::Texture idleSpriteSheet;
	if (walkingSpriteSheet.loadFromFile("Walking.png")) {
		std::cout << "loaded";
	}
	if (idleSpriteSheet.loadFromFile("Idle.png")) {
		std::cout << "loaded\n";
	}
	//Load sprite sheets into the player
	player.LoadAnimation(walkingSpriteSheet, idleSpriteSheet);
	


	//Instantiate and set up the game camera
	//sf::View mainView(sf::FloatRect(0, 0, 640, 480));
	sf::View mainView(sf::FloatRect(0, 0, 1600, 1200));

	//Main game loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		//Switches between screens and levels
		switch (gameState) 
		{
		case 1:
			//Reset clock to get deltaTime
			timesinceFrame = deltaClock.restart();
			deltaTime = timesinceFrame.asSeconds();

			//Store the player's last position
			lastPos = player.body.getPosition();
			//Store the enemies' last positions
			enemyLastPos[0] = enemies[0].body.getPosition();
			enemyLastPos[1] = enemies[1].body.getPosition();
			enemyLastPos[2] = enemies[2].body.getPosition();
			enemyLastPos[3] = enemies[3].body.getPosition();
			//Update the player per frame
			player.Tick(timesinceFrame, deltaTime, 200);

			//Check for collision with all the walls in the game
			for (int i = 0; i < 72; i++)
			{
				if (player.Collision(walls[i]))
				{
					player.body.setPosition(lastPos);
				}
			}

			//Check for collision with all the keys in the game
			for (int i = 0; i < 4; i++)
			{
				if (player.KeyCollision(keys[i]))
				{
					keys[i].isCollected = true;
					doors[i].isUnlocked = true;
				}
			}

			//Check for collision with all the doors in the game
			for (int i = 0; i < 4; i++)
			{
				if (!doors[i].isUnlocked)
				{
					if (player.DoorCollision(doors[i]))
					{
						player.body.setPosition(lastPos);
					}
				}
			}

			//Move the enemy
			enemies[0].Move(sf::Vector2f(enemyMagnitudes[0] * deltaTime, 0));
			//Check the enemy for collisions
			for (int i = 0; i < 72; i++)
			{
				if (enemies[0].Collision(walls[i]))
				{
					//Prevent overlaps and inverse the enemy movement direction
					enemies[0].body.setPosition(enemyLastPos[0]);
					enemyMagnitudes[0] = enemyMagnitudes[0] * -1;
				}
			}

			//Move the enemy
			enemies[1].Move(sf::Vector2f(0, enemyMagnitudes[1] * deltaTime));
			//Check the enemy for collisions
			for (int i = 0; i < 72; i++)
			{
				if (enemies[1].Collision(walls[i]))
				{
					//Prevent overlaps and inverse the enemy movement direction
					enemies[1].body.setPosition(enemyLastPos[1]);
					enemyMagnitudes[1] = enemyMagnitudes[1] * -1;
				}
			}

			//Move the enemy
			enemies[2].Move(sf::Vector2f(0, enemyMagnitudes[2] * deltaTime));
			//Check the enemy for collisions
			for (int i = 0; i < 72; i++)
			{
				if (enemies[2].Collision(walls[i]))
				{
					//Prevent overlaps and inverse the enemy movement direction
					enemies[2].body.setPosition(enemyLastPos[2]);
					enemyMagnitudes[2] = enemyMagnitudes[2] * -1;
				}
			}

			//Move the enemy
			enemies[3].Move(sf::Vector2f(enemyMagnitudes[3] * deltaTime, 0));
			//Check the enemy for collisions
			for (int i = 0; i < 72; i++)
			{
				if (enemies[3].Collision(walls[i]))
				{
					//Prevent overlaps and inverse the enemy movement direction
					enemies[3].body.setPosition(enemyLastPos[3]);
					enemyMagnitudes[3] = enemyMagnitudes[3] * -1;
				}
			}

			//Clear the window
			window.clear();
			//Center the camera to the player
			mainView.setCenter(player.body.getPosition());
			//Set the windows view
			window.setView(mainView);
			//Update the player's animation
			player.playerAnim.update(timesinceFrame);
			//Draw the player on the screen
			window.draw(player.playerAnim);
			//Draw all the walls to the screen
			for (int i = 0; i < 72; i++)
			{
				window.draw(walls[i].body);
			}
			//Draw all the keys to the screen
			for (int i = 0; i < 4; i++)
			{
				if (!keys[i].isCollected)
				{
					window.draw(keys[i].body);
				}
			}
			//Draw all the doors to the screen
			for (int i = 0; i < 4; i++)
			{
				if (!doors[i].isUnlocked)
				{
					window.draw(doors[i].body);
				}
			}
			//Draw all enemies to the screen
			for (int i = 0; i < 4; i++)
			{
				window.draw(enemies[i].body);
			}

			//Display the game
			window.display();
			break;
		case 2: 
			window.clear();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				gameState = 1;
			}
			window.display();
		}
	}

	return 0;
}
