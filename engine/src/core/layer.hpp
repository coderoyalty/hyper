#include <event/event.hpp>
#ifndef HYPER_LAYER_HPP
#define HYPER_LAYER_HPP

#include <string>

namespace hyp {
	class Layer {
	public:
		Layer(const std::string& name = "layer") : m_layerName(name) {}

		virtual ~Layer() = default;
		virtual void onEvent(hyp::Event& event) {};
		virtual void onUpdate(float dt) {};

		virtual void onAttach() {}
		virtual void onDetach() {}

		std::string getName() { return m_layerName; }
	private:
		std::string m_layerName;
	};
}

#endif // !HYPER_LAYER_HPP
