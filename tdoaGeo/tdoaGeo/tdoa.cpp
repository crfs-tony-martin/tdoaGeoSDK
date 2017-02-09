#include "tdoa.h"

tdoa::tdoa() {}

tdoa::~tdoa() {}

//Function which returns the rms time error at the point passed. The search aims
//to minimise the error. That is find coordinates which give consistent time differences
//to those held in _timeDeltas
double tdoa::error(std::valarray<double> &xyz)
{
	//Calculate the time of flight from the location passed to each of the nodes.
	std::vector<double> time;
	//Convert valarray to a point in space
	location loc;

	//If there are only two points then assume search is constrained to the surface of the earth
	if (xyz.size() < 3 || _threeDimensions == false)
		loc.setCartesian(xyz[0], xyz[1]);
	else
		loc.setCartesian(xyz[0], xyz[1], xyz[2]);

	//If more than 100km or below min altitude then consider it invalid
	if (loc.distance(_locations[0]) > 100000 || loc.getAlt() < _minAltitude)
		return std::numeric_limits<double>::max();
	std::vector<double> metres;
	for (size_t i = 0; i < _locations.size(); i++)
	{
		double m = loc.distance(_locations[i]);
		metres.push_back(m);
		double seconds = m / SPEED_OF_LIGHT;
		//Convert to time of flight in ns
		time.push_back(1e9 * m / SPEED_OF_LIGHT);
	}
	double result = 0;
	//std::lock_guard<std::mutex> lck(cout_mtx);
	//Calculate the square of the difference of this guess from the measured values  
	for (size_t i = 0; i < _locations.size(); i++)
	{
		double difference = time[i] - time[0];
		//if(_debug.is_open())
		//	_debug << loc.getLat() << ", " << loc.getLon() << " " << loc.getAlt() << "m " << _locations[i].getLat() << ", " << _locations[i].getLon() << " " << _locations[i].getAlt() << "m " << loc.distance(_locations[i])
		//			 << "m calc delta: " << difference << "ns meas delta " << _locations[i]._timeDelta << "ns" << std::endl;
		difference -= _locations[i]._timeDelta;
		result += pow(difference, 2);
	}
	result = sqrt(result / _locations.size());
	//if (_debug.is_open())
	//{
	//	_debug << " rms: " << result << "ns" << std::endl;
	//	_debug.flush();
	//}
	if (!_heatMapOn)
	{
		loc._error = result;
		_heatmap.push_back(loc);
	}
	return result;
}

//Function which returns the rmsError at 100m from centre at the specified angle.
//Used by the optimiser in locating the ellipse minor axis bearing
double tdoa::gradient(std::valarray<double> &alpha)
{
	location loc = _target;
	loc.move(1000, RAD_TO_DEG(alpha[0]));
	std::valarray<double> position(3);
	loc.getCartesian(position);
	//Look for the minimum
	return error(position);
}

//Function which returns the error relative at the defined distance from the centre at angle _bearing.
//Used by the optimiser in finding the major & minor axis lengths
double tdoa::excessError(std::valarray<double> &distance)
{
	if(distance[0] < 0)
		return std::numeric_limits<double>::max();
	location loc = _target;
	loc.move(distance[0], _bearing);
	std::valarray<double> position(3);
	loc.getCartesian(position);
	//Look for the maximum
	return std::abs(_rmsError - error(position));
}


