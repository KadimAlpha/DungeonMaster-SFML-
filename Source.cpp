//#define SFML_STATIC

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "GHAnimatedSprite.h"
#include "KadimClasses.h"



//game setup and initialization
sf::RenderWindow window(sf::VideoMode(1600, 1200), "My window");
sf::Clock deltaClock;
int gameState = 2;
sf::Vector2f lastPos;
sf::Time timesinceFrame;
float deltaTime;

//Instantiating enemies
Enemy enemies[4];
float enemyMagnitudes[4];
sf::Vector2f enemyLastPos[4];

//Instantiating traps
Trap fire[3];
Object trapTrigger;

//Instantiate the walls of the level into an array
Object walls[72];

//Instantiating the keys
Key keys[4];

//Instantiating the doors
Door doors[4];

//Instantiate and set up the player
Player player(sf::Vector2f(60, 60), sf::Vector2f(30, 30), sf::Color::Red);

//Instantiate the end goal
Object goal;

void Reset()
{
	//Setting up the end goal
	goal.Set(sf::Vector2f(2650, 400), sf::Vector2f(50, 200), sf::Color::White);

	//Setting up all the keys
	keys[0].Set(sf::Vector2f(900, 900), sf::Vector2f(50, 50), sf::Color(255, 192, 0, 255));
	keys[1].Set(sf::Vector2f(550, 250), sf::Vector2f(50, 50), sf::Color(146, 208, 80, 255));
	keys[2].Set(sf::Vector2f(300, 2350), sf::Vector2f(50, 50), sf::Color::Red);
	keys[3].Set(sf::Vector2f(3250, 2500), sf::Vector2f(50, 50), sf::Color::Cyan);
	for (int i = 0; i < 4; i++)
	{
		keys[i].UpdateSprite();
	}

	//Setting up all the doors
	doors[0].Set(sf::Vector2f(500, 1050), sf::Vector2f(200, 50), sf::Color(255, 192, 0, 255));
	doors[1].Set(sf::Vector2f(950, 2450), sf::Vector2f(50, 200), sf::Color(146, 208, 80, 255));
	doors[2].Set(sf::Vector2f(450, 700), sf::Vector2f(50, 150), sf::Color::Red);
	doors[3].Set(sf::Vector2f(1000, 400), sf::Vector2f(50, 200), sf::Color::Cyan);


	//Setting up all enemies
	enemies[0].Set(sf::Vector2f(101, 350), sf::Vector2f(50, 50), sf::Color::Red);
	enemies[1].Set(sf::Vector2f(3350, 2251), sf::Vector2f(50, 50), sf::Color::Red);
	enemies[2].Set(sf::Vector2f(3450, 2749), sf::Vector2f(50, 50), sf::Color::Red);
	enemies[3].Set(sf::Vector2f(251, 2400), sf::Vector2f(50, 50), sf::Color::Red);

	//Setting up trap
	trapTrigger.Set(sf::Vector2f(1650, 2350), sf::Vector2f(50, 50), sf::Color(20,20,20,255));

	for (int i = 0; i < 4; i++)
	{
		enemies[i].UpdateSprite();
	}

	//Reset all keys and doors
	for (int i = 0; i < 4; i++)
	{
		keys[i].isCollected = false;
		doors[i].isUnlocked = false;
	}
	//Reset player
	player.isDead = false;
	player.body.setPosition(sf::Vector2f(60, 60));
}

