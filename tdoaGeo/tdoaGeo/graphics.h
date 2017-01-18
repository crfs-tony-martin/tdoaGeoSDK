#pragma once

#include "tdoa.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class graphics
{
public:
	graphics(uint32_t x, uint32_t y);
	~graphics();
	bool plotResult(tdoaResult *result);
	bool drawSignal(TSignal &s);
	bool drawSignal(std::vector<double> &v);
	private:
		void graphics::processEvents();
		sf::RenderWindow *_window = { NULL };
		sf::Vector2f m_radius;
		double _scale = { 0.2 };
};

