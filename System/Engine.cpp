#include "Engine.hpp"
#include "Util/Debug.hpp"
#include "Task.hpp"
#include <SDL/SDL.h>

#define log log_ << "Engine: "

namespace cge {
	Engine::Engine()
	: TaskSource(*this), initd_(false), dt_(0.0), maxfps_(0.0), fps_(0.0), running_(false), log_() {
		bool opOk;
		log << "Initing SDL...\n";
		opOk = SDL_Init(SDL_INIT_VIDEO)==0;

		if( !opOk ) {
			log << "Raising InitFailedException.";
			throw InitFailedException();
		}

		initd_ = true;
		log << "Init successful.\n";
	}

	Engine::~Engine() {
		if(initd_) {
			log << "Destroyed. Calling SDL_Quit().\n";
			SDL_Quit();
		} else
			log << "Destroyed. Init failed, no cleanup required.\n";
	}

	void Engine::start() {
		running_ = true;
		unsigned t0, msecDelta;
		unsigned fpsInterval=0;
		unsigned fpsCount=0;
		dt_ = 1.0f/30.0f;
		while(running_) {
			t0 = getMiliseconds();
			
			injectRunTask();
			if( !hasListeners() )
				running_ = false;

			// compute time delta
			msecDelta = getMiliseconds() - t0;
			dt_ = static_cast<Scalar>(msecDelta) / 1000.0f;

			// compute fps
			fpsInterval += msecDelta;
			fpsCount ++;
			if( fpsInterval >= 5000 ) {
				fps_ = static_cast<Scalar>(fpsCount*1000) / fpsInterval;
				fpsInterval = 0;
				fpsCount = 0;
				log << "FPS: " << fps_ << '\n';
			}
		}
	}

	void Engine::die( std::string reason ) {
		log << "FATAL: " << reason << '\n';
		running_ = false;
	}


	void Engine::setFpsLimit( double maxFps ) {
		cgeStub(log);
	}

	Scalar Engine::getFpsLimit() const {
		return maxfps_;
	}

	Scalar Engine::getFrameDelta() const {
		return dt_;
	}

	Scalar Engine::getFps() const {
		return fps_;
	}

	bool Engine::isRunning() const {
		return running_;
	}

	Log& Engine::getLog() {
		return log_;
	}

	unsigned Engine::getMiliseconds() {
		return SDL_GetTicks();
	}

	void Engine::waitMiliseconds( double ms ) {
		waitMiliseconds( static_cast<unsigned>(ms) );
	}

	void Engine::waitMiliseconds( unsigned ms ) {
		SDL_Delay(ms);
	}

	Engine::InitFailedException::InitFailedException()
		: runtime_error("Engine: SDL_Init failed.") {}
}