int main()
{
	
	//Instantiate and load all textures and spritesheets
	sf::Texture walkingSpriteSheet;
	sf::Texture idleSpriteSheet;
	sf::Texture redKey;
	sf::Texture blueKey;
	sf::Texture yellowKey;
	sf::Texture greenKey;
	sf::Texture enemySprite;
	sf::Texture fireSpriteSheet;
	if (walkingSpriteSheet.loadFromFile("PlayerWalking.png")) {
		std::cout << "loaded\n";
	}
	if (idleSpriteSheet.loadFromFile("PlayerIdle.png")) {
		std::cout << "loaded\n";
	}
	if (redKey.loadFromFile("RedKey.png")) {
		std::cout << "loaded\n";
	}
	if (greenKey.loadFromFile("GreenKey.png")) {
		std::cout << "loaded\n";
	}
	if (blueKey.loadFromFile("BlueKey.png")) {
		std::cout << "loaded\n";
	}
	if (yellowKey.loadFromFile("YellowKey.png")) {
		std::cout << "loaded\n";
	}
	if (enemySprite.loadFromFile("Enemy.png")) {
		std::cout << "loaded\n";
	}
	if (fireSpriteSheet.loadFromFile("Fire.png")) {
		std::cout << "loaded fire\n";
	}

	//Load sprite sheets into the player
	player.playerAnim.sprite.setTexture(walkingSpriteSheet);
	player.playerAnim.sprite.setScale(3.75, 3.75);

	//Load sprites into the keys
	keys[0].sprite.setTexture(yellowKey);
	keys[1].sprite.setTexture(greenKey);
	keys[2].sprite.setTexture(redKey);
	keys[3].sprite.setTexture(blueKey);

	//Load sprites into enemies
	for (int i = 0; i < 4; i++)
	{
		enemies[i].sprite.setTexture(enemySprite);
	}

	//Setting up all traps
	fire[0].Set(sf::Vector2f(1600, 2300), sf::Vector2f(50, 50), sf::Color::Red);
	fire[1].Set(sf::Vector2f(1650, 2300), sf::Vector2f(50, 50), sf::Color::Red);
	fire[2].Set(sf::Vector2f(1700, 2300), sf::Vector2f(50, 50), sf::Color::Red);

	//Load sprites into the traps
	for (int i = 0; i < 4; i++)
	{
		fire[i].fireAnim.sprite.setTexture(fireSpriteSheet);
		fire[i].fireAnim.sprite.setPosition(fire[i].body.getPosition());
		fire[i].fireAnim.sprite.setScale(6.25, 6.25);
	}
	


	//Instantiate and set up the game camera
	sf::View mainView(sf::FloatRect(250, 0, 640, 480));
	sf::Vector2f cameraStartPos = mainView.getCenter();
	//sf::View mainView(sf::FloatRect(0, 0, 3200, 2400));

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

	//Setting up enemy movement speed
	enemyMagnitudes[0] = 500;
	enemyMagnitudes[1] = 500;
	enemyMagnitudes[2] = -500;
	enemyMagnitudes[3] = 500;

	//Reset the game 
	Reset();

	//Set up main menu text
	sf::Text title;
	sf::Text instructions;
	sf::Text instructions2;
	title.setString("Dungeon Master!");
	instructions.setString("WSAD Keys to move");
	instructions2.setString("Press ENTER to continue");
	sf::Font font;
	font.loadFromFile("Freshman.ttf");
	title.setFont(font);
	title.setColor(sf::Color::White);
	title.setPosition(320, 200);
	instructions.setFont(font);
	instructions.setColor(sf::Color::White);
	instructions.setPosition(320, 240);
	instructions2.setFont(font);
	instructions2.setColor(sf::Color::White);
	instructions2.setPosition(320, 280);

	//Set up Game Over text
	sf::Text gameOver;
	gameOver.setString("GAME OVER");
	gameOver.setPosition(320, 240);
	gameOver.setColor(sf::Color::White);
	gameOver.setFont(font);

	//Set up Win text
	sf::Text win;
	win.setString("YOU WIN!");
	win.setPosition(320, 240);
	win.setColor(sf::Color::White);
	win.setFont(font);

	
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

			//Check if player has reached the goal
			if (player.Collision(goal)) 
			{
				gameState = 4;
			}
			//Check for collision with all the keys in the game
			for (int i = 0; i < 4; i++)
			{
				if (player.KeyCollision(keys[i]))
				{
					keys[i].isCollected = true;
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
						if (keys[i].isCollected)
						{
							doors[i].isUnlocked = true;
						}
					}
				}
			}
			//Check for collision with all enemies in the game
			for (int i = 0; i < 4; i++)
			{
				if (player.EnemyCollision(enemies[i])) 
				{
					player.Kill();
				}
			}

			//Check for collision with trap trigger
			if (player.Collision(trapTrigger))
			{
				for (int i = 0; i < 3; i++)
				{
					fire[i].isActive = true;
				}
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					fire[i].isActive = false;
				}
			}

			//Check for collision with all fire in the game
			for (int i = 0; i < 3; i++)
			{
				if (player.TrapCollision(fire[i]))
				{
					player.Kill();
				}
			}

			//Check if enemy is out of bounds and reset it
			if (enemies[0].body.getPosition().x > 401 || enemies[0].body.getPosition().x < 49)
			{
				enemies[0].body.setPosition(101, 350);
			}

			//Check if enemy is out of bounds and reset it
			if (enemies[1].body.getPosition().y > 2801 || enemies[1].body.getPosition().y < 2249)
			{
				enemies[1].body.setPosition(3350, 2251);
			}

			//Check if enemy is out of bounds and reset it
			if (enemies[2].body.getPosition().y > 2801 || enemies[2].body.getPosition().y < 2249)
			{
				enemies[2].body.setPosition(3450, 2251);
			}

			//Check if enemy is out of bounds and reset it
			if (enemies[3].body.getPosition().x > 701 || enemies[3].body.getPosition().x < 249)
			{
				enemies[3].body.setPosition(251, 2400);
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

			//Update Enemy Sprites
			for (int i = 0; i < 4; i++)
			{
				enemies[i].UpdateSprite();
			}

			//Checks if player has died
			if (player.isDead) 
			{
				gameState = 3;
			}

			//Clear the window
			window.clear();

			//Center the camera to the player
			mainView.setCenter(player.body.getPosition());

			//Set the windows view
			window.setView(mainView);

			//Update the player's animation
			player.playerAnim.Update(deltaTime);
			if (player.isWalking) 
			{
				player.playerAnim.sprite.setTexture(walkingSpriteSheet);
			}
			else 
			{
				player.playerAnim.sprite.setTexture(idleSpriteSheet);
			}

			//Update the trap animation 
			for (int i = 0; i < 3; i++)
			{
				fire[i].fireAnim.Update(deltaTime);
			}

			//Draw the player on the screen
			window.draw(player.playerAnim.sprite);

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
					window.draw(keys[i].sprite);
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
				window.draw(enemies[i].sprite);
			}

			//Draws all traps to the screen
			for (int i = 0; i < 4; i++)
			{
				if (fire[i].isActive) 
				{
					window.draw(fire[i].fireAnim.sprite);
				}
			}
			
			//Draw the end goal
			window.draw(goal.body);

			//Display the game
			window.display();
			break;
		case 2: 
			//The Main Menu screen
			window.clear();
			mainView.setCenter(cameraStartPos);
			window.setView(mainView);
			window.draw(title);
			window.draw(instructions);
			window.draw(instructions2);
			
			//Reset and restart game
			//Switch screen
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				Reset();
				gameState = 1;
			}
			window.display();
			break;
		case 3:
			//Game Over screen
			window.clear();
			mainView.setCenter(cameraStartPos);
			window.setView(mainView);
			window.draw(gameOver);
			window.draw(instructions2);

			//Reset and restart game
			//Switch screen
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				Reset();
				gameState = 2;
			}
			window.display();
			break;
		case 4:
			//Win screen
			window.clear();
			mainView.setCenter(cameraStartPos);
			window.setView(mainView);
			window.draw(win);
			window.draw(instructions2);

			//Reset and restart game
			//Switch screen
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
				Reset();
				gameState = 2;
			}
			window.display();
		}
	}

	return 0;
}