//Get time difference between two data sets
int32_t tdoa::correlate(capture *master, capture *slave)
{
	//std::cout << "correlating " << master->_host << ":" << std::to_string(master->_port) << " with " << slave->_host << ":" << std::to_string(slave->_port) << std::endl;
	//If we don't get a valid correlation for some reason, then return an impossibly large value
	int32_t ns = std::numeric_limits<int32_t>::max();
	//If master and slave one and the same return a difference of 0
	if (master == slave)
		return 0;
	//NB processing will overwrite the slave
	int32_t decimation = master->_deci;
	int32_t sampleRate = master->_srtt;		//MHz
	if (master->_deci == 0 || slave->_deci == 0)
		return  std::numeric_limits<int32_t>::max();

	if (master->iqData.size() != slave->iqData.size())
	{
		//std::lock_guard<std::mutex> lk(cout_mtx);
		std::cout << "inconsistent data" << std::endl;
		return ns;
	}

	//If more than 100ns apart then abort
	if (std::abs(master->_time - slave->_time) > 100)
	{
		//std::lock_guard<std::mutex> lk(cout_mtx);
		std::cout << "synchroniser failure" << std::endl;
		return ns;
	}

	//Nodes performed FFT so we already have the spectra and master should be conjugated
	//Multiply together and invert the FFT
	slave->iqData *= master->iqData;
	fft fourier;
	fourier.invert(slave->iqData);
	slave->iqData = slave->iqData.cshift(slave->iqData.size() / 2);
	//Copy absolute values from complex result
	std::vector<double>correlation(slave->iqData.size());
	for (size_t i = 0; i < correlation.size(); i++)
	{
		correlation[i] = std::abs(slave->iqData[i]);
	}

	if (correlation.size() > 0)
	{
		auto max = std::max_element(correlation.begin(), correlation.end());
		//Return the peak sample
		int32_t offset = static_cast<int32_t>(std::distance(correlation.begin(), max));
		double sum = std::accumulate(correlation.begin(), correlation.end(), 0.0);
		double peak = *max;
		double mean = sum / correlation.size();
		double peakToMean = peak / mean;
		//Cross correlation is lousy so assume we've lost it
		if (peakToMean < 5)
		{
			std::cout << "peakToMean " << peakToMean << std::endl;
			return ns;
		}
		offset -= correlation.size() / 2;

		double sampleTime = static_cast<double>(decimation * 1000) / static_cast<double>(sampleRate);	//ns
		ns = static_cast<int32_t>(offset * sampleTime);
		//std::lock_guard<std::mutex> lk(cout_mtx);
		//std::cout << slave->_port << " vs " << master->_port <<  " offset " << offset << " at " << sampleTime << "ns = " << ns << "ns " << peakToMean << std::endl;
	}
	return ns;
}

