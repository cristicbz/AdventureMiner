#pragma once

#include <boost/thread.hpp>

namespace cge {
	class Progress {
	public:
		typedef long long Work;
		Progress();
		explicit Progress( Work work );

		void addWork( Work work );
		void makeProgress( Work progress );
		
		bool   isWorking() const;
		Work   getWork() const;
		Work   getWorkLeft() const;
		Work   getProgress() const;
		double getPercentage() const;
		void   finish();

		void reset( Work work );

	private:
		mutable boost::mutex m_;
		Work         w_,p_;
	};
}