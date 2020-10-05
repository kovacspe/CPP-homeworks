/*
	du1.hpp - timetable data structures
*/

#ifndef du1_hpp_
#define du1_hpp_

#include <cstdint>
#include <string>
#include <set>
#include <map>
#include <vector>

//classes

class Stop;
class Platform;
class LineOnPlatform;
class LineDeparture;
class Departure;
class Trip;
class timetable;


using packed_time = std::int_least16_t;

inline packed_time pack_time(int hh, int mm)
{
	return 60 * hh + mm;
}

inline int hours(packed_time tm)
{
	return tm / 60;
}

inline int minutes(packed_time tm)
{
	return tm % 60;
}

// dummy - for demonstration purposes only
struct dummy_object {};
using dummy_container = std::set< dummy_object>;

// containers - replace with appropriate types of containers
using stop_container = std::map<std::string,Stop>;
using platform_container = std::map<std::string,Platform>;
using platform_route_container = std::map<std::string,LineOnPlatform>;
using route_departure_container = std::map<packed_time,LineDeparture>;
using trip_container = std::map<std::size_t, Trip>;
using trip_departure_container = std::map<size_t,Departure>;




// public attribute types - no change required
using platform_name_const_reference = const std::string &;
using stop_name_const_reference = const std::string &;
using trip_name_const_reference = std::size_t;
using route_name_const_reference = const std::string &;
using seq_id = size_t;

// public container references - no change required
using stops_const_reference = const stop_container &;
using platforms_const_reference = const platform_container &;
using platform_routes_const_reference = const platform_route_container &;
using route_departures_const_reference = const route_departure_container &;
using trips_const_reference = const trip_container &;
using trip_departures_const_reference = const trip_departure_container &;

// public iterator references - no change required
using trip_departure_const_iterator = trip_departure_container::const_iterator;
using route_departure_const_iterator = route_departure_container::const_iterator;

// public object references - no change required
using timetable_const_reference = const timetable &;
using stop_const_reference = stop_container::const_reference;
using platform_const_reference = platform_container::const_reference;
using platform_route_const_reference = platform_route_container::const_reference;
using route_departure_const_reference = route_departure_container::const_reference;
using trip_const_reference = trip_container::const_reference;
using trip_departure_const_reference = trip_departure_container::const_reference;

// main object - put something inside


class Stop
{
public:
	platform_const_reference getPlatform(const std::string & name) const;
	void addPlatform(platform_const_reference platform);
	platforms_const_reference getPlatforms() const;
	platform_container platforms_;
};

class Platform
{
public:
	platform_routes_const_reference getRoutes() const;
	platform_route_container routes_;
};

class LineOnPlatform
{

public:
	route_departure_container departures_;
};

class LineDeparture
{
public:
	LineDeparture() {}
	LineDeparture(std::pair<const std::size_t, Trip> * trip, trip_departure_const_iterator departureIterator) : trip{ trip }, departureIterator{ departureIterator } {};
	trip_departure_const_reference getDeparture() const;
	
	std::pair<const std::size_t,Trip> * trip;
	trip_departure_const_iterator departureIterator;
};

class Departure
{
public:
	Departure() = default;
	Departure(packed_time time) : time{ time }, stop{} {}
	Departure(const Departure & dep) :time{ dep.time }, stop{dep.stop} {}
	Departure(Departure && dep) :time{ dep.time }, stop{ dep.stop } {}
	Departure& operator=(const Departure & d) { time = d.time; stop = std::move(d.stop); return *this;}
	Departure& operator=(Departure && d) { time = d.time; stop = std::move(d.stop); return *this; }
	packed_time time;
	std::pair<const std::string, Stop> * stop;
};

class Trip
{
public:

	void addDeparture(Departure & departure);
	trip_departures_const_reference getDepartures() const;
	trip_departure_const_reference getDeparture(trip_departure_const_iterator departureIterator) const;
	seq_id getSeqID(trip_departure_const_iterator departureIterator) const;
	trip_departure_container departures_;
};

class timetable
{

public:
	stop_container stops;
	trip_container trips;

	timetable() :stops(), trips()
	{

	}

	stop_const_reference getStop(std::string name) const
	{
		return * stops.find(name);
	}

	void addStop(stop_const_reference stop);
	void addTrip(trip_const_reference trip);

};



// public accessor functions - implement them
stops_const_reference stops(timetable_const_reference tt);
stop_const_reference get_stop(timetable_const_reference tt, const std::string & name);
trips_const_reference trips(timetable_const_reference tt);

stop_name_const_reference stop_name(stop_const_reference st);
platforms_const_reference platforms(stop_const_reference st);
platform_const_reference get_platform(stop_const_reference st, const std::string & name);

platform_name_const_reference platform_name(platform_const_reference pl);
platform_routes_const_reference routes(platform_const_reference pl);

route_name_const_reference route_name(platform_route_const_reference pr);
route_departures_const_reference departures(platform_route_const_reference pr);
route_departure_const_iterator departure_at(platform_route_const_reference pr, packed_time tm);

packed_time departure_time(route_departure_const_reference rd);
trip_const_reference trip(route_departure_const_reference rd);
trip_departure_const_iterator position_in_trip(route_departure_const_reference rd);

trip_name_const_reference trip_name(trip_const_reference tr);
trip_departures_const_reference departures(trip_const_reference tr);
seq_id sequence_id(trip_const_reference tr, trip_departure_const_iterator tdit);

packed_time departure_time(trip_departure_const_reference td);
stop_const_reference stop(trip_departure_const_reference td);

// public functions - implement them
void read_timetable(timetable & tt, std::istream & ifs);

void print_platform_timetables(std::ostream & oss, const timetable & tt, const std::string & sid, const std::string & pid);

#endif

/**/