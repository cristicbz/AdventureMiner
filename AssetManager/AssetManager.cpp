#include "AssetManager.hpp"
#include "Util/Debug.hpp"
#include "System/Engine.hpp"
#include "System/Log.hpp"
#include "AssetContent.hpp"

#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <sstream>
#include <fstream>
#include <queue>
#include <vector>


#define log l_ << "AssetManager: "

namespace cge {
	namespace priv {
		inline std::string getPrefix( AssetPath p ) {
			unsigned s = p.find_last_of("/\\");
			if( s == std::string::npos )
				s = 0;
			else
				s++;
			
			return p.substr(s,3);
		}
	}
	using namespace priv;

	class AssetManager::Loader {
		AssetManager&                 mngr;
		std::auto_ptr<boost::thread>  thread;
		boost::asio::io_service       io;
		boost::asio::io_service::work work;

		void asyncLoad( AssetPath path, AssetPromisePtr pr, AssetContentPtr c, Progress::Work w ) {
			try {
				{
					// TODO: Do this properly...
					// Get the contents of the file
					std::ifstream fs(path.c_str(), std::ios::binary);	
					if( !fs.is_open() )
						BOOST_THROW_EXCEPTION( InexistentAssetPathException(path) );
					fs.seekg(0,std::ios::end);
					std::vector<char> buf((unsigned)fs.tellg() + 1);
					fs.seekg(0,std::ios::beg);
					fs.read(&buf[0],buf.capacity());
					buf.resize((unsigned)fs.gcount());
					fs.close();
					// call the loading method on the AssetContent
					c->asyncLoad(buf);
				}
				
				mngr.prog_.makeProgress(w);

				// after loading is done, set the promise value to the content
				pr->set_value(c);
			} catch( ... ) {
				// transfer exceptions to the main thread
				pr->set_exception(boost::current_exception());
			}
			boost::this_thread::yield();
		}

		
		void asyncRun() {
			io.run();
		}

		bool joinable() { return thread.get() && thread->joinable(); }

	public:
		Loader(AssetManager &m): mngr(m), work(io) {
			thread.reset( new boost::thread(
				boost::bind( &AssetManager::Loader::asyncRun, this )
			));		
		}

		~Loader() {
			// stop & wait for the thread to finish
			io.stop();
			if( joinable() )
				thread->join();
		}

		void syncLoad(AssetPath path, AssetPromisePtr pr, AssetContentPtr c) {
			Progress::Work w = (Progress::Work)boost::filesystem::file_size(path);
			mngr.prog_.addWork( w );
			// add the loading function to the io_service
			io.post( boost::bind(
				&AssetManager::Loader::asyncLoad,
				this, path, pr, c, w
				)
			);
		}
	};

	AssetManager::AssetManager( Engine &e )
	: loader_(), l_(e.getLog()) {
		loader_.reset(new Loader(*this));
	}

	AssetManager::~AssetManager() {

	}

	void AssetManager::precache( AssetPath path ) {
		if( !cached_.count(path) ) {
			auto c = getCache_(path);
			cached_[path] = c;
		}
	}

	bool AssetManager::isLoading() const {
		return prog_.isWorking();
	}

	void AssetManager::addContentFactory(
		std::string prefix,
		AssetContentFactoryPtr assetContentFactory
	) {
		facts_[prefix] = assetContentFactory;
	}

	void AssetManager::finalize() {
		for(auto i=loaded_.begin(); i!=loaded_.end(); ++i) {
			if(!i->second.expired())
				i->second.lock()->getContent().finalize_();
		}
	}

	void AssetManager::clearCache() {
		cached_.clear();
	}

	AssetCachePtr AssetManager::getCache_( AssetPath path ) {
		// see if it is already in loaded (it may have been freed anyway)
		auto lr = loaded_.insert( std::make_pair(path,WeakCache()) );
		
		if( !lr.second ) {
			// if it is in loaded_, return it if it hasn't been freed
			if( auto sh = lr.first->second.lock() )
				return sh;
		}
		// if the asset wasn't already loaded or if it had been freed
		// then load it
		std::string prefix = getPrefix(path);
		cgeTrace( log, prefix );
		auto fiter = facts_.find(prefix);
		if( fiter == facts_.end() )
			BOOST_THROW_EXCEPTION( UnknownAssetTypeException(path) );
		auto cont = fiter->second->build();
		auto prom  = boost::make_shared<AssetPromise>();
		auto cache = boost::make_shared<AssetCache>(
			boost::ref<AssetManager>(*this), path, prom
		);

		// start loading the asset
		//log << "Loading " << path << "...\n";
		loader_->syncLoad(path,prom,cont);

		// set the loaded_ entry to the new cache
		lr.first->second.swap( WeakCache(cache) );

		return cache;
	}

	const Progress& AssetManager::getProgress() const {
		return prog_;
	}

	void AssetManager::cleanup() {
		prog_.reset(0);
		std::queue<WeakMap::iterator> expired;
		for(auto i=loaded_.begin(); i!=loaded_.end(); ++i)
			if( i->second.expired() )
				expired.push(i);

		while( !expired.empty() ) {
			loaded_.erase(expired.front());
			expired.pop();
		}
	}
}