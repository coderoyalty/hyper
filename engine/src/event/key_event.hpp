#ifndef HYP_KEY_EVENT_HPP
#define HYP_KEY_EVENT_HPP

#include <event/event.hpp>
#include <io/key_code.hpp>

namespace hyp {
	class HYPER_API KeyEvent : public hyp::Event {
	public:
		hyp::Key getkey() const { return m_key; }

		EVENT_CLASS_CATEGORY(EventCategory::InputCategory | EventCategory::KeyboardCategory);

	protected:
		KeyEvent(const KeyCode key) : m_key((Key)key) {};
		Key m_key;
	};

	class KeyPressedEvent : public hyp::KeyEvent {
	public:
		KeyPressedEvent(const KeyCode key, bool isRepeated) : KeyEvent(key), m_repeated(isRepeated) {}

		bool isRepeat() const {
			return m_repeated;
		}

		EVENT_CLASS_TYPE(hyp::EventType::KeyPressed);

	private:
		bool m_repeated;
	};

	class KeyReleasedEvent : public hyp::KeyEvent {
	public:
		KeyReleasedEvent(const KeyCode key) : KeyEvent(key) {}
		EVENT_CLASS_TYPE(hyp::EventType::KeyReleased);
	};
}

#endif // !HYP_KEY_EVENT_HPP
