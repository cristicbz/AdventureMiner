#include "Progress.hpp"

#define LOCK(i) boost::unique_lock<boost::mutex> l ## i(m_)
#define LOCK0 LOCK(0)

namespace cge {

	Progress::Progress()
		: w_(0), p_(0) {}

	Progress::Progress( Work work )
		: w_(work), p_(0) {}

	void Progress::addWork( Work work ) {
		LOCK0;
		w_+=work;
	}

	void Progress::makeProgress( Work progress ) {
		LOCK0;
		p_+=progress;
		if( p_ > w_ )
			p_ = w_;
	}

	bool Progress::isWorking() const {
		LOCK0;
		return p_<w_;
	}

	Progress::Work Progress::getWork() const {
		LOCK0;
		return w_;
	}

	Progress::Work Progress::getWorkLeft() const {
		LOCK0;
		return w_-p_;
	}

	Progress::Work Progress::getProgress() const {
		LOCK0;
		return p_;
	}
	
	double Progress::getPercentage() const {
		LOCK0;
		return static_cast<double>(p_)/w_ * 100.0;
	}

	void Progress::reset( Work work ) {
		LOCK0;
		w_ = work;
		p_ = 0;
	}

	void Progress::finish() {
		LOCK0;
		p_ = w_;
	}
}