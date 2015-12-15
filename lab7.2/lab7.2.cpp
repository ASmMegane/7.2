#include "stdafx.h"
#include "config.h"


void dataInitialize(Config & conf) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	conf.window.create(sf::VideoMode(800, 600), "7.2", sf::Style::Default, settings);
	conf.isAnimationStart = false;
	conf.maxStepAnimation = 10;
	conf.stepAnimation = 0;
	conf.sizeValue = 0;
	conf.zoomUpValue = 1.05f;
	conf.zoomDownValue = 0.95f;
	conf.rotateBlock = 30.0f;
}

std::vector<sf::RectangleShape> createBlocks(int & countBlocks) {
	float sizeXBlock = 30.0f;
	float sizeYblock = 10.0f;
	float startXPos = 100.0f;
	float startYPos = 250.0f;
	float deltaXForBlock = 50.0f;

	sf::RectangleShape block(sf::Vector2f(sizeXBlock, sizeYblock));
	block.setFillColor(sf::Color(100, 100, 100));
	block.setOrigin(block.getPosition().x + block.getSize().x / 2, block.getPosition().y + block.getSize().y / 2);
	std::vector<sf::RectangleShape> blocks;
	for (int i = 0; i < countBlocks; i++) {
		block.setPosition(startXPos + i * deltaXForBlock, startYPos);
		blocks.push_back(block);
	}
	return blocks;
}


int randomNumber(int size) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 32700);
	int number = dist(gen) % size;
	return number;
}


void runZoomBlocks(Config & conf, std::vector<sf::RectangleShape> & blocks, float & size) {
	conf.stepAnimation++;
	for (auto&& blockItr : blocks)
		blockItr.scale(size, size);
	if (conf.stepAnimation == conf.maxStepAnimation) {
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
	for (auto&& blockItr : blocks) {
		int areaNumber = 255;
		int color1 = randomNumber(areaNumber);
		int color2 = randomNumber(areaNumber);
		int color3 = randomNumber(areaNumber);
		blockItr.setFillColor(sf::Color(color1, color2, color3));
	}
	if (conf.stepAnimation == conf.maxStepAnimation) {
		conf.isAnimationStart = false;
		conf.stepAnimation = 0;
	}
}


void runRotateBlocks(Config & conf, std::vector<sf::RectangleShape> & blocks) {
	conf.stepAnimation++;
	for (auto&& blockItr : blocks)
		blockItr.rotate(conf.rotateBlock);
	if (conf.stepAnimation == conf.maxStepAnimation) {
		conf.isAnimationStart = false;
		conf.stepAnimation = 0;
	}	
}

void runMoveBlocks(Config & conf, std::vector<sf::RectangleShape> & blocks, int  deltaX, int  deltaY) {
	conf.stepAnimation++;
	for (auto&& blockItr : blocks)
		blockItr.move((float)deltaX, (float)deltaY);		
	if (conf.stepAnimation == conf.maxStepAnimation) {
		conf.isAnimationStart = false;
		conf.stepAnimation = 0;
	}	
}

void drawingObject(Config & conf, std::vector<sf::RectangleShape> & blocks) {
	conf.window.clear(sf::Color(255, 255, 255));
	for (auto blockItr : blocks)
		conf.window.draw(blockItr);
	conf.window.display();
}

void runAnimations(Config & conf, std::vector<sf::RectangleShape> & blocks) {
	//---------------цвет-----------------
	if (conf.numberOfAnimation == 0)
		runSetNewColor(conf, blocks);
	//------------------------------------------
	//------------------------размер увеличение-------------
	else if (conf.numberOfAnimation == 1 && conf.sizeValue != 1)
		runZoomBlocks(conf, blocks, conf.zoomUpValue);
	//--------------------------------------------
	//------------------------размер уменьшение-------------
	else if (conf.numberOfAnimation == 2 && conf.sizeValue != -1)
		runZoomBlocks(conf, blocks, conf.zoomDownValue);
	//--------------------------------------------
	//------------------поворот-----------
	else if (conf.numberOfAnimation == 3)
		runRotateBlocks(conf, blocks);
	//------------------------------------
	else if (conf.numberOfAnimation == 4 && blocks.begin()->getPosition().x < conf.window.getSize().x / 2) {
		conf.deltaXPos = 20;
		conf.deltaYPos = 0;
		runMoveBlocks(conf, blocks, conf.deltaXPos, conf.deltaYPos);
	} else if (conf.numberOfAnimation == 5 && blocks.begin()->getPosition().x > 50) {
		conf.deltaXPos = -20;
		conf.deltaYPos = 0;
		runMoveBlocks(conf, blocks, conf.deltaXPos, conf.deltaYPos);
	} else if (conf.numberOfAnimation == 6 && blocks.begin()->getPosition().y < conf.window.getSize().y - 100) {
		conf.deltaXPos = 0;
		conf.deltaYPos = 20;
		runMoveBlocks(conf, blocks, conf.deltaXPos, conf.deltaYPos);
	} else if (conf.numberOfAnimation == 7 && blocks.begin()->getPosition().y > 100){
		conf.deltaXPos = 0;
		conf.deltaYPos = -20;
		runMoveBlocks(conf, blocks, conf.deltaXPos, conf.deltaYPos);
	} else {
		conf.isAnimationStart = false;
		conf.stepAnimation = 0;
	}
}

void animations(Config & conf) {	
	sf::Clock clock;
	sf::Event event;
	int countBlocks = 5;
	std::vector<sf::RectangleShape> blocks = createBlocks(countBlocks);

	while (conf.window.isOpen()) {
		clock.restart();
		while (conf.window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				conf.window.close();
		}
		if (conf.isAnimationStart)
			runAnimations(conf, blocks);
		else {
			conf.isAnimationStart = true;
			conf.numberOfAnimation = (randomNumber(8));
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

