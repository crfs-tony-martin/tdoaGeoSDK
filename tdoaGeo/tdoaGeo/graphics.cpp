#include "graphics.h"

graphics::graphics(uint32_t x, uint32_t y)
{
	// create the window
	_window = new sf::RenderWindow(sf::VideoMode(x, y), "TDOA");
}
graphics::~graphics(){}

class colour
{
public:
	unsigned char _red;
	unsigned char _green;
	unsigned char _blue;
	int _hue;
	float _sat;
	float _lum;

	colour(unsigned char r, unsigned char g, unsigned char b) { _red = r; _green = g; _blue = b; rgb2hsl(); };
	colour(int h, float s, float l) { _hue = h; _sat = s; _lum = l; hsl2rgb(); };
	colour(double value) { _hue = (1.0 - value) * 240; _sat = 1, _lum = 0.5; hsl2rgb(); };
private:
	float hue2rgb(float v1, float v2, float vH) {
		if (vH < 0)
			vH += 1;
		if (vH > 1)
			vH -= 1;
		if ((6 * vH) < 1)
			return (v1 + (v2 - v1) * 6 * vH);
		if ((2 * vH) < 1)
			return v2;
		if ((3 * vH) < 2)
			return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);
		return v1;
	}

	void hsl2rgb()
	{
		if (_sat == 0)
		{
			_red = _green = _blue = (unsigned char)(_lum * 255);
		}
		else
		{
			float v1, v2;
			float hue = (float)_hue / 360;

			v2 = (_lum < 0.5) ? (_lum * (1 + _sat)) : ((_lum + _sat) - (_lum * _sat));
			v1 = 2 * _lum - v2;

			_red = (unsigned char)(255 * hue2rgb(v1, v2, hue + (1.0f / 3)));
			_green = (unsigned char)(255 * hue2rgb(v1, v2, hue));
			_blue = (unsigned char)(255 * hue2rgb(v1, v2, hue - (1.0f / 3)));
		}
	}

	void rgb2hsl()
	{
		float r = (float)_red / 255.0;
		float g = (float)_green / 255.0;
		float b = (float)_blue / 255.0;
		float cmax = std::max(std::max(r, g), b);
		float cmin = std::min(std::min(r, g), b);
		float delta = cmax - cmin;
		_lum = (cmax + cmin) / 2;
		if (delta == 0)
		{
			_hue = 0; _sat = 0;
		}
		else
		{
			if (cmax == r)
				_hue = 60 * fmod((g - b) / delta, 6.0);
			else if (cmax == g)
				_hue = 60 * (((b - r) / delta) + 2.0);
			else if (cmax == b)
				_hue = 60 * (((r - g) / delta) + 4.0);
			_sat = delta / (1 - std::abs(2 * (_lum - 1)));
		}
	}

};

