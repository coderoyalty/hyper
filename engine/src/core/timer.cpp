#include "timer.hpp"
#include <chrono>
#include <utils/logger.hpp>

namespace hyp {

	namespace {

		using namespace std;

		// FPS
		float min_fps = 30.f;
		float max_fps = 600.f;
		float m_fps = min_fps;

		//
		float delta_time = 0.f;

		chrono::steady_clock::time_point last_tick;

		float clamp(float value, float min, float max) {
			return (value < min) ? min : (value > max) ? max :
			                                             value;
		}

	};

	float Timer::getDeltaTime() {
		return delta_time / 1000.f;
	}

	float Timer::getDeltaTimeMs() {
		return delta_time;
	}

	void hyp::Timer::postTick() {
		if (last_tick.time_since_epoch() == chrono::steady_clock::duration::zero())
		{
			last_tick = chrono::steady_clock::now();
			return;
		}

		delta_time = static_cast<float>(chrono::duration<float, milli>(chrono::steady_clock::now() - last_tick).count());

		double target_limit = 1000.f / m_fps;

		while (delta_time < target_limit)
		{
			delta_time = static_cast<float>(chrono::duration<float, milli>(chrono::steady_clock::now() - last_tick).count());
		}

		last_tick = chrono::steady_clock::now();
	}

	void Timer::setFPS(unsigned int fps) {
		if ((float)fps < 0.f)
		{
			fps = min_fps;
			return;
		}

		fps = clamp(fps, min_fps, max_fps);

		if (fps == m_fps) return;

		m_fps = fps;
		HYP_INFO("Set to %.2f FPS", m_fps);
	}

	unsigned int Timer::getFPS() {
		return (unsigned int)m_fps;
	}

}
