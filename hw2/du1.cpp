#include "du1.hpp"
#include <map>
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>


	


	platform_const_reference Stop::getPlatform(const std::string & name) const
	{
		return *platforms_.find(name);
	}


	platforms_const_reference Stop::getPlatforms() const
	{
		return platforms_;
	}

	void Stop::addPlatform(platform_const_reference platform)
	{
		if (platforms_.find(platform.first) == platforms_.end())
			platforms_.insert(platform);
	}

	/////////////////////////




	platform_routes_const_reference Platform::getRoutes() const
	{
		return routes_;
	}


	trip_departure_const_reference LineDeparture::getDeparture() const
	{
		return 	(*trip).second.getDeparture(departureIterator);
	}


	void Trip::addDeparture(Departure & departure)
	{
		//departures_.push_back(departure);
	}
	trip_departures_const_reference Trip::getDepartures() const
	{
		return departures_;
	}
	trip_departure_const_reference Trip::getDeparture(trip_departure_const_iterator departureIterator) const
	{
		return (*departureIterator);
	}
	seq_id Trip::getSeqID(trip_departure_const_iterator departureIterator) const
	{
		return (*departureIterator).first;
	}

	void timetable::addStop(stop_const_reference stop)
	{
		if (stops.find(stop.first) == stops.end())
			stops.insert(stop);
	}
	void timetable::addTrip(trip_const_reference trip)
	{
		if (trips.find(trip.first) == trips.end())
			trips.insert(trip);
	}


stops_const_reference stops(timetable_const_reference tt)
{
	return tt.stops;
}
stop_const_reference get_stop(timetable_const_reference tt, const std::string & name) 
{
	return tt.getStop(name);
}
trips_const_reference trips(timetable_const_reference tt)
{
	return tt.trips;
}

/////////////////////////////////////////
stop_name_const_reference stop_name(stop_const_reference st) 
{
	return st.first;
}
platforms_const_reference platforms(stop_const_reference st) 
{
	return st.second.getPlatforms();
}


platform_const_reference get_platform(stop_const_reference st, const std::string & name) 
{
	return st.second.getPlatform(name);
}
/////////////////////////////////////////////
platform_name_const_reference platform_name(platform_const_reference pl)
{
	return pl.first;
}
platform_routes_const_reference routes(platform_const_reference pl) 
{
	return pl.second.getRoutes();
}

///////////////////////////////////////////

route_name_const_reference route_name(platform_route_const_reference pr)
{
	return pr.first;
}
route_departures_const_reference departures(platform_route_const_reference pr)
{
	return pr.second.departures_;
}
route_departure_const_iterator departure_at(platform_route_const_reference pr, packed_time tm) 
{
	//std::cout << "Iterating throght map" << std::endl;
	for(auto i=pr.second.departures_.begin(); i != pr.second.departures_.end();i++)
	{

		//std::cout << (*i).first << std::endl;
		if ((*i).first >= tm) return i;
	}
	return pr.second.departures_.end();
}
////////////////////////////////////
packed_time departure_time(route_departure_const_reference rd)
{
	return rd.first;
}
trip_const_reference trip(route_departure_const_reference rd)
{
	return *rd.second.trip;
}
trip_departure_const_iterator position_in_trip(route_departure_const_reference rd)
{
	return rd.second.departureIterator;
}

////////////////////////////////

trip_name_const_reference trip_name(trip_const_reference tr)
{
	return tr.first;
}


trip_departures_const_reference departures(trip_const_reference tr)
{
	return tr.second.getDepartures();
}


seq_id sequence_id(trip_const_reference tr, trip_departure_const_iterator tdit)
{
	return (*tdit).first;
}

/////////////////////////////
packed_time departure_time(trip_departure_const_reference td)
{
	return td.second.time;
}
stop_const_reference stop(trip_departure_const_reference td)
{
	return *(td.second.stop);
}


struct colon_is_space : std::ctype<char> {
	colon_is_space() : std::ctype<char>(get_table()) {}
	static mask const* get_table()
	{
		static mask rc[table_size];
		rc['\t'] = std::ctype_base::space;
		rc['\n'] = std::ctype_base::space;
		return &rc[0];
	}
};

void read_timetable(timetable & tt, std::istream & ifs)
{
	std::string route_id,stop_id,stop_name,dep_time;
	size_t trip_id, stop_seq;
	packed_time time;
	ifs.imbue(std::locale(ifs.getloc(), new colon_is_space));
	std::getline(ifs, route_id);
	while (!ifs.eof())
	{

		ifs >> route_id >> trip_id >> stop_seq >> stop_id >> stop_name >> dep_time;
		int hh = std::stoi(dep_time.substr(0, 2));
		int mm = std::stoi(dep_time.substr(3, 2));
		time = pack_time(hh, mm);
		Departure dep(time);
		tt.trips[trip_id].departures_[stop_seq] = dep;

	}
	ifs.clear();
	ifs.seekg(0, ifs.beg);
	std::getline(ifs, route_id);
	while (!ifs.eof())
	{
		ifs >> route_id >> trip_id >> stop_seq >> stop_id >> stop_name >> dep_time;
		int hh = std::stoi(dep_time.substr(0, 2));
		int mm = std::stoi(dep_time.substr(3, 2));
		time = pack_time(hh, mm);
		
		LineDeparture ldep(&*tt.trips.find(trip_id),tt.trips[trip_id].departures_.find(stop_seq));
		
		tt.stops[stop_name].platforms_[stop_id].routes_[route_id].departures_[time] = ldep;
		tt.trips[trip_id].departures_[stop_seq].stop = &*tt.stops.find(stop_name);
	}
	
	
	



}
void print_line_stops(std::ostream & oss, const timetable & tt,trip_const_reference trip )
{
	for (auto && departure : trip.second.departures_)
	{
		oss << (*departure.second.stop).first << std::endl;
	}
}

void print_line_departures(std::ostream & oss, const timetable & tt, platform_route_const_reference line)
{
	route_departure_const_iterator it = line.second.departures_.begin();
	for (size_t i = 0;i < 24;++i)
	{
		oss << std::setw(2) << std::setfill('0') << i << ":";
		while (it!=line.second.departures_.end() && (*it).first >= pack_time(i, 00) && (*it).first <= pack_time(i, 59))
		{
			oss<<" " << std::setw(2) << std::setfill('0') << minutes((*it).first);
			++it;
		}
		oss << std::endl;
	}
}


void print_route_timetable(std::ostream & oss, const timetable & tt, platform_route_const_reference line)
{
	oss << "*** " <<line.first<< " ***" << std::endl;
	const std::pair<const packed_time, LineDeparture> * tripRef= &*line.second.departures_.begin();
	for (auto && trip : line.second.departures_)
	{
		if ((*trip.second.trip).second.departures_.size() < (*(*tripRef).second.trip).second.departures_.size())
		{
			tripRef = &trip;
		}
	}
	trip_const_reference trip = *(*tripRef).second.trip;
	print_line_stops(oss, tt, trip);
	print_line_departures(oss, tt, line);

}


void print_platform_timetables(std::ostream & oss, const timetable & tt, const std::string & sid, const std::string & pid)
{
	for (auto && line : tt.stops.at(sid).platforms_.at(pid).routes_)
	{
		print_route_timetable(oss,tt, line);
	}
}
