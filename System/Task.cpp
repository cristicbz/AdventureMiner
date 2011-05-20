#include "Task.hpp"
#include "Engine.hpp"

namespace cge {
	TaskSource::TaskSource( Engine& engine ): e_(engine), l_(engine.getLog()) {}
}