#pragma once
#ifndef HYP_EDITOR_CAMERA_HPP
	#define HYP_EDITOR_CAM #ERA_HPP
	#include <camera/camera.hpp>
	#include <event/event.hpp>
	#include <io/input.hpp>
	#include <event/mouse_event.hpp>

namespace hyp {
	class EditorCamera : public hyp::Camera {
	public:
		EditorCamera() = default;

		EditorCamera(float fov, float aspectRatio, float near, float far);

		void onUpdate(float dt);
		void onEvent(hyp::Event& e);

		void setViewport(float v_width, float v_height) {
			m_vWidth = v_width;
			m_vHeight = v_height;

			updateProjection();
		}

		inline float getDistance() const { return m_distance; }
		inline void setDistance(float distance) { m_distance = distance; }

		inline float getYaw() const { return m_yaw; }
		inline float getPitch() const { return m_pitch; }

		glm::mat4 getViewMatrix() const;
		glm::mat4 getViewProjectionMatrix() const;

		glm::vec3 getUpDirection() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getForwardDirection() const;

		const glm::vec3& getPosition() const { return m_position; }
		void setPosition(const glm::vec3& pos) { m_position = pos; }

		glm::quat getOrientation() const;

	private:
		bool onMouseScrolled(hyp::MouseScrolledEvent& ev);

		void updateProjection();
		void updateView();

		glm::vec3 calcPosition();

	private:
		void mousePan(const glm::vec2& delta);
		void mouseRotate(const glm::vec2& delta);
		void mouseZoom(const float& delta);

		std::pair<float, float> panSpeed();

		float zoomSpeed() const;

	private:
		float m_fov = 45.f, m_near = 0.1f, m_far = 1000.f;
		float m_aspectRatio = 1.778f;

		glm::vec3 m_focalPoint = { 0.f, 0.f, 0.f };

		glm::mat4 m_viewMatrix = glm::mat4(1.0);
		glm::vec3 m_position = { 0.f, 0.f, 0.f };
		float m_yaw = 0.f, m_pitch = 0.f;
		float m_distance = 10.f;
		glm::vec2 m_initialMousePos = { 0.f, 0.f };

		float m_vWidth = 1280.f, m_vHeight = 720.f;
	};
}

#endif