#pragma once

#include "Util/BaseSource.hpp"
#include "Util/BaseListener.hpp"
#include "Log.hpp"

namespace cge {
	class Engine;

	class Task: public BaseListener<Task> {
	public:
		Task() {}

		virtual void runTask() = 0;
	};

	class TaskSource: public BaseSource<Task> {
	public:
		TaskSource( Engine& engine );

		void addTask( Task& task )    { addListener(task); }
		void removeTask( Task& task ) { removeListener(task); }
		
		Engine& getEngine() const     { return e_; }
		Log&    getLog()    const     { return l_; }
	
	protected:
		void injectRunTask() { inject( &Task::runTask ); }

	private:
		Engine& e_;
		Log&    l_;
	};

	class TaskGroup: public TaskSource, public Task  {
	public:
		TaskGroup( Engine &engine ): TaskSource(engine) {}

		void runTask() { injectRunTask(); }
	};
}