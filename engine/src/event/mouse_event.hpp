#ifndef HYPER_MOUSE_EVENT_HPP
#define HYPER_MOUSE_EVENT_HPP

#include <event/event.hpp>


namespace hyp {
	class HYPER_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(const float& x, const float& y) : x(x), y(y) {

		};

		float getX() const { return x; }
		float getY() const { return y; }

		EVENT_CLASS_TYPE(hyp::EventType::MouseMoved);
	private:
		float x, y;
	};

	class HYPER_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(const float& xOffset, const float& yOffset): x(xOffset), y(yOffset)  {}

		float getXOffset() const { return x; }
		float getYOffset() const { return y; }

		EVENT_CLASS_TYPE(hyp::EventType::MouseScroll);
	private:
		float x, y;
	};
}

#endif // !HYPER_MOUSE_EVENT_HPP
