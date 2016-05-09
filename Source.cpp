#define SFML_STATIC

#include <SFML/Graphics.hpp>
#include <string>

class Object 
{
public:
	sf::RectangleShape body; 
	float bottom, right, left, top;

	Object(sf::Vector2f position, sf::Vector2f size, sf::Color color)
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
	void Tick(float deltaTime, Object hit) {
		Update(); 
		sf::Vector2f lastPos = body.getPosition();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			body.move(-700 * deltaTime, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			body.move(700 * deltaTime, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			body.move(0, -700 * deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			body.move(0, 700 * deltaTime);
		}
		if (Collision(hit)) {
			body.setPosition (lastPos);
		}
	}
	Player(sf::Vector2f position, sf::Vector2f size, sf::Color color) : Object(position, size, color)
	{

	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	sf::Clock deltaClock;
	Player player (sf::Vector2f(50, 50), sf::Vector2f(30, 30), sf::Color::Red);
	Object wall(sf::Vector2f(350, 350), sf::Vector2f(300, 60), sf::Color::White);
	sf::View mainView(sf::FloatRect(0, 0, 800, 600));
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
		
		
		player.Tick(deltaTime, wall);
		window.clear();
		mainView.setCenter(player.body.getPosition());
		window.setView(mainView);
		window.draw(player.body);
		window.draw(wall.body);




		window.display();
	}

	return 0;
}

