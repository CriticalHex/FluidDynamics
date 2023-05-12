#pragma once
#include"globals.h"

class Game
{
public:
	Game();
	void run();
private:
	void initialize();
	void update();

	float cVelocityX[winX * winY];
	float cVelocityY[winX * winY];
	float lVelocityX[winX * winY];
	float lVelocityY[winX * winY];
	float cDensity[winX * winY];
	float lDensity[winX * winY];
	float diffusion = 0;
	float viscosity = 0.000001;

	sf::RenderWindow window;
	sf::Event gameEvent;
	sf::Color bgColor = sf::Color(6, 8, 12);
	sf::Clock clock;
	sf::Time dt;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite fluid;
	sf::Vector2i mousePos;
	sf::Vector2i lastMousePos;
};