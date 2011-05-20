#pragma once

#include <boost/shared_ptr.hpp>
#include "AssetCache.hpp"

namespace cge {
	template< class T >
	class Asset {
	public:
		Asset( Asset&& asset )
			: c_(asset.c_) {}

		Asset& operator=( Asset&& asset ) {
			p_ = asset.p_;
			return *this;
		}

		T* operator->() {
			return dynamic_cast<T*>( &c_->getContent() );
		}

		T& operator*() {
			return *operator->();
		}

		bool operator==( const Asset &asset ) {
			return p_ == asset.p_;
		}
	
		friend class AssetManager;
	private:
		Asset( AssetCachePtr cache )
			: c_(cache) {}

		AssetCachePtr c_;
	};

}