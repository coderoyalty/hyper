#include "hierarchyPanel.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <imgui_internal.h>

static void drawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);

hyp::HierarchyPanel::HierarchyPanel(const hyp::Ref<hyp::Scene>& scene) {
	setContext(scene);
}

void hyp::HierarchyPanel::setContext(const hyp::Ref<hyp::Scene>& scene) {
	m_context = scene;
	m_selectedEntity = {};
}

void hyp::HierarchyPanel::setSelectedEntity(Entity entity) {
	m_selectedEntity = entity;
}

void hyp::HierarchyPanel::onUIRender() {
	ImGui::Begin("Scene Hierarchy");
	if (m_context)
	{
		m_context->m_registry.each([&](auto entityId)
		{
			Entity entity(entityId, m_context.get());
			drawEntityNode(entity);
		});

		if (ImGui::BeginPopupContextWindow(0,
		          ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
		{
			if (ImGui::MenuItem("Create Empty Entity")) {
				Entity entity = m_context->createEntity("Empty Entity");
				auto& model = entity.get<TransformComponent>();
				model.size = { 100.f, 100.f };
				auto& sprite = entity.add<SpriteRendererComponent>(glm::vec4(1.0, 0.0, 0.0, 1.f));

			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();

	ImGui::Begin("Properties");
	drawComponents(m_selectedEntity);
	ImGui::End();
}

void hyp::HierarchyPanel::drawEntityNode(Entity entity) {
	static Entity hoveredEntity;

	auto& tag = entity.get<hyp::TagComponent>().name;

	std::stringstream id_ss;
	id_ss << tag << entity;

	ImGuiTreeNodeFlags flags = ((m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx(id_ss.str().c_str(), flags, tag.c_str());

	if (ImGui::IsItemHovered()) {
		hoveredEntity = entity;
	}

	if (ImGui::IsItemClicked())
	{
		m_selectedEntity = entity;
	}

	bool entityDeleted = false;
	if (hoveredEntity == entity && ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		m_context->destroyEntity(entity);
		if (m_selectedEntity == entity)
			m_selectedEntity = {};
	}
}

void hyp::HierarchyPanel::drawComponents(Entity entity) {
	if (!entity) return;

	if (entity.has<TagComponent>())
	{
		auto& tag = entity.get<TagComponent>().name;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	auto& model = entity.get<TransformComponent>();

	drawVec3Control("Position", model.position);
	glm::vec3 size(model.size, 0.f);
	drawVec3Control("Size", size);
	model.size = glm::vec2(size);

	ImGui::Separator();

	if (entity.has<SpriteRendererComponent>()) {
		auto& sprite = entity.get<SpriteRendererComponent>();
		ImGui::ColorEdit4("Color", glm::value_ptr(sprite.color));
	}
}

void drawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth) {
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}
