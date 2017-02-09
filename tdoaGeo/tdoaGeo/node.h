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
#include "location.h"


extern std::mutex cout_mtx;

typedef std::valarray<std::complex<double>> TSignal;
//typedef struct { double x; double y; double z; } location;	//location is x, y, z



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
	bool operator < (capture &c) { return _power < c._power; };
	TSignal iqData;
	double _power = { 0 };
	int32_t _srtt = { 40 };		//Only returned by Nexus so default to 40MHz for axis/marvell
	int32_t _deci = { 0 };
	int32_t _gfix = { 0 };
	int32_t _alti = { 0 };		
	int32_t _lati = { 0 };		//micro degrees
	int32_t _long = { 0 };		//micro degrees
	int64_t _time = { 0 };
	int32_t _gain = { 640 };	//dB/16 = 40dB
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
	void UpSampleSpectrum(uint32_t interpolation, TSignal &spectrum);
	T_PACKET* get_rx_packet();
	std::atomic_bool  _terminate = { false };
	std::mutex _mtx;
	uint32_t _measureInterval_ms;
	uint64_t _frequency_Hz;
	uint64_t _bandwidth_Hz;
	uint32_t _samples = { 1024 };
	double _minSampleRate = { 10e6 };
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

