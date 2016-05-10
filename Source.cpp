
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

class Object 
{
public:
	sf::RectangleShape body; 
	float bottom, right, left, top;
	sf::String tag;

	Object(/*sf::Vector2f position, sf::Vector2f size, sf::Color color*/)
	{
		//body.setPosition(position);
		//body.setSize(size);
		//body.setFillColor(color);
	}

	void Set(sf::Vector2f position, sf::Vector2f size, sf::Color color) 
	{
		body.setPosition(position);
		body.setSize(size);
		body.setFillColor(color);
	}

	void Update()
	{
		bottom = body.getPosition().y + body.getSize().y;
		left = body.getPosition().x; 
		right = body.getPosition().x + body.getSize().x;
		top = body.getPosition().y;
	}

	bool Collision(Object hit)
	{
		if (hit.body.getGlobalBounds().intersects(body.getGlobalBounds()))
		{
			return true; 
		}
		return false;
	}
};

class Player : public Object
{
public:
	void Tick(float deltaTime, float speed) {
		Update(); 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			body.move(-speed * deltaTime, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			body.move(speed * deltaTime, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			body.move(0, -speed * deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			body.move(0, speed * deltaTime);
		}
	}
	Player(sf::Vector2f position, sf::Vector2f size, sf::Color color) /*: Object(position, size, color)*/
	{
		body.setPosition(position);
		body.setSize(size);
		body.setFillColor(color);
	}
};


int main()
{

	//game setup
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	sf::Clock deltaClock;
	Object walls[72];
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


	Player player (sf::Vector2f(50, 50), sf::Vector2f(30, 30), sf::Color::Red);
	sf::View mainView(sf::FloatRect(0, 0, 800, 600));

	//game start
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	    sf::Time timesinceFrame = deltaClock.restart();
		float deltaTime = timesinceFrame.asSeconds();
		

		sf::Vector2f lastPos = player.body.getPosition();
		player.Tick(deltaTime, 200);
		for (int i = 0; i < 72; i++) 
		{
			if (player.Collision(walls[i]))
			{
				
				player.body.setPosition(lastPos);
			}
		}
		
		window.clear();
		mainView.setCenter(player.body.getPosition());
		window.setView(mainView);
		window.draw(player.body);
		for (int i = 0; i < 72; i++)
		{
			window.draw(walls[i].body);
		}
		




		window.display();
	}

	return 0;
}

