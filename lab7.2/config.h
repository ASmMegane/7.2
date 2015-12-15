#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h> 
#include <list>
#include <random>

struct Config {
	Config() {};
	sf::RenderWindow window;
	int numberOfAnimation; //
	bool isAnimationStart;
	int maxStepAnimation;
	int stepAnimation;
	int sizeValue;
	float zoomUpValue;
	float zoomDownValue;
	float rotateBlock;
	int deltaXPos;
	int deltaYPos;
};
