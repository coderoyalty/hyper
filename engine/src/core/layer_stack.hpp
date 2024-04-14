#ifndef HYPER_LAYER_STACK_HPP
#define HYPER_LAYER_STACK_HPP

#include <utils/assert.hpp>
#include <utils/logger.hpp>
#include <core/layer.hpp>
#include <vector>
namespace hyp {
	class LayerStack {
		using Layers = std::vector<hyp::Layer*>;

	public:
		LayerStack() {
		}

		~LayerStack();

		void pushLayer(hyp::Layer* layer);
		void popLayer(hyp::Layer* layer);

		void pushOverlay(hyp::Layer* overlay);
		void popOverlay(hyp::Layer* overlay);

	public:
		Layers::iterator begin() {
			return m_layers.begin();
		}
		Layers::iterator end() {
			return m_layers.end();
		}

		Layers::const_iterator begin() const {
			return m_layers.begin();
		}
		Layers::const_iterator end() const {
			return m_layers.end();
		}

		Layers::reverse_iterator rbegin() {
			return m_layers.rbegin();
		}
		Layers::reverse_iterator rend() {
			return m_layers.rend();
		}

		Layers::const_reverse_iterator rbegin() const {
			return m_layers.rbegin();
		}

		Layers::const_reverse_iterator rend() const {
			return m_layers.rend();
		}

	private:
		Layers m_layers;
		unsigned int m_layerInsertIndex = 0;
	};
}

#endif // !HYPER_LAYER_STACK_HPP
