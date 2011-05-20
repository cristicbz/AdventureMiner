#pragma once

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/make_shared.hpp>
#include <boost/unordered_map.hpp>
#include <memory>
#include <map>

#include "Asset.hpp"
#include "Progress.hpp"


namespace cge {
	class AssetContentFactory;
	class Engine;
	class Log;

	typedef boost::shared_ptr<AssetContentFactory>
		AssetContentFactoryPtr;

	class AssetManager: public boost::noncopyable {
	public:
		explicit AssetManager( Engine &e );
		~AssetManager();

		void precache( AssetPath path );
		void finalize();
		void clearCache();
		void cleanup();

		bool            isLoading() const;
		const Progress& getProgress() const;
		
		void  addContentFactory( AssetPath prefix,
                                 AssetContentFactoryPtr factory );

		template<class ContentType>
		void addContentFactory( AssetPath prefix );
		
		template<class AssetType>
		AssetType getAsset( AssetPath path );
		
		template< class PathList >
		void precacheMany( PathList& pathList );


		friend class AssetCache;
	private:
		void          precache_( AssetPath path );
		AssetCachePtr getCache_( AssetPath path );

		class Loader;
		friend class Loader;

		typedef boost::weak_ptr<AssetCache>
			WeakCache;
		
		typedef boost::unordered_map<AssetPath,WeakCache>
			WeakMap;
		
		typedef boost::unordered_map<AssetPath,AssetCachePtr>
			PrecachedMap;
		
		typedef boost::unordered_map<AssetPath,AssetContentFactoryPtr>
			FactoryMap;
		
		WeakMap               loaded_;
		PrecachedMap          cached_;
		FactoryMap            facts_;
		Log&                  l_;
		Progress              prog_;
		std::auto_ptr<Loader> loader_;
	};

	class AssetContentFactory {
	public:
		virtual ~AssetContentFactory() {}
		virtual AssetContentPtr build() = 0;
	};

	struct UnknownAssetTypeException: public std::runtime_error {
		UnknownAssetTypeException( AssetPath path ): runtime_error("Unknown asset type.") {}
	};

	struct InexistentAssetPathException: public std::runtime_error {
		InexistentAssetPathException( AssetPath path ): runtime_error("Inexistent asset.") {}
	};

	template< class ContentType >
	class SimpleAssetContentFactory: public AssetContentFactory {
	public:
		virtual AssetContentPtr build() {
			return boost::make_shared<ContentType>();
		}
	};

	template<class ContentType>
	void AssetManager::addContentFactory( AssetPath prefix ) {
		addContentFactory(
			prefix,
			boost::make_shared< SimpleAssetContentFactory<ContentType> >()
		);
	}

	template<class AssetType>
	AssetType AssetManager::getAsset( AssetPath path ) {
		return AssetType( getCache_(path) );
	}
		
	template< class PathList >
	void AssetManager::precacheMany( PathList& pathList ) {
		for(auto i = pathList.begin(); i != pathList.end(); ++i)
			precache(*i);
	}
}