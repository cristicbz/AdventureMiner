#pragma once

#include "BaseSource.hpp"

namespace cge {
	template< class Derived >
	class BaseListener {
	public:
		BaseListener(): s_(0), dead_(false) {}
		virtual ~BaseListener() {
			if( s_ ) {
				s_->removeListener(static_cast<Derived&>(*this));
				*it_ = 0;
			}
		}
		
		friend class BaseSource<Derived>;

	protected:
		bool                 hasSource() const { return s_ != 0; }
		BaseSource<Derived>& getSource() const { return *s_; }

	private:
		BaseListener( const BaseListener& );

		BaseSource<Derived>*                    s_;
		typename std::list<Derived*>::iterator  it_;
		bool                                    dead_;
	};
}