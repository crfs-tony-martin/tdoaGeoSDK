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

	//TODO this is slow because it needlessly converts to polar
	//If there are only two points then assume search is constrained to the surface of the earth
	if (xyz.size() < 3)
	{
		double z = pow(EARTH_RADIUS, 2) - pow(xyz[0], 2) - pow(xyz[1], 2);
		loc.setCartesian(xyz[0], xyz[1], sqrt(std::abs(z)));
	}
	else
		loc.setCartesian(xyz[0], xyz[1], xyz[2]);

	if (loc.distance(_locations[0]) > 100000)
		return std::numeric_limits<double>::max();

	for (size_t i = 0; i < _locations.size(); i++)
	{
		double m = loc.distance(_locations[i]);
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
		//std::cout << "Evaluating " << loc._lat << " " << loc._lon << " " << loc._alt 
		//	      << " calc delta: " << difference << "ns meas delta " << _timeDeltas[i] << "ns" << std::endl;
		difference -= _locations[i]._timeDelta;
		result += pow(difference, 2);
	}
	//if(isnan(loc._x))
	//	std::cout << loc._x << " " << loc._y << " " << loc._z << " lat: " << loc._lat << " lon: " << loc._lon << " alt: " << loc._alt << " rms: " << sqrt(result) << "ns" << std::endl;

	result = sqrt(result);
	if (!_heatMap)
	{
		loc._error = result;
		_result->heatmap.push_back(loc);
	}
	return result;
}

//Function which returns the rmsError at 100m at the specified angle.
//Used by the optimiser in locating the ellipse minor axis bearing
double tdoa::negGradient(std::valarray<double> &alpha)
{
	std::complex<double> centre(_result->target.centre._x, _result->target.centre._y);
	std::complex<double> point = centre + std::polar<double>(100, alpha[0]);
	std::valarray<double> position(2);
	position[0] = point.real(); 	position[1] = point.imag();
	return -error(position);
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
	_fourier.invert(slave->iqData);
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
		if (offset > correlation.size() / 2)
			offset -= correlation.size();

		double sampleTime = static_cast<double>(decimation * 1000) / static_cast<double>(sampleRate);	//ns
		ns = static_cast<int32_t>(offset * sampleTime);
		//std::lock_guard<std::mutex> lk(cout_mtx);
		//std::cout << slave->_port << " vs " << master->_port <<  " offset " << offset << " = " << ns << "ns " << peakToMean << std::endl;
	}
	return ns;
}

