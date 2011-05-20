#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace cge {
	class AssetCache;
	class AssetContent;
	class AssetManager;

	typedef boost::shared_ptr<AssetCache>         AssetCachePtr;
	typedef boost::shared_ptr<AssetContent>       AssetContentPtr;
	typedef boost::unique_future<AssetContentPtr> AssetFuture;
	typedef boost::promise<AssetContentPtr>       AssetPromise;
	typedef boost::shared_ptr<AssetPromise>       AssetPromisePtr;
	typedef std::string                           AssetPath;
	
	class AssetCache: boost::noncopyable {
	public:
		AssetCache(
			AssetManager&        manager,
			AssetPath            path,
			AssetPromisePtr      promise );
		
		~AssetCache();

		AssetContent& getContent();

	private:
		AssetManager&      m_;
		AssetPath          p_;
		AssetContentPtr    c_;
		AssetFuture        f_;
	};
}