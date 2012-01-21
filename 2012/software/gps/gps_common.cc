#include "gps_common.hpp"

#include <cmath>

//distance in meters
double lambert_distance(const GPSState& a, const GPSState& b)
{
	if ((a.lat == b.lat) && (a.lon == b.lon))
		return 0;
	
	static const double eq_rad = 6378.137e3;
	static const double r = 298.257223563;	

	double alat = a.lat*M_PI/180.0;
	double blat = b.lat*M_PI/180.0;
	double alon = a.lon*M_PI/180.0;
	double blon = b.lon*M_PI/180.0;

	const double psia = atan2( (r - 1.0) * tan(alat) , r );
	const double psib = atan2( (r - 1.0) * tan(blat) , r );

	const double P = (psia + psib) / 2.0;
	const double Q = (psia - psib) / 2.0;

	const double dlon = blon - alon;
	const double sigma = acos( sin(alat)*sin(blat) + cos(alat)*cos(blat) * cos(dlon) );

	const double sP = sin(P);
	const double cP = cos(P);

	const double sQ = sin(Q);
	const double cQ = cos(Q);

	const double ssgimahalf = sin(sigma/2.0);
	const double csgimahalf = cos(sigma/2.0);

	const double X = (sigma - sin(sigma)) * (sP*sP*cQ*cQ) / (csgimahalf*csgimahalf);
	const double Y = (sigma + sin(sigma)) * (cP*cP*sQ*sQ) / (ssgimahalf*ssgimahalf);

	const double d = eq_rad * (sigma - (X+Y) / (2.0*r));
	return d;
}

void lambert_distance_xy(const GPSState& a, const GPSState& b,double* x,double* y)
{
	static const double eq_rad = 6378.137e3;
	static const double r = 298.257223563;	

	double alat = a.lat*M_PI/180.0;
	double blat = b.lat*M_PI/180.0;
	double alon = a.lon*M_PI/180.0;
	double blon = b.lon*M_PI/180.0;

	const double psia = atan2( (r - 1.0) * tan(alat) , r );
	const double psib = atan2( (r - 1.0) * tan(blat) , r );

	const double P = (psia + psib) / 2.0;
	const double Q = (psia - psib) / 2.0;

	const double dlon = blon - alon;
	const double sigma = acos( sin(alat)*sin(blat) + cos(alat)*cos(blat) * cos(dlon) );

	const double sP = sin(P);
	const double cP = cos(P);

	const double sQ = sin(Q);
	const double cQ = cos(Q);

	const double ssgimahalf = sin(sigma/2.0);
	const double csgimahalf = cos(sigma/2.0);

	*x = (sigma - sin(sigma)) * (sP*sP*cQ*cQ) / (csgimahalf*csgimahalf);
	*y = (sigma + sin(sigma)) * (cP*cP*sQ*sQ) / (ssgimahalf*ssgimahalf);

}


double lambert_bearing(const GPSState& a, const GPSState& b)
{
	double alat = a.lat*M_PI/180.0;
	double blat = b.lat*M_PI/180.0;
	double alon = a.lon*M_PI/180.0;
	double blon = b.lon*M_PI/180.0;

	const double dlon = blon - alon;

	const double YY = sin(dlon) * cos(blat);
	const double XX = cos(alat) * sin(blat) - sin(alat) * cos(blat) * cos(dlon);
	
	double bring = atan2(YY, XX) * 180/M_PI;
	if (bring < 0)
	{
	bring = bring + 360;
	}	

	return bring;

}
