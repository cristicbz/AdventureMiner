#pragma once

#include <list>
#include <algorithm>
#include <functional>
#include <vector>

#include "Debug.hpp"

namespace cge {
	// TODO: Move implementations outside of declaration
	template< class Listener >
	class BaseSource {
	public:
		BaseSource() { dead_.reserve( 4 ); }

		virtual ~BaseSource() {
			removeDead();
			std::for_each(ls_.begin(), ls_.end(),
				[]( Listener *l ) { if( l ) l->s_ = 0; }
			);
		}

	protected:
		bool hasListeners() const { return !ls_.empty(); }

		void addListener ( Listener& l ) {
			if( l.s_ ) {
				if( l.s_ == this ) {
					cgeAssertM( l.dead_, "Duplicate listener addition." );
					l.dead_ = false;
					std::remove(dead_.begin(),dead_.end(),l.it_);
				} else
					cgeAssertM( l.s_ != this, "Adding of a listener belonging to a different source attempted." );
			} else {
				// TODO: Change to push_back.
				ls_.push_front(&l);
				l.it_ = ls_.begin();
				l.s_  = this;
			}
		}

		void removeListener( Listener& l ) {
			cgeAssertM(l.s_, "Unbound listener removal attempted.");
			cgeAssertM(l.s_ == this , "Removal of a listener belonging to a different source attempted.");
			
			if( !l.dead_ ) {
				l.dead_ = true;
				dead_.push_back(l.it_);
			}
		}

		template<class T>
		void inject( T handler ) {
			removeDead();
			std::for_each( ls_.begin(), ls_.end(),
				[&handler]( Listener* l ) { (l->*handler)(); }
			);
		}
		
		template<class T, class Arg1>
		void inject( T handler, Arg1&& arg1 ) {
			removeDead();
			std::for_each( ls_.begin(), ls_.end(),
				[&handler, &arg1]( Listener* l ) { (l->*handler)( arg1 ); }
			);
		}
		
		template<class T, class Arg1, class Arg2>
		void inject( T handler, Arg1&& arg1, Arg2&& arg2 ) {
			removeDead();
			std::for_each( ls_.begin(), ls_.end(),
				[&handler, &arg1, &arg2]( Listener* l ) { (l->*handler)( arg1, arg2 ); }
			);
		}

	private:
		template<class D>
		friend class BaseListener;

		typedef typename std::list<Listener*>::iterator LsIter;

		BaseSource(const BaseSource&);

		void removeDead() {
			if( !dead_.empty() )
				doRemoveDead_();
		}

		void doRemoveDead_() {
			std::for_each( dead_.begin(), dead_.end(),
				[this]( LsIter it ) {
					Listener* l = *it;
					
					if( l ) {
						cgeAssertM( l->dead_, "Undead listener." );
						
						l->s_    = 0;
						l->dead_ = false;
					}

					ls_.erase( it );
				}
			);
			
			dead_.clear();
		}

		// TODO: Use unstable vector instead of list (iterator problem)
		std::list< Listener* >  ls_;
		std::vector< LsIter >   dead_;
	};
}