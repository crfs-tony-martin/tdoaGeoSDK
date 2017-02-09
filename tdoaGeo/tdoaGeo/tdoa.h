#pragma once
#include <iostream>
#include <thread>
#include <future>
#include <mutex> 
#include <condition_variable>
#include <vector>
#include <deque>
#include <valarray>
#include <map>
#include <complex>
#include <chrono>
#include <ratio>
#include <random>
#include <numeric>
#include "safeQueue.h"
#include "fft.h"
#include "node.h"
#include "simplex.h"

class ellipse
{
public:
	ellipse() {};
	~ellipse() {};
	location _centre;		//centre of the ellipse (may be offset from emitter centre)
	double _major;			//distance along ellipse major axis
	double _minor;			//distance along ellipse minor axis
	double _angle;			//rotation angle of the ellipse
};

class tdoaResult
{
public:
	tdoaResult() {};
	~tdoaResult() {};
	std::deque<location> _nodes;
	struct
	{
		int64_t _timeStamp = { 0 };
		location _centre;			//emitter location
		ellipse _ellipse;
	} _target;
	std::deque<location> _heatmap;
};

class tdoa
{
public:
	tdoa();
	tdoa(SafeQueue<tdoaResult *> *results) { _resultQ = results; };
	~tdoa();
	std::vector<location> _searchLog;
	location _target;
	SafeQueue<tdoaResult *> *_resultQ;
	SafeQueue<capture *> _sharedQ;
	std::map<int64_t, std::vector<capture *> *> _packets;
	std::deque<node *> _nodes;
	std::deque<location> _locations;						//we have a number of them
	std::deque<location> _heatmap;
	double _bearing;										//rotation angle of the ellipse
	std::ofstream _debug;
	bool _heatMapOn = { true };
	bool _threeDimensions = { false };
	double _minAltitude = { 0 };
	double _badThreshold = { 10 };
	double _rmsError = { 100 };
	void setParams(Json::Value config);
	double error(std::valarray<double> &xyz);
	//double negGradient(std::valarray<double> &xyz);
	double gradient(std::valarray<double> &xyz);
	double excessError(std::valarray<double> &distance);
	int32_t correlate(capture *master, capture *slave);
	void process(uint64_t key);
	void manageBuffer();
	void run();
	void stop() { _terminate = true; }
	std::atomic<bool> _terminate = { false };


	node *addNode(Json::Value config);
};

