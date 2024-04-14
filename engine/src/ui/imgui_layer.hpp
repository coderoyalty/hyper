#ifndef HYP_IMGUI_LAYER_HPP
#define HYP_IMGUI_LAYER_HPP

#include <core/layer.hpp>

namespace hyp {
	class ImGuiLayer : public hyp::Layer {
	public:
		ImGuiLayer()
		    : hyp::Layer("ui_layer") {};

		virtual void onEvent(hyp::Event& event) override;
		virtual void onAttach() override;
		virtual void onDetach() override;

		void begin();
		void end();

		void blockEvent(bool value);

	private:
		bool m_blockEvents = false;
	};
} // namespace hyp

#endif // !HYP_IMGUI_LAYER_HPP
