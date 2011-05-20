#pragma once

#include <boost/noncopyable.hpp>

namespace cge {
	class NonCopyable {
	protected:
							NonCopyable() {}
							~NonCopyable() {}
	private:
							NonCopyable( const NonCopyable& );
		const NonCopyable&	operator=  ( const NonCopyable& );
	};
}