void tdoa::process(uint64_t key)
{
	//The set of captures that we're going to process
	//std::cout << "processing " << key << " ";
	if (_packets.find(key) == _packets.end())
		return;
	_heatmap.clear();
	_locations.clear();
	if (_packets[key]->size() > 2)
	{
		//Sort in order of power
		std::sort(_packets[key]->begin(), _packets[key]->end(), [](const capture *a, const capture *b)
		{
			return a->_power > b->_power;
		});
		//Master is the one with highest power
		auto master = _packets[key]->front();
		//Avoid overdetermined condition by using only the 3/4 strongest nodes
		size_t count = 3;
		if (_threeDimensions)
			count = 4;
		while (_packets[key]->size() > count)
		{
			_packets[key]->pop_back();
		}

		//Conjugate the master
		master->iqData = master->iqData.apply(std::conj);
		//Run the correlations concurrently. correlation returns ns	
		std::vector<std::future<int32_t>> ftrs;
		for (auto pkt : *_packets[key])
			ftrs.push_back(std::async(std::launch::async, &tdoa::correlate, this, master, pkt));

		//Iterate over the futures to get the time differences from master. Each future will block until completed.
		//The master is the front entry in the vector, so make sure we observe the sorted order of the packets. 
		size_t i = 0;
		for (auto &ftr: ftrs)
		{
			int32_t ns = ftr.get();
			if (ns != std::numeric_limits<int32_t>::max())
			{
				capture *pkt = _packets[key]->at(i++);
				//std::cout << key << " " << pkt->_lati / 1e6 << " " << pkt->_long / 1e6 << " " << ns << "ns" << std::endl;
				location loc(pkt->_lati / 1e6, pkt->_long / 1e6, pkt->_alti / 1e3);
				loc._timeDelta = ns;
				loc._power = pkt->_power;
				loc._gain = pkt->_gain;
				loc._port = pkt->_port;
				loc._host = pkt->_host;
				//Save location of the node in the array
				_locations.push_back(loc);
			}
		}
		//Now have everything needed to solve for location so long as we have at least 3 nodes
		//If not enough locations to geolocate then don't bother.
		double confidence = _badThreshold + 1;
		std::valarray<double> xyz(2);
		tdoaResult *result = new tdoaResult;
		if (_locations.size() > 2)
		{
			//Give the optimiser a second chance if needed
			for (int i = 0; i < 2 && confidence >= _badThreshold; i++)
			{
				//Use master position as start of search for optimum
				_locations.front().getCartesian(xyz);

				if (_locations.size() < 4 || _threeDimensions == false)
				{	
					double x = xyz[X];
					double y = xyz[Y];
					xyz.resize(2);
					xyz[X] = x;
					xyz[Y] = y;
				}
				//Tell the optimiser how to calculate error function values 
				using namespace std::placeholders;
				simplex splx(std::bind(&tdoa::error, this, _1));
				//Optimise overwrites xyz with result. If have only 3 nodes we should constrain the 
				//search to the surface of the earth. This is done by passing only x and y data to the
				//solver
				confidence = splx.optimise(xyz, 1000, 1e-6);
				//std::cout << "attempt " << i << " confidence " << confidence << std::endl;
			}

			if (confidence < _badThreshold)
			{
				result->_target._timeStamp = master->_time;
				//Optimisation carried out in cartesian space. Convert back to spherical
				result->_target._centre.setCartesian(xyz);
				_target = result->_target._centre;
				result->_target._centre._error = confidence;

				result->_nodes = _locations;
				location best = result->_target._centre;
				if (_heatMapOn)
				{
					_heatmap.clear();
					for (double lat = result->_target._centre.getLat() - 0.01; lat < result->_target._centre.getLat() + 0.01; lat += 0.0002)
					{
						for (double lon = result->_target._centre.getLon() - 0.02; lon < result->_target._centre.getLon() + 0.02; lon += 0.0005)
						{
							location loc(lat, lon, result->_target._centre.getAlt());
							std::valarray<double> map(3);
							loc.getCartesian(map);
							loc._error = error(map);
							if (loc._error < best._error)
								best = loc;
							_heatmap.push_back(loc);
						}
					}
				}

				//Find the minor axis of the elipse. This involves searching for the direction with the maximum uphill gradient.
				//This is more robust than searching for the major axis which may lie along a valley. Use the optimiser as it will
				//require far fewer iterations than a brute force search.
				std::valarray<double> alpha(1);			//radians
				using namespace std::placeholders;
				simplex splx(std::bind(&tdoa::gradient, this, _1));
				//Optimise overwrites alpha with result.
				splx.optimise(alpha, PI/8, 1e-3);

				_bearing = (180 * alpha[0] / PI);
				result->_target._ellipse._angle = _bearing + 90;
				//Now we have the orientation of the ellipse search for the defined rms error
				simplex splx2(std::bind(&tdoa::excessError, this, _1));
				//Optimise overwrites shift with result.
				std::valarray<double> shift{ 1000 };
				splx2.optimise(shift, 100, 1);
				double offset1 = shift[0];
				//Repeat for the major axis in opposite direction
				_bearing -= 180;
				splx2.optimise(shift, 100, 1);
				double offset2 = shift[0];
				result->_target._ellipse._major = offset1 + offset2;
				result->_target._ellipse._centre = _target;
				//Shift the ellipse centre along the major axis
				result->_target._ellipse._centre.move((offset2 - offset1) / 2, _bearing);
				_target = result->_target._ellipse._centre;
				//finally do minor axis
				shift[0] /= 4;
				_bearing += 90;
				splx2.optimise(shift, 10, 1);
				result->_target._ellipse._minor = 2 * shift[0];

				result->_heatmap = _heatmap;
				_resultQ->push(result);
				if (_debug.is_open())
					_debug << result->_target._centre.getLat() << ", " << result->_target._centre.getLon() << ", " << result->_target._centre.getAlt() << std::endl;
				std::lock_guard<std::mutex> lk(cout_mtx);
				std::cout << "result " << result->_target._centre.getLat() << ", " << result->_target._centre.getLon() << " " << result->_target._centre.getAlt() << "m " << confidence << std::endl;
				//for (auto l : _locations)
				//{
				//	std::cout << l._port << " power: " << l._power << " gain: " << l._gain << std::endl;
				//}
			}
			else
			{
				delete result;
				std::cout << "bad result " << confidence << std::endl;
			}
		}
		//if (_debug.is_open())
		//	_debug.close();
	}
	//clean up
	//std::cout << "deleting " << key << " entries " << _packets[key]->size() << std::endl;
	for (auto pos : *_packets[key])
		delete pos;

	_packets.erase(key);
}

