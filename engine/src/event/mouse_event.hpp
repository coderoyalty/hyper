#ifndef HYPER_MOUSE_EVENT_HPP
#define HYPER_MOUSE_EVENT_HPP

#include <event/event.hpp>
#include <io/mouse_code.hpp>

namespace hyp {
	class HYPER_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(const float& x, const float& y) : x(x), y(y) {

		                                                  };

		float getX() const { return x; }
		float getY() const { return y; }

		EVENT_CLASS_TYPE(hyp::EventType::MouseMoved);
		EVENT_CLASS_CATEGORY(hyp::EventCategory::MouseCategory | hyp::EventCategory::InputCategory);

	private:
		float x, y;
	};

	class HYPER_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(const float& xOffset, const float& yOffset) : x(xOffset), y(yOffset) {}

		float getXOffset() const { return x; }
		float getYOffset() const { return y; }

		EVENT_CLASS_TYPE(hyp::EventType::MouseScroll);
		EVENT_CLASS_CATEGORY(hyp::EventCategory::MouseCategory | hyp::EventCategory::InputCategory);

	private:
		float x, y;
	};

	class HYPER_API MouseBtnEvent : public Event {
	public:
		hyp::Mouse getButton() const { return m_button; }
		EVENT_CLASS_CATEGORY(hyp::EventCategory::MouseCategory | hyp::EventCategory::InputCategory | hyp::EventCategory::MouseBtnCategory);

	protected:
		MouseBtnEvent(const hyp::MouseCode mouse) : m_button((hyp::Mouse)mouse) {}
		hyp::Mouse m_button;
	};

	class HYPER_API MouseBtnPressedEvent : public MouseBtnEvent {
	public:
		MouseBtnPressedEvent(const hyp::MouseCode button) : MouseBtnEvent(button) {}

		EVENT_CLASS_TYPE(hyp::EventType::MouseBtnPressed);
	};

	class HYPER_API MouseBtnReleasedEvent : public MouseBtnEvent {
	public:
		MouseBtnReleasedEvent(const hyp::MouseCode button) : MouseBtnEvent(button) {}

		EVENT_CLASS_TYPE(hyp::EventType::MouseBtnReleased);
	};
}

#endif // !HYPER_MOUSE_EVENT_HPP
