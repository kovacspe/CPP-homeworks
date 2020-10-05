/*
dump.hpp - dump data into file
*/

#ifndef dump_hpp_
#define dump_hpp_

#include <ostream>
#include <iomanip>
#include <string>
#include <array>

namespace dump {

	template< typename TT>
	void dump_stopwise(std::ostream & oss, TT && tt)
	{
		static const auto tab = '\t';

		oss << "route_short_name" << tab << "trip_id" << tab << "stop_sequence" << tab << "stop_id" << tab << "stop_name" << tab << "departure_time" << std::endl;

		for (auto && stop : stops(tt))
		{
			auto && stop_name_ = stop_name(stop);

			for (auto && platform : platforms(stop))
			{
				auto && platform_name_ = platform_name(platform);

				for (auto && route : routes(platform))
				{
					auto && route_name_ = route_name(route);

					for (auto && departure : departures(route))
					{
						auto departure_time_ = departure_time(departure);
						auto && departure_trip = trip(departure);
						auto trip_iterator = position_in_trip(departure);

						auto && trip_name_ = trip_name(departure_trip);
						auto stop_sequence_ = sequence_id(departure_trip, trip_iterator);

						oss
							<< route_name_ << tab
							<< trip_name_ << tab
							<< stop_sequence_ << tab
							<< platform_name_ << tab
							<< stop_name_ << tab
							<< std::setw(2) << std::setfill('0') << hours(departure_time_) << ":"
							<< std::setw(2) << std::setfill('0') << minutes(departure_time_) << std::endl;
					}
				}
			}
		}
	}

	template< typename TT>
	void dump_tripwise(std::ostream & oss, TT && tt)
	{
		static const auto tab = '\t';

		oss << "route_short_name" << tab << "trip_id" << tab << "stop_sequence" << tab << "stop_id" << tab << "stop_name" << tab << "departure_time" << std::endl;
			
		for (auto && trip_ : trips(tt))
		{
			auto && trip_name_ = trip_name(trip_);

			for ( auto trip_iterator = departures(trip_).begin(); trip_iterator != departures(trip_).end(); ++trip_iterator)
			{
				auto && departure = *trip_iterator;
				auto departure_time_ = departure_time(departure);
				auto && departure_stop = stop(departure);
				auto && stop_name_ = stop_name(departure_stop);

				auto stop_sequence_ = sequence_id(trip_, trip_iterator);

				std::string platform_name_;
				std::string route_name_;
				
				for (auto && platform : platforms(departure_stop))
				{
					
					for (auto && route : routes(platform))
					{

						for ( auto route_iterator = departure_at(route, departure_time_); 
							route_iterator != departures(route).end()
							&& departure_time( * route_iterator) == departure_time_; 
							++ route_iterator)
						{
							
							if (& trip(*route_iterator) == & trip_
								&& position_in_trip(*route_iterator) == trip_iterator)
							{
								// this is my trip
				
								platform_name_ = platform_name(platform);
								route_name_ = route_name(route);
							}
						}
					}
				}
				
				oss
					<< route_name_ << tab
					<< trip_name_ << tab
					<< stop_sequence_ << tab
					<< platform_name_ << tab
					<< stop_name_ << tab
					<< std::setw(2) << std::setfill('0') << hours(departure_time_) << ":"
					<< std::setw(2) << std::setfill('0') << minutes(departure_time_) << std::endl;
			}
		}
	}
}

#endif
