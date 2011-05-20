#include "AssetCache.hpp"
#include "Util/Debug.hpp"
#include "AssetManager.hpp"

namespace cge {
	AssetCache::AssetCache( AssetManager& m,
	                        AssetPath p,
	                        AssetPromisePtr pr )
		: m_(m), p_(p), c_(), f_(pr->get_future()) {}

	AssetCache::~AssetCache() {
		m_.loaded_[p_].reset();
	}

	AssetContent& AssetCache::getContent() {
		if(!c_) {
			c_ = f_.get();
			cgeAssert(c_);
		}
		return *c_;
	}
}