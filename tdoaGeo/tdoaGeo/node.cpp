#include "node.h"
#include <queue>

//#include "windows.h"

//Test function to generate simulated data packets.
capture *node::getPacketData(void)
{
	capture *r = new capture(_host.c_str(), _port);
	try
	{
		_timeGrid += std::chrono::milliseconds(_measureInterval_ms);
		std::this_thread::sleep_until(_timeGrid);
		//Make sure we don't don't generate data while parameters being updated
		std::lock_guard<std::mutex> lck(_mtx);
		r->_gfix = 2;
		std::valarray<double> position(3);
		_loc.getSpherical(position);
		r->_alti = position[ALT] * 1e3;
		r->_lati = position[LAT] * 1e6;
		r->_long = position[LON] * 1e6;
		r->_deci = 40e6 / _bandwidth_Hz;
		r->_time = _timeGrid.time_since_epoch().count();
		int size = 1;
		while (size < _samples)
			size *= 2;
		size /= 2;
		r->iqData.resize(size);
		double metres = _loc.distance(_transmitter);
		r->_power = 1 / (1 + metres);
		double flightTime = metres / SPEED_OF_LIGHT;
		std::default_random_engine dre;
		dre.seed(r->_time & 0xffffffff);
		std::uniform_real_distribution<double> modulation(-1, 1);
		for (size_t i = 0; i < r->iqData.size(); i++)
			r->iqData[i] = std::complex<double>(modulation(dre), modulation(dre));
		//implement time shift in frequency domain
		fft fourier;
		fourier.transform(r->iqData);
		r->iqData = r->iqData.cshift(static_cast<int>(r->iqData.size()) / 2);
		//Calculate the actual bandwidth based on sample rate and decimation
		double bw = static_cast<double>(1e6 * r->_srtt) / static_cast<double>(r->_deci);
		double freq = _frequency_Hz - (bw / 0.5);
		double fftBin = bw / r->iqData.size();
		for (size_t i = 0; i < r->iqData.size(); i++)
		{
			r->iqData[i] *= std::polar<double>(1.0, -2 * PI * freq * flightTime);
			freq += fftBin;
		}
		r->iqData = r->iqData.cshift(static_cast<int>(r->iqData.size()) / 2);
		fourier.invert(r->iqData);
		//std::lock_guard<std::mutex> lk(cout_mtx);
		//std::cout << _port << " " << metres << "m " << flightTime << "s " << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	return r;
};

capture *node::getPacketData(T_PACKET* packet)
{

	capture *r = new capture(_host.c_str(), _port);
	try
	{
		if (packet != NULL)
		{
			int fname, fid;
			packet_read(packet);
			double power = 0;
			/* Loop through all the fields */
			while (packet_get_next_field(packet, &fname, &fid) == 1)
			{
				//std::cout << "packet: " << packet << std::hex << " field: " << fname << " id: " << fid << std::endl;
				void* pdata;
				int pname, plength;
				T_PARAM_DATA_TYPE ptype;
				while (packet_get_next_param(packet, &pname, &ptype, &pdata, &plength) == 1)
				{
					if (strstr(packet_key_to_str(pname, NULL), "WARN") != NULL)
					{
						printf("%s\n", (char*)pdata);
						//				return;
					}
					switch (fname)
					{
					case 0x53504753:	//SGPS backwards
						if (strcmp(packet_key_to_str(pname, NULL), "GFIX") == 0)
							r->_gfix = static_cast<int32_t>(*static_cast<int32_t *>(pdata));
						if (strcmp(packet_key_to_str(pname, NULL), "ALTI") == 0)
							r->_alti = static_cast<int32_t>(*static_cast<int32_t *>(pdata));
						if (strcmp(packet_key_to_str(pname, NULL), "LATI") == 0)
							r->_lati = static_cast<int32_t>(*static_cast<int32_t *>(pdata));
						if (strcmp(packet_key_to_str(pname, NULL), "LONG") == 0)
							r->_long = static_cast<int32_t>(*static_cast<int32_t *>(pdata));
						break;
					case 0x454d4954:	//TIME backwards
						if (strcmp(packet_key_to_str(pname, NULL), "SRTT") == 0)
						{
							r->_srtt = static_cast<int32_t>(*static_cast<int32_t *>(pdata));
							//std::cout << _host << packet_key_to_str(pname, NULL) << " = "
							//	<< static_cast<int32_t>(*static_cast<int32_t *>(pdata)) << std::endl;
						}
						else if (strcmp(packet_key_to_str(pname, NULL), "DECI") == 0)
						{
							r->_deci = static_cast<int32_t>(*static_cast<int32_t *>(pdata));
							//std::cout << _host << packet_key_to_str(pname, NULL) << " = "
							//	<< static_cast<int32_t>(*static_cast<int32_t *>(pdata)) << std::endl;
						}
						else if (strcmp(packet_key_to_str(pname, NULL), "SRTF") == 0)
						{
							//std::cout << _host << packet_key_to_str(pname, NULL) << " = "
							//	<< static_cast<int32_t>(*static_cast<int32_t *>(pdata)) << std::endl;
						}
						else if (strcmp(packet_key_to_str(pname, NULL), "SAMP") == 0)
						{
							//std::cout << _host << packet_key_to_str(pname, NULL) << " = "
							//	<< static_cast<int32_t>(*static_cast<int32_t *>(pdata)) << std::endl;
						}
						else if (strcmp(packet_key_to_str(pname, NULL), "RGAI") == 0)
						{
							r->_gain = static_cast<int32_t>(*static_cast<int32_t *>(pdata));
							//std::cout << _host << packet_key_to_str(pname, NULL) << " = "
							//	<< static_cast<int32_t>(*static_cast<int32_t *>(pdata)) << std::endl;
						}
						else if (strcmp(packet_key_to_str(pname, NULL), "NANO") == 0)
							r->_time += static_cast<int32_t>(*static_cast<int32_t *>(pdata));
						else if (strcmp(packet_key_to_str(pname, NULL), "UTIM") == 0)
							r->_time += (1000000000 * static_cast<int64_t>(*static_cast<int32_t *>(pdata)));

						if (ptype != PARAM_INT && ptype != PARAM_UNSIGNED_INT && ptype != PARAM_STRING)
						{
							//std::cout << _host << " returned " << plength << " samples" << std::endl;
							int size = 1;
							while (size < plength / 2)
								size *= 2;
							size /= 2;
							r->iqData.resize(size);
							//std::cout << plength << " samples size: " << size << std::endl;
							int16_t *iq = static_cast<int16_t *>(pdata);
							//Copy in last first so we discard any warm up samples
							power = 0;
							for (int i = plength - 1, j = size - 1; j >= 0; i -= 2, j--)
							{
								r->iqData[j] = std::complex<double>(iq[i - 1], iq[i]);
								power += pow(std::abs(r->iqData[j]), 2);
							}
							power /= size;
							power = sqrt(power);
						}
						//std::cout << _host << packet_key_to_str(pname, NULL) << " = "
						//	<< static_cast<int32_t>(*static_cast<int32_t *>(pdata)) << std::endl;
					}
				}
			}
			//Apply any gain to the power
			double gain = r->_gain;
			gain = pow(10, gain / 160);
			r->_power = power / gain;
			//std::lock_guard<std::mutex> lk(cout_mtx);
			//std::cout << _port << ": " << r->_gain << ", " << r->power << std::endl;
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	//std::cout << "packet received " << r->_lati << " " << r->_long << " " << r->iqData.size() << "samples\n";
	return r;
}

bool node::connect()
{
	std::cout << "connecting " << _host << ":" << std::to_string(_port) << std::endl;
	_timeGrid = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());

	//Capture time used if in test mode
	if (_testMode)
		return true;

	int32_t ret = 0;
	if (_ncp_client)
	{
		ncp_client_free(_ncp_client);
		_ncp_client = NULL;
	}
	_ncp_client = ncp_client_create();
	if (_ncp_client != NULL)
	{

		/* Connect to the client. returns <=0 for error or port if successful*/
		ret = ncp_client_connect(_ncp_client, _host.c_str(), _port, 0, -1);
		//Start the mission as disconnection will have lost it
		if(ret > 0)
			startMission();
	}
	return ret > 0 ? true : false;
}

bool node::isConnected()
{
	if (_ncp_client == NULL)
		return false;
	if (!ncp_client_is_connected(_ncp_client))
	{
		_ncp_client = NULL;
		return false;
	}
	return true;
}

void node::disconnect()
{
	if (!isConnected())
	{
		ncp_client_disconnect(_ncp_client);
		ncp_client_free(_ncp_client);
		_ncp_client = NULL;
	}
}

void node::receive_packet()
{
	if (!isConnected())
		connect();
	if (isConnected())
	{

		int packet_found = 0;
		while (packet_found == 0)
		{
			ncp_client_receive(_ncp_client);
			if (packet_get_packet_type(ncp_client_get_rx_packet(_ncp_client)) != PACKET_TYPE_LINK)
				packet_found = 1;
		}
	}
}

void node::send_packet(T_PACKET *packet)
{
	if (!isConnected())
		connect();
	if(isConnected())
		ncp_client_send(_ncp_client, packet);
}


T_PACKET* node::get_rx_packet()
{
	if (isConnected())
		return ncp_client_get_rx_packet(_ncp_client);
	else
		return NULL;
}

void node::ackWait()
{

	//If we've lost the connection return
	if (!isConnected())
		return;
	//Wait for an ACK

	for (int i = 0; i < 100; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		T_PACKET* rxpacket = ncp_client_get_rx_packet(_ncp_client);
		receive_packet();

		packet_read(rxpacket);
		/* Loop through all the fields */
		int fname, fid;
		while (packet_get_next_field(rxpacket, &fname, &fid) == 1)
		{

			//printf("Field Name: %s, Id: %d\n", packet_key_to_str(fname, NULL), fid);
			int pname, plength;
			T_PARAM_DATA_TYPE ptype;
			void* pdata;
			while (packet_get_next_param(rxpacket, &pname, &ptype, &pdata, &plength) == 1)
			{
				if (std::string(packet_key_to_str(pname, NULL)) == "ACKN");
				{
					//packet_free(rxpacket);
					return;
				}
			}
		}
		//packet_free(rxpacket);
	}
}

void node::startMission()
{
	//Build a packet to start time capture loops
	T_PACKET* txpacket = packet_create("", 8192);
	packet_write(txpacket, PACKET_TYPE_DSP_LOOP, _field_id++);
	/* Add the time field */
	packet_add_field(txpacket, FIELD_TIME, _field_id++);
	/* Add the parameters */
	int32_t frequency_MHz = static_cast<uint32_t>(_frequency_Hz / 1000000);
	int32_t dds_Hz = static_cast<uint32_t>(_frequency_Hz % 1000000);
	//We need to ensure that the DDS is well away from passband to avoid LO breakthrough
	frequency_MHz -= 9;
	dds_Hz += 9000000;
	packet_add_param_int(txpacket, TIME_CENTER_FREQ_MHZ, frequency_MHz);
	packet_add_param_int(txpacket, TIME_DDS_OFFSET_HZ, dds_Hz);
	//packet_add_param_int(txpacket, ANY_INPUT_NODE, 1);					//Select antenna 1         
	packet_add_param_int(txpacket, TIME_QUALITY_FAST_TUNE, 1);				//Fast capture

	//REPLACE with DECIMATION for marvell/axis
	if (_nexusNode)
	{
		uint32_t bandwidth_MHz = static_cast<uint32_t>(_bandwidth_Hz / 1000000);
		uint32_t bandwidth_mHz = static_cast<uint32_t>(1000 * (_bandwidth_Hz - (bandwidth_MHz * 1000000)));
		packet_add_param_int(txpacket, PKEY_CONST('R', 'B', 'M', 'E'), bandwidth_MHz);		//Span MHz
		packet_add_param_int(txpacket, PKEY_CONST('R', 'B', 'M', 'I'), bandwidth_mHz);		//Span mHz
		uint32_t seconds = _samples / _bandwidth_Hz;
		uint32_t nanoseconds = _samples % _bandwidth_Hz;
		nanoseconds *= 1000000000 / _bandwidth_Hz;
		packet_add_param_int(txpacket, PKEY_CONST('L', 'S', 'E', 'C'), seconds);			//Seconds
		packet_add_param_int(txpacket, PKEY_CONST('L', 'N', 'A', 'N'), nanoseconds);		//Nanoseconds
		//std::cout << "NEXUS bandwidth_MHz: " << bandwidth_MHz << " bandwidth_mHz: " << bandwidth_mHz << std::endl;
	}
	else
	{
		packet_add_param_int(txpacket, TIME_NUM_SAMPLES, _samples);		//Capture a few more than we need to flush digital section
		uint32_t time_decimation = round(40000000.0 / _bandwidth_Hz);
		packet_add_param_int(txpacket, TIME_DECIMATION, time_decimation);
		//std::cout << "MARVELL decimation: " << time_decimation << std::endl;
	}
	uint32_t unix = _measureInterval_ms / 1000;
	uint32_t nano = 1000000 * (_measureInterval_ms % 1000);
	packet_add_param_int(txpacket, TIME_TRIG_REPEAT_UNIX, unix);          //Repeat time
	packet_add_param_int(txpacket, TIME_TRIG_REPEAT_NANO, nano);	

	packet_add_param_int(txpacket, TIME_TRIG_MODE_ABS_TIME, 1);
	packet_write_complete(txpacket);
	send_packet(txpacket);
	ackWait();
	packet_free(txpacket);

	T_PACKET* txpacket2 = packet_create("", 8192);
	packet_write(txpacket2, PACKET_TYPE_NODE, _field_id++);
	packet_add_field(txpacket2, FIELD_CONFIGURE, _field_id++);
	packet_add_param_int(txpacket2, CONFIGURE_GPS_ALL, 1);
	packet_write_complete(txpacket2);
	send_packet(txpacket2);
	ackWait();
	packet_free(txpacket2);
}

void node::setParams(Json::Value &config)
{
	std::string s = config.toStyledString();
	std::cout << "node::setParams " << s << std::endl;
	_host = config.get("host", _host).asString();
	_port = config.get("port", _port).asInt();
	_nexusNode = config.get("nexusNode", _nexusNode).asBool();
	_bandwidth_Hz = config.get("bandwidth_Hz", _bandwidth_Hz).asUInt64();
	_frequency_Hz = config.get("frequency_Hz", _frequency_Hz).asUInt64();
	_samples = config.get("samples", _samples).asUInt();
	_measureInterval_ms = config.get("measureInterval_ms", _measureInterval_ms).asInt();
	_testMode = config.get("testMode", _testMode).asBool();
	_minSampleRate = config.get("minSampleRate", _minSampleRate).asDouble();
	_loc.setSpherical(config.get("lat", _loc.getLat()).asDouble(), config.get("lon", _loc.getLon()).asDouble(), config.get("alt", _loc.getAlt()).asDouble());
	if (config.isMember("transmitter"))
	{
		Json::Value tx = config["transmitter"];
		_transmitter.setSpherical(tx.get("lat", 0).asDouble(), tx.get("lon", 0).asDouble(), tx.get("alt", 0).asDouble());
	}
};

// Given a spectrum, zero pad so that after FFT inversion the signal is resampled by a factor of 2^N
void node::UpSampleSpectrum(uint32_t interpolation, TSignal &spectrum)
{
	int shift = static_cast<int>(spectrum.size() / 2);
	TSignal temp = spectrum.cshift(-shift);
	spectrum.resize(interpolation * spectrum.size());
	std::copy(begin(temp), end(temp), begin(spectrum));
	spectrum = spectrum.cshift(shift);
	spectrum[shift] /= 2.0;
	spectrum[spectrum.size() - shift] = spectrum[shift];
	spectrum *= interpolation;
}

void node::run()
{
	try
	{
		connect();
		//In this thread we need to receive IQ time captures from our connected node,
		//enqueue the data for subscribing nodes to consume. 
		while (!_terminate)
		{
			capture *packet = NULL;
			if (_testMode)
				packet = getPacketData();
			else
			{

				T_PACKET *ncp_packet;
				ncp_packet = get_rx_packet();
				receive_packet();
				packet = getPacketData(ncp_packet);
			}
			if (packet->iqData.size())
			{
				//std::cout << packet->time << " count " << packet.use_count() << std::endl;
				//We'll need the spectrum for correlation. Do it once before distribution.
				_fourier.transform(packet->iqData);
				//Interpolate to improve resolution of the result. We aim for at least 100ns
				//resolution so effective sample rate needs to be >10MHz. The resolution of the 
				//measured correlation peak will affect the best achievable rms error as a
				//perfect solution is unlikely to be found if the time offsets are even slightly off.
				double sampleRate = (1e6 * packet->_srtt) / packet->_deci;
				uint32_t interpolation = 1;
				while (sampleRate < _minSampleRate)
				{
					interpolation *= 2;
					sampleRate *= 2;
				}
				UpSampleSpectrum(interpolation, packet->iqData);
				packet->_srtt *= interpolation;
				_result->push(packet);

				//std::lock_guard<std::mutex> lk(cout_mtx);
				//std::cout << _host << ":" << _port << " pushed " << packet->_time << " power: " << packet->power << std::endl;
			}
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
}



