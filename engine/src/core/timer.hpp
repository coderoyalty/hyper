#pragma once
#ifndef HYP_TIMER_HPP
	#define HYP_TIMER_HPP

namespace hyp {
	class Timer {
	public:
		static void setFPS(unsigned int fps);

		static unsigned int getFPS();

		/**
		* \brief elapsed time since last tick in seconds
		*/
		static float getDeltaTime();
		/**
		* \brief elapsed time since last tick in milliseconds
		*/
		static float getDeltaTimeMs();

	private:
		static void postTick();
		friend class Application;
	};
}

#endif // !HYP_TIMER_HPP
