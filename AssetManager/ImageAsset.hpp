#pragma once

#include "PixelBuffer.hpp"
#include "AssetContent.hpp"


namespace cge {
	class ImageContent: public PixelBuffer, public AssetContent {
	protected:
		virtual void syncFinalize();
		virtual void asyncLoad( const std::vector<char>& buffer );
	};

	typedef Asset<ImageContent> ImageAsset;
}