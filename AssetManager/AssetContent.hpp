#pragma once

#include "Asset.hpp"
#include <boost/noncopyable.hpp>

namespace cge {
	class AssetContent;

	class CorruptAssetException: public virtual boost::exception {
	public:
		CorruptAssetException( AssetPath assetPath );
	};


	class AssetContent: boost::noncopyable {
	public:
		virtual ~AssetContent();
		
		friend class AssetManager;

	protected:
		AssetContent();

		virtual void asyncLoad( const std::vector<char>& buffer ) = 0;
		virtual void syncFinalize() = 0;

	private:
		void finalize_();

		bool f_;
	};
}