#include "stdafx.h"
#include "config.h"


void dataInitialize(Config & conf) {
	conf.countBlocks = 5;
	conf.settings.antialiasingLevel = 8;
	conf.window.create(sf::VideoMode(800, 600), "7.2", sf::Style::Default, conf.settings);
	conf.isAnimationStart = false;
	conf.stepAnimation = 0;
	conf.sizeValue = 0;
}

std::vector<sf::RectangleShape> createBlocks(const int & countBlocks) {
	sf::RectangleShape block(sf::Vector2f(30.0, 10.0));
	block.setFillColor(sf::Color(100, 100, 100));
	block.setOrigin(block.getPosition().x + block.getSize().x / 2, block.getPosition().y + block.getSize().y / 2);
	std::vector<sf::RectangleShape> blocks;
	for (int i = 0; i < countBlocks; i++) {
		block.setPosition(100 + i * 50, 250);
		blocks.push_back(block);
	}
	return blocks;
}


int randomNumber(const int & size) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 32700);
	int number = dist(gen) % size;
	return number;
}


void runZoomBlocks(Config & conf, std::vector<sf::RectangleShape> & blocks, const float & size) {
	conf.stepAnimation++;
	for (auto blockItr = blocks.begin(); blockItr != blocks.end(); blockItr++)
		blockItr->scale(size, size);
	if (conf.stepAnimation == 10) {
		conf.isAnimationStart = false;
		conf.stepAnimation = 0;
		if (size > 1.0)
			conf.sizeValue = conf.sizeValue + 1;
		else if (size < 1.0)
			conf.sizeValue--;
	}
}

void runSetNewColor(Config & conf, std::vector<sf::RectangleShape> & blocks) {
	conf.stepAnimation++;
	if (conf.stepAnimation % 2 == 0) {
		for (auto blockItr = blocks.begin(); blockItr != blocks.end(); blockItr++) {
			int color1 = randomNumber(255);
			int color2 = randomNumber(255);
			int color3 = randomNumber(255);
			blockItr->setFillColor(sf::Color(color1, color2, color3));
		}
		if (conf.stepAnimation == 10) {
			conf.isAnimationStart = false;
			conf.stepAnimation = 0;
		}
	}
}


void runRotateBlocks(Config & conf, std::vector<sf::RectangleShape> & blocks) {
	conf.stepAnimation++;
	for (auto blockItr = blocks.begin(); blockItr != blocks.end(); blockItr++)
		blockItr->rotate(30);
	if (conf.stepAnimation == 10) {
		conf.isAnimationStart = false;
		conf.stepAnimation = 0;
	}
	
}

void runMoveBlocks(Config & conf, std::vector<sf::RectangleShape> & blocks, const int & deltaX, const int & deltaY) {
	conf.stepAnimation++;
	for (auto blockItr = blocks.begin(); blockItr != blocks.end(); blockItr++)
		blockItr->move(deltaX, deltaY);		
	if (conf.stepAnimation == 10) {
		conf.isAnimationStart = false;
		conf.stepAnimation = 0;
	}
	
}

void drawingObject(Config & conf, std::vector<sf::RectangleShape> & blocks) {
	conf.window.clear(sf::Color(255, 255, 255));
	for (auto blockItr = blocks.begin(); blockItr != blocks.end(); blockItr++)
		conf.window.draw(*blockItr);
	conf.window.display();
}

void runAnimations(Config & conf, std::vector<sf::RectangleShape> & blocks) {
	//---------------цвет-----------------
	if (conf.numberOfAnimation == 0)
		runSetNewColor(conf, blocks);
	//------------------------------------------
	//------------------------размер увеличение-------------
	else if (conf.numberOfAnimation == 1 && conf.sizeValue != 1)
		runZoomBlocks(conf, blocks, 1.05);
	//--------------------------------------------
	//------------------------размер уменьшение-------------
	else if (conf.numberOfAnimation == 2 && conf.sizeValue != -1)
		runZoomBlocks(conf, blocks, 0.95);
	//--------------------------------------------
	//------------------поворот-----------
	else if (conf.numberOfAnimation == 3)
		runRotateBlocks(conf, blocks);
	//------------------------------------
	else if (conf.numberOfAnimation == 4 && blocks.begin()->getPosition().x < conf.window.getSize().x / 2)
		runMoveBlocks(conf, blocks, 20, 0);
	else if (conf.numberOfAnimation == 5 && blocks.begin()->getPosition().x > 50)
		runMoveBlocks(conf, blocks, -20, 0);
	else if (conf.numberOfAnimation == 6 && blocks.begin()->getPosition().y < conf.window.getSize().y - 100)
		runMoveBlocks(conf, blocks, 0, 20);
	else if (conf.numberOfAnimation == 7 && blocks.begin()->getPosition().y > 100)
		runMoveBlocks(conf, blocks, 0, -20);
	else {
		conf.isAnimationStart = false;
		conf.stepAnimation = 0;
	}
}

void animations(Config & conf) {	
	sf::Clock clock;
	sf::Event event;
	std::vector<sf::RectangleShape> blocks = createBlocks(conf.countBlocks);

	while (conf.window.isOpen()) {
		clock.restart();
		while (conf.window.pollEvent(event)) {
			if (event.type == sf::Event::Closed ||
				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				conf.window.close();
		}

		if (conf.isAnimationStart)
			runAnimations(conf, blocks);
		else {
			conf.isAnimationStart = true;
			conf.numberOfAnimation = (float)(randomNumber(8));
		}

		drawingObject(conf, blocks);
		sf::sleep(sf::microseconds((sf::Int64)(1000000 / 10.0) - clock.getElapsedTime().asMicroseconds()));
	}
}

int main()
{
	Config conf;
	dataInitialize(conf);
	animations(conf);
    return 0;
}

