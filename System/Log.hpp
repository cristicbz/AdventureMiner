#pragma once

#include <boost/lexical_cast.hpp>

#include <list>
#include <string>
#include <sstream>
#include <iostream>


namespace cge {
	class Engine;

	class Log;
	class LogDevice;
	class StreamLogDevice;
	
	// TODO: Include differenet logging levels.
	class Log {
	public:
		Log();
		virtual ~Log();

		void addDevice   ( LogDevice *logDevice );
		void removeDevice( LogDevice *logDevice );

		void println( std::string line );
		void print( std::string message );

		template<class T>
		Log& operator<<( const T& message );

	private:
		Log( const Log& );
		// TODO: Use vector instead of lists.
		std::list< LogDevice* > devs_;
		std::list< std::string > lines_;
		std::stringstream buff_;
	};

	class LogDevice {
	public:
		virtual ~LogDevice();
		virtual void println( std::string line ) = 0;
	};

	class StreamLogDevice: public LogDevice {
	public:
		StreamLogDevice( std::ostream& ostream );
		StreamLogDevice( std::ostream* ostream );

		~StreamLogDevice();

		void println( std::string line );

	private:
		StreamLogDevice( const StreamLogDevice& );

		std::ostream *os_;
		bool own_;
	};

	template<class T>
	Log& Log::operator<<(const T& message) {
		print( boost::lexical_cast<std::string,T>(message) );
		return *this;
	}
}