#include"game.h"
#include"fluid_utils.h"

Game::Game() {
	window.create(sf::VideoMode(winX, winY), "Fluid", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);
	image.create(winX, winY, sf::Color::Red);
	texture.loadFromImage(image);
	fluid.setTexture(texture);
}

void Game::initialize() {

};

void Game::update() {
	for (int x = winX / 2 - 10; x < winX / 2 + 10; x++) {
		cVelocityX[index(x, 0)] += -9.8;
	}
	step(cVelocityX, cVelocityY, lVelocityX, lVelocityY, cDensity, lDensity, diffusion, viscosity, dt.asSeconds());
	int x, y;
	for (y = 0; y < winY; y++) {
		for (x = 0; x < winX; x++) {
			float d = cDensity[index(x, y)];
			image.setPixel(x, y, sf::Color::Color(d, d, d));
		}
	}
}

void Game::run() {

	//initialize();

	while (window.isOpen()) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			mousePos = sf::Mouse::getPosition();
			for (int y = mousePos.y - 4; y <= mousePos.y + 4 && y > 0 && y < winY; y++) {
				for (int x = mousePos.x - 4; x <= mousePos.x + 4 && x > 0 && x < winX; x++) {
					cDensity[index(x, y)] += 50;
				}
			}
		}
		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			mousePos = sf::Mouse::getPosition();
			sf::Vector2i mouseChange = mousePos - lastMousePos;
			for (int y = mousePos.y - 4; y <= mousePos.y + 4 && y > 0 && y < windowSize.y; y++) {
				for (int x = mousePos.x - 4; x <= mousePos.x + 4 && x > 0 && x < windowSize.x; x++) {
					pixels[Pixel::index(x, y)]->addVelocity(sf::Vector2f(mouseChange.x, mouseChange.y));
				}
			}
		}*/
		lastMousePos = sf::Mouse::getPosition();
		while (window.pollEvent(gameEvent))
		{
			// Close window: exit-----------------
			if (gameEvent.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				window.close();
			}
		}
		//UPDATE-------------------------------------------------------------
		dt = clock.getElapsedTime();
		clock.restart();
		update();

		//RENDER--------------------------------------------------------------
		window.clear(bgColor);

		texture.loadFromImage(image);
		window.draw(fluid);

		window.display();
	}
}