#include "hierarchyPanel.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <imgui_internal.h>
#include <filesystem>
#include <utils/file_dialog.hpp>
#include <scripting/script_engine.hpp>
#include <functional>
#include <unordered_map>

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
	drawHierarchyPanel();
	drawPropertiesPanel();
}

void hyp::HierarchyPanel::drawHierarchyPanel() {
	ImGui::Begin("Scene Hierarchy");
	if (!m_context)
	{
		ImGui::End();
		return;
	}

	auto view = m_context->getEntities<hyp::TagComponent, hyp::TransformComponent>();
	for (auto entityId : view)
	{
		Entity entity(entityId, m_context.get());
		drawEntityNode(entity);
	}

	if (ImGui::BeginPopupContextWindow(0,
	        ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
		{
			Entity entity = m_context->createEntity("Empty Entity");
			entity.get<TransformComponent>();
			this->setSelectedEntity(entity);
		}

		ImGui::EndPopup();
	}

	ImGui::End();
}

void hyp::HierarchyPanel::drawPropertiesPanel() {
	ImGui::Begin("Properties");

	static int selectedItemIndex = 0;
	drawComponents(m_selectedEntity);

	drawAddScriptModal();

	ImGui::End();
}

void hyp::HierarchyPanel::drawAddScriptModal() {
	if (!m_selectedEntity) return;

	if (!m_selectedEntity.has<hyp::ScriptComponent>() && ImGui::Button("Add Script"))
	{
		ImGui::OpenPopup("Add Script");
	}

	if (ImGui::BeginPopupModal("Add Script", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static bool dont_ask_me_next_time = false;
		static char buf[512];

		ImGui::InputText("##script_path", buf, sizeof(buf) / sizeof(char));
		ImGui::SameLine();
		if (ImGui::Button("..."))
		{
			auto path = hyp::FileDialog::openFile("*.lua\0", "Lua Script (*.lua)");
			path = fs::relative(path).string();
			std::memcpy(buf, path.c_str(), sizeof(buf) / sizeof(char));
		};

		if (buf[0] != '\0' && !fs::exists(buf))
		{
			if (ImGui::Button("Create", ImVec2(200, 0)))
			{
				//TODO: provide entity name
				if (hyp::scripting::create_script_file(buf, ""))
				{
					hyp::ScriptEngine::addScriptComponent(m_selectedEntity, buf);
					std::memset(buf, 0, sizeof(buf));
					ImGui::CloseCurrentPopup();
				}
			}
		}

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			hyp::ScriptEngine::addScriptComponent(m_selectedEntity, buf);
			std::memset(buf, 0, sizeof(buf));
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			std::memset(buf, 0, sizeof(buf));
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void hyp::HierarchyPanel::drawEntityNode(Entity entity) {
	static Entity hoveredEntity;

	auto& tag = entity.getName();

	std::stringstream id_ss;
	id_ss << tag << entity.getUUID();

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

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}

	if (ImGui::BeginPopup("AddComponent"))
	{
		drawAddComponentEntry<ScriptComponent>("Script"); // TODO: work with script modal
		drawAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
		drawAddComponentEntry<CircleRendererComponent>("Circle Renderer");
		drawAddComponentEntry<TextComponent>("Text Component");
		drawAddComponentEntry<RigidBodyComponent>("Rigid Body");
		drawAddComponentEntry<BoxColliderComponent>("Box Collider");
		drawAddComponentEntry<CircleColliderComponent>("Circle Collider");

		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();

	drawComponent<hyp::TransformComponent>("Transform", entity, [](hyp::TransformComponent& component)
	{
		utils::drawVec3Control("Position", component.position, 0.f, 65.f);
		utils::drawVec3Control("Scale", component.scale, 0.f, 65.f);
		utils::drawVec3Control("Rotation", component.rotation, 0.f, 65.f);
	});



	drawComponent<hyp::SpriteRendererComponent>("Sprite Renderer", entity, [](hyp::SpriteRendererComponent& component)
	{
		ImGui::ColorEdit4("Color", glm::value_ptr(component.color));

		if (ImGui::Button("Texture", ImVec2(100.0f, 0.0f)))
		{
			auto texturePath = hyp::FileDialog::openFile("*.png\0*.jpg\*.jpeg\0", "Image file");

			hyp::Ref<hyp::Texture2D> texture = hyp::Texture2D::create(fs::relative(texturePath).string());
			texture->setFilter(hyp::TextureParam::NEAREST);
			if (texture->isLoaded())
			{
				component.texture = texture;
			}
		}

		if (ImGui::BeginDragDropTarget()) // waiting for asset browser..
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

		if (component.texture)
		{
			ImGui::SameLine();
			auto texturePath = component.texture->getPath();
			ImGui::Text(texturePath.c_str());
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
		ImGui::DragFloat("##Fade", &component.fade, 0.00025f, 0.0f, 1.0f);
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
		if (ImGui::InputTextMultiline("##Text", buffer, sizeof(buffer)))
		{
			component.text = std::string(buffer);
		}

		ImGui::Text("Font Size");
		ImGui::SameLine();
		ImGui::DragFloat("##FontSize", &component.fontSize, 1.f, 0.0f, 150.0f);

		ImGui::Text("Line Spacing");
		ImGui::SameLine();
		ImGui::DragFloat("##LineSpacing", &component.lineSpacing, 0.01f, 0.1f, 1.0f);
	});

	drawComponent<RigidBodyComponent>("Rigid Body", entity, [](hyp::RigidBodyComponent& component)
	{
		const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
		const char* currentBodyTypeString = bodyTypeStrings[(int)component.type];
		if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
		{
			for (int i = 0; i < 3; i++)
			{
				bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
				if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
				{
					currentBodyTypeString = bodyTypeStrings[i];
					component.type = (RigidBodyComponent::BodyType)i;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::Checkbox("Fixed Rotation", &component.fixedRotation);
	});

	drawComponent<hyp::BoxColliderComponent>("Box Collider", entity, [](hyp::BoxColliderComponent& component)
	{
		ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
		ImGui::DragFloat2("Size", glm::value_ptr(component.size));

		ImGui::DragFloat("Density", &component.density, 0.01f, 0.0, 1.f);
		ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0, 1.f);
		ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0, 1.f);
		ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.f);
	});

	drawComponent<hyp::CircleColliderComponent>("Circle Collider", entity, [](hyp::CircleColliderComponent& component)
	{
		ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));

		ImGui::DragFloat("Radius", &component.radius, 0.1f);
		ImGui::DragFloat("Density", &component.density, 0.01f, 0.0, 1.f);
		ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0, 1.f);
		ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0, 1.f);
		ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.f);
	});

	drawComponent<hyp::ScriptComponent>("Script", entity, [](hyp::ScriptComponent& component)
	{
		static char buffer[128];
		strcpy_s(buffer, sizeof(buffer), component.script_file.c_str());
		if (ImGui::InputText("Script File Path", buffer, sizeof(buffer))) {
			component.script_file = buffer;
		}
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

template <typename T>
void hyp::HierarchyPanel::drawAddComponentEntry(const std::string& entryName) {
	if (!m_selectedEntity || m_selectedEntity.has<T>())
	{
		return;
	}

	if (ImGui::MenuItem(entryName.c_str()))
	{
		m_selectedEntity.add<T>();
		ImGui::CloseCurrentPopup();
	}
}
