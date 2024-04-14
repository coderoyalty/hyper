#include "layer_stack.hpp"

hyp::LayerStack::~LayerStack() {
	for (auto layer : m_layers)
	{
		if (layer)
		{
			layer->onDetach();
			delete layer;
		}
	}
}

void hyp::LayerStack::pushLayer(hyp::Layer* layer) {
	HYP_ASSERT_CORE(layer != nullptr, "can't add a nullptr to the layerstack");
	this->m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
	m_layerInsertIndex++;
}

void hyp::LayerStack::popLayer(hyp::Layer* layer) {
	if (layer == nullptr)
	{
		HYP_WARN("attempt to remove an empty layer will be futile");
		return;
	}
	auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
	if (it != m_layers.begin() + m_layerInsertIndex)
	{
		(*it)->onDetach();
		m_layers.erase(it);
		m_layerInsertIndex--;
	}
}

void hyp::LayerStack::pushOverlay(hyp::Layer* overlay) {
	if (overlay == nullptr)
	{
		HYP_WARN("attempt to remove an empty layer will be futile");
		return;
	}
	m_layers.emplace_back(overlay);
}

void hyp::LayerStack::popOverlay(hyp::Layer* overlay) {
	HYP_ASSERT_CORE(overlay != nullptr, "Cannot remove a nullptr layer from the layer stack");
	auto it = std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), overlay);
	if (it != m_layers.end())
	{
		overlay->onDetach();
		m_layers.erase(it);
	}
}
