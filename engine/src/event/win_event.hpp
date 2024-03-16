#ifndef HYPER_WINDOW_EVENT_HPP
#define HYPER_WINDOW_EVENT_HPP


#include <event/event.hpp>

namespace hyp {
	class WindowResizeEvent : public hyp::Event {
	public:
		WindowResizeEvent(uint32_t width, uint32_t height): m_width(width), m_height(height) {
		};

		uint32_t getWidth() const { return m_width; }
		uint32_t getHeight() const { return m_height; }

		std::string toString() const {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_width  << ", " << m_height;

			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
	private:
		uint32_t m_width;
		uint32_t m_height;
	};

	class WindowCloseEvent : public hyp::Event {
	public:
		WindowCloseEvent() {

		}

		EVENT_CLASS_TYPE(WindowClose);
	};
}

#endif // !HYPER_WINDOW_EVENT_HPP
