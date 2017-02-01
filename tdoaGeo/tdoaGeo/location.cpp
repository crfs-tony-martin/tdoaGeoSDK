#include "location.h"

std::ostream& operator<< (std::ostream& os, const serializable& s) {
	s.serialize(os);
	return os;
}


location::location(double lat, double lon, double alt)
{
	setSpherical(lat, lon, alt);
}

void location::setAltitude(const double alt)
{
	setSpherical(_lat, _lon, alt);
}

void location::getLatLong(std::valarray<double> &coordinates)
{
	if(coordinates.size() != 2)
		coordinates.resize(2);

	coordinates[0] = _lat;
	coordinates[1] = _lon;
}

void location::getLatLong(std::complex<double> &coordinates)
{
	coordinates = std::complex<double>(_lat, _lon);
}

double location::getLat()
{
	return _lat;
}

double location::getLon()
{
	return _lon;
}

double location::getAlt()
{
	return(_alt);
}

void location::getSpherical(std::valarray<double> &coordinates)
{
	if (coordinates.size() != 3)
		coordinates.resize(3);

	coordinates[0] = _lat;
	coordinates[1] = _lon;
	coordinates[2] = _alt;
}

void location::setSpherical(const std::valarray<double> &coordinates)
{
	double alt = 0;
	if (coordinates.size() > 2)
		setSpherical(coordinates[0], coordinates[1], coordinates[2]);
	else
		setSpherical(coordinates[0], coordinates[1], 0);
}

void location::setSpherical(const double lat, const double lon, const double alt)
{
	_lat = lat;
	_lon = lon;
	_alt = alt;
	latLongAltToRect();
}

void location::getCartesian(std::valarray<double> &coordinates)
{
	//Make sure enough room for x,y & z
	if (coordinates.size() < 3)
		coordinates.resize(3);

	coordinates[0] = _x;
	coordinates[1] = _y;
	coordinates[2] = _z;
}

void location::setCartesian(const std::valarray<double> &coordinates)
{
	double x = coordinates[X];
	double y = coordinates[Y];
	double z;

	if (coordinates.size() > 2)
		setCartesian(coordinates[0], coordinates[1], coordinates[2]);
	else
		setCartesian(coordinates[0], coordinates[1]);

}

void location::setCartesian(const double x, const double y, const double z)
{
	_x = x; _y = y; _z = z;
	latLongAltFromRect();
}

void location::setCartesian(const double x, const double y, bool autoUpdate)
{
	setCartesian(x, y, sqrt(std::abs(pow(EARTH_RADIUS, 2) - pow(x, 2) - pow(y, 2))));
}


//Convert lat, long and altitude coordinates to a point in cartesian space with earth centre at origin
void location::latLongAltToRect()
{
	double phi = 90 - _lat;			//lat
	double theta = _lon;			//long
	double radius = _alt + EARTH_RADIUS;		//Altitude

	_x = radius * sin(DEG_TO_RAD(phi)) * cos(DEG_TO_RAD(theta));
	_y = radius * sin(DEG_TO_RAD(phi)) * sin(DEG_TO_RAD(theta));
	_z = radius * cos(DEG_TO_RAD(phi));
}

//Convert cartesian coordinates of a point in space realive to earth centre to lat/long/alt
void location::latLongAltFromRect()
{
	double radius = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
	double theta = RAD_TO_DEG(atan2(_y, _x));
	double phi = RAD_TO_DEG(atan2(sqrt(pow(_x, 2) + pow(_y, 2)), _z));

	_lon = theta;
	_lat = 90 - phi;
	_alt = radius - EARTH_RADIUS;
}

double location::distance(const location &loc2)
{
	return sqrt(pow(_x - loc2._x, 2.0) + pow(_y - loc2._y, 2.0) + pow(_z - loc2._z, 2.0));
}

//Shift the location along the bearing (deg) by distance (m) 
void location::move(double distance, double bearing)
{
	//std::cout << "moving " << _lat << ", " << _lon << " by " << distance << "m " << bearing << "deg";
	distance = distance / EARTH_RADIUS;
	bearing = DEG_TO_RAD(bearing);
	double lat1 = DEG_TO_RAD(_lat);
	double lng1 = DEG_TO_RAD(_lon);

	_lat = asin((sin(lat1) * cos(distance)) + (cos(lat1) * sin(distance) * cos(bearing)));
	_lon = lng1 + atan2(sin(bearing) * sin(distance) * cos(lat1), cos(distance) - (sin(lat1) * sin(_lat)));

	_lat = RAD_TO_DEG(_lat);
	_lon = RAD_TO_DEG(_lon);
	//std::cout << " = " << _lat << ", " << _lon << std::endl;
	latLongAltToRect();
}

//Great circle distance between two points on the earth
double location::circleDistance(const location &loc2)
{
	double lat1 = DEG_TO_RAD(_lat);
	double lon1 = DEG_TO_RAD(_lon);
	double lat2 = DEG_TO_RAD(loc2._lat);
	double lon2 = DEG_TO_RAD(loc2._lon);

	double latitudeSqrd = pow(sin((lat1 - lat2) / 2), 2);
	double longitudeSqrd = pow(sin((lon1 - lon2) / 2), 2);
	double squareRoot = sqrt(latitudeSqrd + (cos(lat1) * cos(lat2) * longitudeSqrd));
	double distance = EARTH_RADIUS * (2 * asin(squareRoot));
	double altitudeDelta = loc2._alt - _alt;
	return sqrt((distance * distance) + (altitudeDelta * altitudeDelta));
}