void tdoa::process(uint64_t key)
{
	//The set of captures that we're going to process
	//std::cout << "processing " << key << " ";
	if (_packets.find(key) == _packets.end())
		return;

	_locations.clear();
	if (_packets[key]->size() > 2)
	{
		//Sort in order of power
		std::sort(_packets[key]->begin(), _packets[key]->end(), [](const capture *a, const capture *b)
		{
			return a->power > b->power;
		});
		//Master is the one with highest power
		auto master = _packets[key]->front();
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
				//Save location of the node in the array
				_locations.push_back(loc);
			}
		}
		//Now have everything needed to solve for location so long as we have at least 3 nodes
		//If not enough locations to geolocate then don't bother.
		double confidence = 100;
		std::valarray<double> xyz(2);
		tdoaResult *result = new tdoaResult;
		_result = result;
		if (_locations.size() > 2)
		{
			//Give the optimiser a second chance if needed
			for (int i = 0; i < 2 && confidence > 1; i++)
			{
				//Use master position as start of search for optimum
				xyz.resize(2);
				if (_locations.size() > 3)
				{
					xyz.resize(3);
					xyz[2] = _locations.front()._z;
				}
				xyz[0] = _locations.front()._x;
				xyz[1] = _locations.front()._y;

				//Tell the optimiser how to calculate error function values 
				using namespace std::placeholders;
				simplex splx(std::bind(&tdoa::error, this, _1));
				//Optimise overwrites xyz with result. If have only 3 nodes we should constrain the 
				//search to the surface of the earth. This is done by passing only x and y data to the
				//solver
				confidence = splx.optimise(xyz, 1000, 1e-3);
				//std::cout << "attempt " << i << " confidence " << confidence << std::endl;
			}

			if (confidence < 1)
			{
				result->target.rmsError = confidence;
				result->target.timeStamp = master->_time;
				//Optimisation carried out in cartesian space. Convert back to spherical
				if (_locations.size() > 3)
					result->target.centre.setCartesian(xyz[0], xyz[1], xyz[2]);
				else
					result->target.centre.setCartesian(xyz[0], xyz[1], sqrt(std::abs(pow(EARTH_RADIUS, 2) - pow(xyz[0], 2) - pow(xyz[1], 2))));
				
				result->nodes = _locations;
				if (_heatMap)
				{
					result->heatmap.reserve(65536);
					for (double lat = result->target.centre._lat - 0.01; lat < result->target.centre._lat + 0.01; lat += 0.0002)
					{
						for (double lon = result->target.centre._lon - 0.02; lon < result->target.centre._lon + 0.02; lon += 0.0005)
						{
							location loc(lat, lon, 0);
							std::valarray<double> map(2);
							map[0] = loc._x; map[1] = loc._y;
							loc._error = error(map);
							result->heatmap.push_back(loc);
						}
					}
				}
				//Find the minor axis of the elipse. This involves searching for the direction with the maximum uphill gradient.
				//This is more robust than searching for the major axis which may lie along a valley. Use the optimiser as it will
				//require far fewer itearions than a brute force search.
				std::complex<double> centre(xyz[0], xyz[1]);
				std::valarray<double> alpha;
				alpha.resize(1);
				using namespace std::placeholders;
				simplex splx(std::bind(&tdoa::negGradient, this, _1));
				//Optimise overwrites alpha with result.
				double errorVal = splx.optimise(alpha, PI/8, 1e-3);

				result->target.ellipse.angle = RAD_TO_DEG(alpha[0]);
				//Now we have the orientation of the ellipse search for the defined rms error
				double offset1 = 1, offset2 = 1, offset3 = 1;
				do
				{
					std::complex<double> point = centre + std::polar<double>(offset1, alpha[0]);
					xyz[0] = point.real(); 	xyz[1] = point.imag();
					offset1 += 1;
				}
				while (error(xyz) < _rmsError);
				result->target.ellipse.minor = offset1;
				//Repeat for the major axis, using both directions
				do
				{
					std::complex<double> point = centre + std::polar<double>(offset2, alpha[0] + (PI/2));
					xyz[0] = point.real(); 	xyz[1] = point.imag();
					offset2 += 1;
				} while (error(xyz) < _rmsError + result->target.rmsError);
				do
				{
					std::complex<double> point = centre + std::polar<double>(offset3, alpha[0] - (PI / 2));
					xyz[0] = point.real(); 	xyz[1] = point.imag();
					offset3 += 1;
				} while (error(xyz) < _rmsError + result->target.rmsError);

				//Major and minor axes lengths in metres
				result->target.ellipse.minor = 2 * offset1;
				result->target.ellipse.major = offset2 + offset3;
				//std::cout << "ellipse: " << result->target.ellipse.major << " x " << result->target.ellipse.minor << std::endl;
				//Shift the ellipse centre along the major axis
				std::complex<double> point = centre + std::polar<double>(offset2 - offset3, alpha[0] + (PI / 2));
				xyz[0] = point.real(); xyz[1] = point.imag();
				double z = sqrt(std::abs(pow(EARTH_RADIUS, 2) - pow(xyz[0], 2) - pow(xyz[1], 2)));
				result->target.ellipse.centre.setCartesian(xyz[0], xyz[1], z);
				_resultQ->push(result);
				std::cout << "good result " << confidence << std::endl;
			}
			else
			{
				delete result;
				std::cout << "bad result " << confidence << std::endl;
			}
		}
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
	if (_packets.size() > 100)
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
	_heatMap = tdoa.get("heatMap", _heatMap).asBool();
	_rmsError = tdoa.get("rmsError", _rmsError).asDouble();

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
			//std::cout << "packet from " << packet->_host << ":" << packet->_port << " " << packet->_time << std::endl;
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