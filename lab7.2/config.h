#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h> 
#include <list>
#include <random>

struct Config {
	Config() {};
	int countBlocks;
	sf::RenderWindow window;
	sf::ContextSettings settings;
	int numberOfAnimation; //
	bool isAnimationStart;
	int stepAnimation;
	int sizeValue;
};
