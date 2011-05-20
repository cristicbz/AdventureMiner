#pragma once

#include <string>
#include <stdexcept>
#include "VideoMode.hpp"
#include "Task.hpp"

namespace cge {
	class Engine;
	class Log;

	class Window: public TaskGroup {
	public:
		class InitFailedException;
		class VideoModeNotSupportedException;
		class UninitializedCallException;

		Window( Engine& engine );
		virtual ~Window();
		
		void setVideoMode( const VideoMode& mode );
		void setTitle( std::string title );

		void setDecorated( bool decorated );
		void setVSync( bool vsync );
		void setFullscreen( bool fullscreen );
		void setSize(unsigned width, unsigned height);

		unsigned  getWidth() const;
		unsigned  getHeight() const;

		const VideoMode& getVideoMode() const;

		bool     isDecorated() const;
		bool     isVSync() const;
		bool     isFullscreen() const;

		void     swapBuffers() const;

		void     runTask() { TaskGroup::runTask(); swapBuffers(); }

		class VideoModeNotSupportedException: public std::runtime_error {
		public:
			VideoModeNotSupportedException();
		};

	private:
		bool      initd_;
		VideoMode mode_;
		Log&      lg_;
	};
}