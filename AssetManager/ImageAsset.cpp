#include "ImageAsset.hpp"

namespace cge {
	void ImageContent::syncFinalize() {
	
	}

	void ImageContent::asyncLoad( const std::vector<char>& buffer ) {
		load( buffer );
	}
}