void tdoa::manageBuffer()
{
	if (_packets.size() == 0)
		return;
	//Once buffer has reached maximum size, delete the oldest 
	if (_packets.size() > 5)
		process(_packets.begin()->first);
		
	for (auto pkts : _packets)
	{
		//If we have all the node packets then process
		if ((pkts.second->size()) == _nodes.size())
		{
			process(pkts.first);
			//Recursively process one entry as deletion corrupts iterator loop
			manageBuffer();
			break;
		}
	}
	//std::cout << "size = " << _packets.size() << std::endl;
}

void tdoa::setParams(Json::Value config)
{
	//Create nodes according to config
	std::string s = config.toStyledString();
	std::cout << s << std::endl;
	//Create a number of nodes
	Json::Value nodes = config["nodes"];
	Json::Value tdoa = config["tdoa"];
	_heatMapOn = tdoa.get("heatMapOn", _heatMapOn).asBool();
	_threeDimensions = tdoa.get("threeDimensions", _threeDimensions).asBool();
	_rmsError = tdoa.get("rmsError", _rmsError).asDouble();
	_badThreshold = tdoa.get("badThreshold", _badThreshold).asDouble();
	std::string debugFile = tdoa.get("debugFile", "").asString();
	if(!debugFile.empty())
		_debug.open(debugFile, std::ios::out);

	for (Json::ArrayIndex i = 0; i < nodes.size(); i++)
	{
		node *n = addNode(nodes[i]);
		n->setParams(nodes[i]);
		n->setParams(tdoa);
	}
}

node *tdoa::addNode(Json::Value config)
{
	std::string host = config.get("host", "host").asString();
	uint32_t port = config.get("port", 9999).asInt();
	//If this node already exists return a reference to it
	for (node *n : _nodes)
	{
		if (n->_host == host && n->_port == port)
			return n;
	}
	//Node doesn't already exist so create it and add
	node *n = new node(&_sharedQ);
	_nodes.push_back(n);
	return n;
}

void tdoa::run()
{

	//Run each node in a thread of its own
	std::vector<std::thread> threads;
	for (auto n : _nodes)
	{
		threads.push_back(std::thread(&node::run, n));
	}

	while(!_terminate)
	{
		//Pop a pointer to IQ data from a node.
		//We will be blocked waiting for data to appear.
		capture *packet = _sharedQ.front();
		//add the pointer to a multimap entry.
		if (packet->iqData.size() > 0)
		{
			//std::cout << "packet from " << packet->_host << ":" << packet->_port << " " << packet->_time 
			//	      << "lat " << packet->_lati << " " << packet->_long << " " << packet->_alti << std::endl;
			//Set key resolution to 1ms for synchronisation
			int64_t key = packet->_time / 1000000;
			//If first with this key then create a vector
			if (_packets.find(key) == _packets.end())
			{
				std::vector<capture *> *cohort = new std::vector<capture *>;
				_packets[key] = cohort;
			}
			//Add the packet
			_packets[key]->push_back(packet);
			manageBuffer();
		}
		else
		{
			delete packet;
		}
		_sharedQ.pop();
	}

	for (auto n : _nodes)
		n->stop();

	for (auto& t : threads)	t.join();
}