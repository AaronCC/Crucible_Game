#pragma once
#include <SFML/Graphics.hpp>

class Helper
{
public:
	float dotProduct(sf::Vector2f v1, sf::Vector2f v2);
	std::vector<std::pair<int, int>> getNeighbors(int x, int y, int width, int height);
	sf::FloatRect resizeView(float windowW, float windowH, float aspectRatio);
	float magnitude(sf::Vector2f);
	sf::Vector2f normalized(sf::Vector2f vec, float mag);
	sf::Vector2f clamp(sf::Vector2f vec, float max);
	Helper();
	~Helper();
};
