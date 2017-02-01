#pragma once

#include <cmath>
#include <valarray>
#include <complex>
#include <iostream>

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286
#define EARTH_RADIUS 6371000
#define DEG_TO_RAD(a) (a*PI/180)
#define RAD_TO_DEG(a) (180*a/PI)
#define TO_LAT_LONG(x) (static_cast<double>(x)/1e7)
#define FROM_LAT_LONG(x) (static_cast<int32_t>(x*1e7))
#define SPEED_OF_LIGHT 299792458
#define FREE_SPACE_LOSS(dist_m,freq_Hz) (dist_m*freq_Hz>0?20*(log10(dist_m)+log10(freq_Hz)+log10(4*PI/SPEED_OF_LIGHT))
enum {LAT=0, LON, ALT};
enum {X=0, Y, Z};

class serializable {
public:
	virtual void serialize(std::ostream& os) const = 0;
};

class location : public serializable
{
public:
	virtual void serialize(std::ostream& os) const 
	{
		os << _lat << ", " << _lon << " " << _alt << "m " << _timeDelta << "ns";
	}
	location() {};
	location(double lat, double lon, double alt);
	~location() {};
	bool operator == (location &l) { return l._lat == _lat && l._lon == _lon; };
	bool operator <= (location &l) { return (l == *this || l < *this); };
	bool operator !=  (location &l) { return !(l == *this); };
	bool operator < (location &l) { return (l._lat + l._lon < _lat + _lon); };
	void latLongAltToRect();
	void latLongAltFromRect();
	void getLatLong(std::valarray<double> &coordinates);
	void getLatLong(std::complex<double> &coordinates);
	double getLat();
	double getLon();
	double getAlt();
	void getxy(std::valarray<double> &coordinates);
	void getSpherical(std::valarray<double> &coordinates);
	void getCartesian(std::valarray<double> &coordinates);
	void setSpherical(const double lat, const double lon, const double alt);
	void setCartesian(const double x, const double y, const double z);
	void setCartesian(const double x, const double y, bool autoUpdate = true);
	void setSpherical(const std::valarray<double> &coordinates);
	void setCartesian(const std::valarray<double> &coordinates);
	void setAltitude(double alt);
	double distance(const location &loc2);
	void move(double distance, double bearing);
	double circleDistance(const location &loc2);
	double _error = { 0 };
	//Delay at this location relative to the master
	int32_t _timeDelta = { 0 };
private:
	double _x; double _y; double _z;
	double _lat; double _lon; double _alt;
	//struct { bool spherical = { false }; bool cartesian = { false }; } valid;
};

