#pragma once

#include <list>
#include <stdexcept>

#include "Log.hpp"
#include "Task.hpp"

#include "Math/Constants.hpp"

namespace cge {
	class Engine: public TaskSource {
	public:
		class InitFailedException;
		typedef std::list<Task*> TaskList;

		Engine();
		virtual ~Engine();

		void   start();
		void   die( std::string reason = "" );

		void   setFpsLimit( double maxFps );
		Scalar getFpsLimit()   const;
		Scalar getFrameDelta() const;
		Scalar getFps()        const;
		bool   isRunning()     const;

		Log&   getLog();

		class  InitFailedException: public std::runtime_error {
		public:
			InitFailedException();
		};

		static unsigned getMiliseconds();
		static void     waitMiliseconds( unsigned msec );
		static void     waitMiliseconds( double   msec );
		
	private:
		bool     initd_;
		Scalar   dt_;
		Scalar   maxfps_;
		Scalar   fps_;
		bool     running_;
		Log      log_;
	};
}