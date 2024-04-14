#pragma once
#ifndef HYP_ORTHOGRAPHIC_CAMERA_HPP
	#define HYP_ORTHOGRAPHIC_CAMERA_HPP

	#include <renderer/camera.hpp>

namespace hyp {
	class OrthoGraphicCamera : public hyp::Camera {
	public:
		OrthoGraphicCamera(float l, float r, float b, float t);

		void setProjection(float l, float r, float b, float t);

		float getRotation() const { return m_rotation; }

		void setRotation(float rotation) {
			m_rotation = rotation;
			recalculateViewMatrix();
		}

		const glm::vec3& getPosition() const {
			return m_position;
		}

		void setPosition(const glm::vec3& pos) {
			m_position = pos;
			recalculateViewMatrix();
		}

		const glm::mat4& getViewMatrix() const {
			return m_viewMatrix;
		}

		const glm::mat4& getViewProjectionMatrix() const {
			return m_viewProjectionMatrix;
		}

	private:
		void recalculateViewMatrix();

		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;

		glm::vec3 m_position = { 0.f, 0.f, 0.f };

		float m_rotation = 0.f;
	};
}

#endif // !HYP_ORTHOGRAPHIC_CAMERA_HPP