bool graphics::plotResult(tdoaResult *result)
{
	bool open = _window != NULL && _window->isOpen();
	if (open)
	{
		processEvents();
		if (result != NULL)
		{
			_window->clear(sf::Color::Blue);
			sf::Vector2u pixels = _window->getSize();
			double maxLat = -90;
			double minLat = 90;
			double maxLon = -180;
			double minLon = +180;
			for (auto n : result->nodes)
			{
				maxLat = std::max(maxLat, n._lat);
				maxLon = std::max(maxLon, n._lon);
				minLat = std::min(minLat, n._lat);
				minLon = std::min(minLon, n._lon);
			}

			double yscale = maxLat - minLat;
			double xscale = maxLon - minLon;
			double yoffset = (maxLat + minLat) / 2;
			double xoffset = (maxLon + minLon) / 2;
			location l1(maxLat, 0, 0);
			location l2(minLat, 0, 0);
			double distance = l1.distance(l2);
			double m_pix = l1.distance(l2) / ((_scale * pixels.y * (maxLat - yoffset) / yscale) - (_scale * pixels.y * (minLat - yoffset) / yscale));

			double maxHeatmap = 0;
			for (auto l : result->heatmap)
				maxHeatmap = std::max(maxHeatmap, l._error);

			for (auto l : result->heatmap)
			{
				float y = _scale * pixels.y * (l._lat - yoffset) / yscale;
				float x = _scale * pixels.x * (l._lon - xoffset) / xscale;
				sf::CircleShape shape(3);
				colour rgb(1.0 - (l._error / maxHeatmap));
				shape.setFillColor(sf::Color(rgb._red, rgb._green, rgb._blue));
				shape.setPosition((pixels.x / 2) + x, (pixels.y / 2) - y);
				_window->draw(shape);
			}
			//Draw a square for each of the nodes
			for (auto n : result->nodes)
			{
				float y = _scale * pixels.y * (n._lat - yoffset) / yscale;
				float x = _scale * pixels.x * (n._lon - xoffset) / xscale;
				//sf::CircleShape symbol(3);
				sf::RectangleShape symbol(sf::Vector2f(10, 10));
				if (n == result->nodes.front())
					symbol.setFillColor(sf::Color::White);
				else
					symbol.setFillColor(sf::Color::Black);
				symbol.setPosition((pixels.x / 2) + x, (pixels.y / 2) - y);
				_window->draw(symbol);
			}
			//Draw the ellipse
			float y = _scale * pixels.y * (result->target.ellipse.centre._lat - yoffset) / yscale;
			float x = _scale * pixels.x * (result->target.ellipse.centre._lon - xoffset) / xscale;
			double size = result->target.ellipse.major;
			sf::CircleShape ellipse(result->target.ellipse.major/m_pix);
			ellipse.setPosition((pixels.x / 2) + x, (pixels.y / 2) - y);
			ellipse.setOrigin(ellipse.getRadius(), ellipse.getRadius());
			ellipse.setFillColor(sf::Color(0,128,0,128));
			ellipse.setOutlineColor(sf::Color::Black);
			ellipse.setOutlineThickness(1);
			ellipse.setScale(1.0, result->target.ellipse.minor / result->target.ellipse.major);
			ellipse.setRotation(-result->target.ellipse.angle);
			_window->draw(ellipse);
			//Draw the target
			y = _scale * pixels.y * (result->target.centre._lat - yoffset) / yscale;
			x = _scale * pixels.x * (result->target.centre._lon - xoffset) / xscale;
			sf::CircleShape shape(3);
			colour rgb(1.0 - result->target.rmsError);
			//shape.setFillColor(sf::Color(rgb._red, rgb._green, rgb._blue));
			shape.setFillColor(sf::Color::Black);
			shape.setPosition((pixels.x / 2) + x, (pixels.y / 2) - y);
			shape.setOrigin(shape.getRadius(), shape.getRadius());
			_window->draw(shape);

			//End the current frame
			std::string title = std::to_string(result->target.centre._lat) + ", " + std::to_string(result->target.centre._lon)
				+ "   " + std::to_string(result->target.timeStamp) + "   " + std::to_string(result->target.rmsError);
			_window->setTitle(title.c_str());
			_window->display();
		}
	}
	return open;
}



bool graphics::drawSignal(TSignal &s)
{
	bool open = _window != NULL && _window->isOpen();
	if (open)
	{
		processEvents();
		if (s.size() > 1)
		{
			_window->clear(sf::Color::Blue);
			sf::Vector2u pixels = _window->getSize();

			double max = std::abs(s[0]);
			for (size_t i = 0; i < s.size(); i++)
				max = std::max(max, std::abs(s[i]));

			TSignal y = s;
			y *= -0.4 * (static_cast<double>(pixels.y) / max);
			y += std::complex<double>(pixels.y / 2, pixels.y / 2);

			double xdelta = static_cast<double>(pixels.x) / static_cast<double>(y.size());
			for (int i = 0; i < y.size(); i++)
			{
				//Draw as connected lines
				if (i > 0)
				{
					sf::Vertex realLine[] =
					{
						sf::Vertex(sf::Vector2f((i - 1) * xdelta, y[i - 1].real()), sf::Color::Yellow),
						sf::Vertex(sf::Vector2f(i * xdelta, y[i].real()), sf::Color::Yellow)
					};
					sf::Vertex imagLine[] =
					{
						sf::Vertex(sf::Vector2f((i - 1) * xdelta, y[i - 1].imag()), sf::Color::Cyan),
						sf::Vertex(sf::Vector2f(i * xdelta, y[i].imag()), sf::Color::Cyan)
					};
					_window->draw(realLine, 2, sf::Lines);
					_window->draw(imagLine, 2, sf::Lines);
				}
			}
			// end the current frame
			_window->display();
		}
	}
	return open;
}

bool graphics::drawSignal(std::vector<double> &v)
{
	TSignal s(v.size());
	for (size_t i = 0; i < v.size(); i++)
	{
		s[i] = std::complex<double>(v[i], 0.0);
	}
	return drawSignal(s);
}


void graphics::processEvents()
{
	if (_window != NULL && _window->isOpen())
	{
		sf::Event event;
		while (_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				_window->close();
				return;
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				if (event.mouseWheel.delta < 0)
					_scale /= 1.5;
				else
					_scale *= 1.5;
			}
		}
	}
}
