#pragma once
#ifndef HYP_HIERARCHYPANEL_HPP

	#define HYP_HIERARCHYPANEL_HPP

	#include <core/base.hpp>
	#include <scene/scene.hpp>
	#include <scene/entity.hpp>
	#include <scene/components.hpp>
	#include <imgui.h>

namespace hyp {
	class HierarchyPanel {
	public:
		HierarchyPanel(const hyp::Ref<hyp::Scene>& scene);

		void setContext(const hyp::Ref<hyp::Scene>& scene);

		Entity getSelectedEntity() const { return m_selectedEntity; }
		void setSelectedEntity(Entity entity);

		void onUIRender();

	private:
		void drawHierarchyPanel();
		void drawPropertiesPanel();
		void drawAddScriptModal();

	private:
		void drawEntityNode(Entity entity);
		void drawComponents(Entity entity);
		template <typename T>
		void drawAddComponentEntry(const std::string& entryName);

	private:
		hyp::Ref<hyp::Scene> m_context;
		Entity m_selectedEntity;

		std::unordered_map<std::string, ComponentAdder> m_componentAdders;
		bool m_listComponents = false;
	};
}

#endif