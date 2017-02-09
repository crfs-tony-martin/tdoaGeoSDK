#pragma once
#include <cmath>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <valarray>
#include <iostream>
#include <fstream>
#include <random>


	class simplex
	{
	std::default_random_engine _dre;
	#define BEST (*(*_simplex.begin()))
	#define WORST (*(*_simplex.rbegin()))
	#define LOUSY (*(*(_simplex.rbegin()+1)))
	#define MID(v1,v2) (v1+v2)/2
	#define F(v1) (*(v1)->y)
	#define ALPHA 1.0
	#define BETA 0.5
	#define GAMMA 1.0
	#define DELTA 0.5
	public:
		simplex() 
		{
		};
		simplex(std::function<double(std::valarray<double>)> callback)
		{
			_callback = callback;
			auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			_dre.seed(seed & 0xffffffff);
		}
		~simplex() {};
		void print()
		{
			std::cout << std::endl << "++++++++++++++++++++++++++++++++" << std::endl;
			for (size_t i = 0; i < _simplex.size(); i++)
			{
				_simplex[i]->print();
			}
			std::cout << std::endl << "--------------------------------" << std::endl;
		}

		double optimise(std::valarray<double> &start, double spread, double threshold = 1e-6)
		{
			std::uniform_real_distribution<double> dist(-spread, spread);
			//Create simplex with n+1 vertices
			for (uint16_t i = 0; i < start.size() + 1; i++)
			{
				vertex *v = new vertex(_callback);
				v->x = start;
				for (size_t i = 0; i < v->x.size(); i++)
				{
					v->x[i] += dist(_dre);
				}
				v->evaluate();
				_simplex.push_back(v);
			}
			//print();
			int i = 0;
			for (i = 0; i < 1000;i++)
			{
				std::sort(_simplex.begin(), _simplex.end(), [](vertex *a, vertex *b) -> bool { return (a->y < b->y); });
				//std::cout << "------------------------------------------------------------- iteration " << i << std::endl;
				//print();
				//Find centroid
				vertex centroid = BEST;
				for (size_t i = 1; i < _simplex.size() - 1; i++)
					centroid += *_simplex[i];
				centroid /= (_simplex.size() - 1);
				centroid.evaluate();
				//centroid.print("centroid = ");
				//Test for covergence
				if (converged(threshold))
					break;
				//Reflect
				vertex reflected = centroid + ((centroid - WORST) * ALPHA);
				reflected.evaluate();
				//print("reflected =");

				if (reflected < BEST)
				{
					vertex expanded = reflected + ((reflected - centroid) * GAMMA);
					expanded.evaluate();
					if (expanded < BEST)
					{
						WORST = expanded;
						//std::cout << "WORST = expanded (97)" << std::endl;
					}
					else
					{
						WORST = reflected;
						//std::cout << "WORST = reflected (102)" << std::endl;
					}
				}
				else if (reflected <= LOUSY)
				{
					WORST = reflected;
					//std::cout << "WORST = reflected (108)" << std::endl;
				}
				else
				{
					if (reflected > WORST)
					{
						//Inside contraction
						vertex contracted = centroid - ((centroid - WORST) * BETA);
						contracted.evaluate();
						//contracted.print("contracted =");
						if (contracted < WORST)
						{
							WORST = contracted;
							//std::cout << "WORST = contracted (121)" << std::endl;
						}
						else
						{
							//Shrink
							//TODO multithread evaluation
							for (auto it = _simplex.begin(); it != _simplex.end(); it++)
							{
								**it = BEST + ((**it - BEST) * DELTA);
								(*it)->evaluate();
							}
							//std::cout << "shrink (131)" << std::endl;
						}
					}
					else
					{
						//Outside contraction
						vertex contracted = centroid + ((centroid - WORST) * BETA);
						contracted.evaluate();
						if (contracted < reflected)
						{
							WORST = contracted;
							//std::cout << "WORST = contracted (142)" << std::endl;
						}
						else
						{
							//Shrink
							//TODO multithread evaluation
							for (auto it = _simplex.begin(); it != _simplex.end(); it++)
							{
								**it = BEST + ((**it - BEST) * DELTA);
								(*it)->evaluate();
							}
							//std::cout << "shrink (152)" << std::endl;
						}
					}
				}
			}
			//print();
			//std::cout << "iterations " << i << std::endl;
			start = BEST.x;
			return BEST.y;
		}

	private:

		class vertex
		{
		public:
			vertex() {};
			vertex(std::function<double(std::valarray<double>)> callback)
			{
				_callback = callback;
			}

			void print(const char *c = NULL)
			{
				if (c)
					std::cout << c << " ";
				for (size_t i = 0; i < x.size(); i++)
				{
					std::cout << x[i] << " ";
				}
				std::cout << "    " << y << std::endl;
			}

			//Comparison operators
			bool operator < (vertex &v) { return y < v.y; };
			bool operator <= (vertex &v) { return y <= v.y; };
			bool operator >= (vertex &v) { return y >= v.y; };
			bool operator > (vertex &v) { return y > v.y; };
			bool operator == (vertex &v) { return y == v.y; };

			vertex &operator += (const vertex &v)
			{
				x += v.x;
				return *this;
			};
			vertex &operator -= (const vertex &v) { x -= v.x; return *this; };
			vertex &operator /= (const double div) { x /= div; return *this; };
			vertex &operator *= (const double m) { x *= m; return *this; };
			vertex &operator / (double div) { x /= div;	return *this; };
			vertex &operator = (const vertex &v) { _callback = v._callback;	x = v.x; y = v.y; return *this; };
			vertex operator+ (const vertex &v1) { vertex sum = *this; sum += v1; return sum; };
			vertex operator* (double m) { vertex sum = *this; sum *= m; return sum; };
			vertex operator- (const vertex &v1) { vertex sum = *this; sum -= v1; return sum; };

			double evaluate()
			{
				y = _callback(x);
				return y;
			}
			std::function<double(std::valarray<double>)> _callback;
			std::valarray<double> x;
			double y;
		};

		bool converged(double threshold)
		{
			double a = WORST.y;
			double b = BEST.y;
			return ((a - b) < threshold);
			//return b < threshold;
		};

		std::vector<vertex *> _simplex;		//A vector of vertices
		std::function<double(std::valarray<double>)> _callback;

	};

