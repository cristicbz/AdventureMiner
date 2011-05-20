#include "AssetContent.hpp"

namespace cge {

	AssetContent::~AssetContent() {

	}

	AssetContent::AssetContent()
		: f_(false) {}

	void AssetContent::finalize_() {
		if( !f_ ) {
			f_ = true;
			syncFinalize();
		}
	}

}