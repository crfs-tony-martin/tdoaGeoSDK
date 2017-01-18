#pragma once
#include <iostream>
#include <thread>
#include <future>
#include <mutex> 
#include <condition_variable>
#include <vector>
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

class tdoaResult
{
public:
	tdoaResult() { heatmap.reserve(32768); };
	~tdoaResult() {};
	std::vector<location> nodes;
	struct
	{
		int64_t timeStamp = { 0 };
		location centre;			//emitter location
		double rmsError = { 0 };	//error value at the target location
		struct
		{
			location centre;		//centre of the ellipse (may be offset from emitter centre)
			double major;			//distance along ellipse major axis
			double minor;			//distance along ellipse minor axis
			double angle;			//rotation angle of the ellipse
		} ellipse;
	} target;
	std::vector<location> heatmap;
};

class tdoa
{
public:
	tdoa();
	tdoa(SafeQueue<tdoaResult *> *results) { _resultQ = results; };
	~tdoa();
	std::vector<location> _searchLog;
	tdoaResult *_result = { NULL };
	SafeQueue<tdoaResult *> *_resultQ;
	SafeQueue<capture *> _sharedQ;
	std::map<int64_t, std::vector<capture *> *> _packets;
	std::vector<node *> _nodes;
	std::vector<location> _locations;							//we have a number of them
	fft _fourier;
	bool _heatMap = { true };
	double _rmsError = { 100 };
	void setParams(Json::Value config);
	double error(std::valarray<double> &xyz);
	double negGradient(std::valarray<double> &xyz);
	int32_t correlate(capture *master, capture *slave);
	void process(uint64_t key);
	void manageBuffer();
	void run();
	void stop() { _terminate = true; }
	std::atomic<bool> _terminate = { false };


	node *addNode(Json::Value config);
};

