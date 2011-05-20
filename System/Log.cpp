#include "Log.hpp"
#include "Engine.hpp"
#include "Util/Debug.hpp"

namespace cge {
	Log::Log() {
		println("Log: Log started.");
	}

	Log::~Log() {
		println("Log: Log ends.");
		for(auto i=devs_.begin(); i!=devs_.end(); ++i)
			delete *i;
	}

	void Log::addDevice( LogDevice* device ) {
		devs_.push_back(device);
		for(auto i=lines_.begin(); i!=lines_.end(); ++i)
			device->println(*i);
	}

	void Log::removeDevice( LogDevice* device ) {
		devs_.remove(device);
		delete device;
	}

	void Log::println( std::string line ) {
		lines_.push_back( line );
		for(auto i=devs_.begin(); i!=devs_.end(); ++i)
			(**i).println(line);
	}

	void Log::print( std::string str ) {
		buff_ << str;
		if(str.find('\n') == std::string::npos)
			return;
		
		std::string line;
		while(true) {
			std::getline(buff_,line);
			if(buff_.eof()) break;
			println(line);
		}

		buff_ = std::stringstream(line);
	}

	LogDevice::~LogDevice() {
	
	}

	StreamLogDevice::~StreamLogDevice() {
		cgeTrace(*os_,own_);
		if(own_)
			delete os_;
	}

	StreamLogDevice::StreamLogDevice( std::ostream& os )
		: os_(&os), own_(false) {}

	StreamLogDevice::StreamLogDevice( std::ostream* os )
		: os_(os), own_(true) {}

	void StreamLogDevice::println( std::string line ) {
		(*os_) << line << std::endl;
	}


}