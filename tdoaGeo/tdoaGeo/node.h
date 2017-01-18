#pragma once
#include "packets.h"
#include "ncp_client.h"
#include "ncp_packets.h"
#include <boost/any.hpp>
#include <vector>
#include <valarray>
#include <complex>
#include <thread>
#include <chrono>
#include <iostream>
#include <map>
#include <atomic>
#include <mutex>
#include <memory>
#include <random>
#include <numeric>
#include <atomic>
#include "safeQueue.h"
#include "json.h"
#include "fft.h"


#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286
#define EARTH_RADIUS 6371000
#define DEG_TO_RAD(a) (a*PI/180)
#define RAD_TO_DEG(a) (180*a/PI)
#define TO_LAT_LONG(x) (static_cast<double>(x)/1e7)
#define FROM_LAT_LONG(x) (static_cast<int32_t>(x*1e7))
#define SPEED_OF_LIGHT 299792458
#define FREE_SPACE_LOSS(dist_m,freq_Hz) (dist_m*freq_Hz>0?20*(log10(dist_m)+log10(freq_Hz)+log10(4*PI/SPEED_OF_LIGHT))

extern std::mutex cout_mtx;

typedef std::valarray<std::complex<double>> TSignal;
//typedef struct { double x; double y; double z; } location;	//location is x, y, z

class location
{
public:
	location() {};
	location(double lat, double lon, double alt)
	{
		setSpherical(lat, lon, alt);
	}
	~location() {};
	bool operator == (location &l) { return l._lat == _lat && l._lon == _lon; };
	bool operator <= (location &l) { return (l == *this || l < *this); };
	bool operator !=  (location &l) { return !(l == *this); };
	bool operator < (location &l) { return (l._lat + l._lon < _lat + _lon); };
	void setSpherical(double lat, double lon, double alt)
	{
		_lat = lat;
		_lon = lon;
		_alt = alt;
		latLongAltToRect();
	}
	void setCartesian(double x, double y, double z)
	{
		_x = x; _y = y; _z = z;
		latLongAltFromRect();
	}
	//Native is cartesian
	double _x; double _y; double _z;
	double _lat; double _lon; double _alt;
	//Error between measured and calculated value at this location
	double _error = { 0 };
	//Convert lat, long and altitude coordinates to a point in cartesian space with earth centre at origin
	void latLongAltToRect()
	{
		double phi = 90 - _lat;			//lat
		double theta = _lon;			//long
		double radius = _alt + EARTH_RADIUS;		//Altitude

		_x = radius * sin(DEG_TO_RAD(phi)) * cos(DEG_TO_RAD(theta));
		_y = radius * sin(DEG_TO_RAD(phi)) * sin(DEG_TO_RAD(theta));
		_z = radius * cos(DEG_TO_RAD(phi));
	}

	//Convert cartesian coordinates of a point in space realive to earth centre to lat/long/alt
	void latLongAltFromRect()
	{
		double radius = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
		double theta = RAD_TO_DEG(atan2(_y, _x));
		double phi = RAD_TO_DEG(atan2(sqrt(pow(_x, 2) + pow(_y, 2)), _z));

		_lon = theta;
		_lat = 90 - phi;
		_alt = radius - EARTH_RADIUS;
	}

	double distance(location &loc2)
	{
		latLongAltToRect();
		return sqrt(pow(_x - loc2._x, 2.0) + pow(_y - loc2._y, 2.0) + pow(_z - loc2._z, 2.0));
	}

	//Delay at this location relative to the master
	int32_t _timeDelta = { 0 };
};

class capture
{
public:
	capture(const char *host, const uint32_t port)
	{
		_host = host;
		_port = port;
	};
	~capture()
	{
	}
	bool operator < (capture &c) { return power < c.power; };
	TSignal iqData;
	double power = { 0 };
	int32_t _srtt = { 40 };		//Only returned by Nexus so default to 40MHz for axis/marvell
	int32_t _deci = { 0 };
	int32_t _gfix = { 0 };
	int32_t _alti = { 0 };		
	int32_t _lati = { 0 };		//micro degrees
	int32_t _long = { 0 };		//micro degrees
	int64_t _time = { 0 };
	int32_t _gain = { 0 };
	int32_t _index = { 0 };
	std::string _host = { "" };
	uint32_t _port = { 0 };
};


class node
{
public:
	node() {};
	node(SafeQueue<capture *> *q) { _result = q; };
	virtual ~node() {};

	void setParams(Json::Value &config);

	void run();
	void stop() { _terminate = true; };
	std::string _host;
	uint32_t _port = { 9999 };
protected:
	void ackWait();
	std::vector<node *> *_nodes;
	void startMission();
	capture *getPacketData(T_PACKET* packet);
	capture *getPacketData(void);				//used for generating test data
	bool isConnected();
	bool connect();
	void disconnect();
	void receive_packet();
	void send_packet(T_PACKET *packet);
	T_PACKET* get_rx_packet();
	std::atomic_bool  _terminate = { false };
	std::mutex _mtx;
	uint32_t _measureInterval_ms;
	uint64_t _frequency_Hz;
	uint64_t _bandwidth_Hz;
	uint32_t _samples = { 1024 };
	bool _nexusNode = { false };
	//uint64_t _startTime = { 0 };
	uint32_t _field_id = { 0 };
	fft _fourier;
	SafeQueue<capture *> *_result;
	T_NCP_CLIENT_CONNECTION* _ncp_client = { NULL };

	//The following are applicable to test mode only
	bool _testMode = { false };
	std::atomic<int> TxDelay_ns = { 0 };
	location _loc;
	std::chrono::time_point<std::chrono::system_clock> _timeGrid;
	location _transmitter;		
};

