#ifndef HYPER_EVENT_HPP
#define HYPER_EVENT_HPP

#include <system/export.hpp>
#include <functional>
#include <sstream>
#include <string>

namespace hyp {

	enum class HYPER_API EventType
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocused,
		WindowNotFocused,
		WindowMoved,
		KeyPressed,
		KeyReleased,
		MouseBtnPressed,
		MouseBtnReleased,
		MouseMoved,
		MouseScroll
	};

	enum EventCategory
	{
		None = 0,
		WindowCategory,
		InputCategory,
		KeyboardCategory,
		MouseCategory,
		MouseBtnCategory
	};

#define EVENT_CLASS_TYPE(type)                   \
	static EventType getType() { return type; }; \
	EventType getEventType() { return getType(); }

#define EVENT_CLASS_CATEGORY(category) \
	virtual int getCategoryFlags() const override { return category; }

#define BIND_EVENT_FN(function) [this](auto& args) -> decltype(auto) { \
	return this->function(args);                                       \
}

	class HYPER_API Event {
	public:
		virtual ~Event() = default;

		virtual EventType getEventType() {
			return getType();
		};
		static EventType getType() {
			return EventType::None;
		}

		virtual int getCategoryFlags() const = 0;

		bool belongsToCategory(EventCategory category) const {
			return getCategoryFlags() & static_cast<int>(category);
		}

		bool handled = false;
	};

	class HYPER_API EventDispatcher {
	public:
		EventDispatcher(Event& event) : m_event(event) {
		}

		template <typename T, typename F>
		bool dispatch(const F& func) {
			if (m_event.getEventType() == T::getType())
			{
				m_event.handled |= func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};
}

#endif // !HYPER_EVENT_HPP
