#include "hierarchyPanel.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <imgui_internal.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace utils {
	static void drawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.f, float columnWidth = 100.f);
	static void drawVec2Control(const std::string& label, glm::vec2& values, float resetValue = 0.f, float columnWidth = 100.f);
}

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
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				Entity entity = m_context->createEntity("Empty Entity");
				auto& model = entity.get<TransformComponent>();
				model.size = { 0.5, 0.5f };
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();

	ImGui::Begin("Properties");
	{
		static bool showComponents;
		static const char* componentsName[] = {
			"None",
			"TransformComponent",
			"SpriteComponent",
			"CircleComponent",
			"TextComponent"
		};
		static int selectedItemIndex = 0;
		drawComponents(m_selectedEntity);

		if (m_selectedEntity)
		{
			if (ImGui::Button("Add Component"))
			{
				showComponents = true;
			}

			if (showComponents)
			{
				if (ImGui::BeginCombo("##component_combos", componentsName[selectedItemIndex]))
				{
					for (int i = 0; i < 5; i++)
					{
						bool isSelected = (selectedItemIndex == i);
						if (ImGui::Selectable(componentsName[i], isSelected))
						{
							showComponents = false;
							selectedItemIndex = i;
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus(); // Set focus on the selected item
						}
					}
					ImGui::EndCombo();
				}
			}

			if (selectedItemIndex != 0)
			{
				switch (selectedItemIndex)
				{
				case 1: // TransformComponent
				{
					auto& transform = m_selectedEntity.getOrAdd<hyp::TransformComponent>();
					transform.size = { 0.5, 0.5f };
					break;
				}
				case 2: // Sprite
				{
					if (!m_selectedEntity.has<hyp::CircleRendererComponent>() && !m_selectedEntity.has<hyp::TextComponent>())
					// shouldn't have sprite and circle component to an entity
					{
						m_selectedEntity.getOrAdd<hyp::SpriteRendererComponent>(glm::vec4(1.f));
					}
					break;
				}
				case 3: // Circle
				{
					if (!m_selectedEntity.has<hyp::SpriteRendererComponent>() && !m_selectedEntity.has<hyp::TextComponent>())
					// shouldn't have sprite and circle component to an entity
					{
						m_selectedEntity.getOrAdd<hyp::CircleRendererComponent>(glm::vec4(1.f));
					}
					break;
				}
				case 4: // Text
				{
					if (!(m_selectedEntity.has<hyp::SpriteRendererComponent>() || m_selectedEntity.has<hyp::CircleRendererComponent>()))
					{
						auto& text = m_selectedEntity.getOrAdd<hyp::TextComponent>();
						text.text = text.text.length() == 0 ? "Untitled" : text.text;
					}
					break;
				}
				default:
					break;
				}

				selectedItemIndex = 0;
			}
		}
	}
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

	if (ImGui::IsItemHovered())
	{
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

template <typename T, typename UIFunction>
static void drawComponent(const std::string& name, hyp::Entity entity, UIFunction uiFunction) {
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (entity.has<T>())
	{
		auto& component = entity.get<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 { 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2 { lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.remove<T>();
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

	drawComponent<hyp::TransformComponent>("Transform", entity, [](hyp::TransformComponent& component)
	{
		utils::drawVec3Control("Position", component.position, 0.f, 65.f);
		utils::drawVec2Control("Scale", component.size, 0.f, 65.f);
		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::DragFloat("##Rotation", &component.rotation, 0.1, -360.f, 360.f);
	});

	drawComponent<hyp::SpriteRendererComponent>("Sprite Renderer", entity, [](hyp::SpriteRendererComponent& component)
	{
		ImGui::ColorEdit4("Color", glm::value_ptr(component.color));

		ImGui::Button("Texture", ImVec2(100.0f, 0.0f));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;

				fs::path texturePath(path);
				if (fs::exists(texturePath))
				{
					hyp::Ref<hyp::Texture2D> texture = hyp::Texture2D::create(texturePath.string());

					if (texture->isLoaded())
					{
						component.texture = texture;
					}
				}
			}

			ImGui::EndDragDropTarget();
		}
		ImGui::Text("Tiling Factor");
		ImGui::SameLine();
		ImGui::DragFloat("##TilingFactor", &component.tilingFactor, 0.1f, 0.0f, 100.0f);
	});

	drawComponent<hyp::CircleRendererComponent>("Circle Renderer", entity, [](hyp::CircleRendererComponent& component)
	{
		ImGui::ColorEdit4("Color", glm::value_ptr(component.color));

		ImGui::Text("Thickness");
		ImGui::SameLine();
		ImGui::DragFloat("##Thickness", &component.thickness, 0.01f, 0.0f, 1.0f);

		ImGui::Text("Fade");
		ImGui::SameLine();
		ImGui::DragFloat("##Fade", &component.fade, 0.01f, 0.0f, 1.0f);
	});

	drawComponent<hyp::TextComponent>("Text Renderer", entity, [](hyp::TextComponent& component)
	{
		ImGui::Text("Color");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Color", glm::value_ptr(component.color));
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, sizeof(buffer), component.text.c_str(), sizeof(buffer));

		ImGui::Text("Text");
		ImGui::SameLine();
		if (ImGui::InputText("##Text", buffer, sizeof(buffer)))
		{
			component.text = std::string(buffer);
		}

		ImGui::Text("Font Size");
		ImGui::SameLine();
		ImGui::DragFloat("##FontSize", &component.fontSize, 1.f, 0.0f, 150.0f);

		ImGui::Text("Line Spacing");
		ImGui::SameLine();
		ImGui::DragFloat("##LineSpacing", &component.lineSpacing, 0.01f, 0.0f, 1.0f);
	});
}

void utils::drawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth) {
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 2, 2 });
	ImGui::PushID(label.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 2 });

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
	ImGui::DragFloat("##X", &values.x, 0.01f, 0.0f, 0.0f, "%.2f");
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
	ImGui::DragFloat("##Y", &values.y, 0.01f, 0.0f, 0.0f, "%.2f");
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
	ImGui::DragFloat("##Z", &values.z, 0.01f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
	ImGui::PopStyleVar();
}

void utils::drawVec2Control(const std::string& label, glm::vec2& values, float resetValue, float columnWidth) {
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 2 });

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
	ImGui::DragFloat("##X", &values.x, 0.01f, 0.0f, 0.0f, "%.2f");
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
	ImGui::DragFloat("##Y", &values.y, 0.01f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
	ImGui::Columns(1);

	ImGui::PopID();
}
