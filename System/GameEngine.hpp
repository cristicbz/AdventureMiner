#pragma once

#include "Engine.hpp"

#include <string>
#include <vector>

namespace cge {
	class Window;
	class Input;
	class Renderer;
	class Audio;
	class AssetManager;
	class Config;
	class Log;
	class Connector;

	class GameEngine: public Engine {
	public:
		GameEngine( int argc, char *argv[] );
		virtual ~GameEngine();

		Window&       getWindow();
		Input&        getInput();
		Renderer&     getRenderer();
		Audio&        getAudio();
		AssetManager& getResourcer();
		Config&       getConfig();
		
	};
}