#pragma once
#ifndef HYP_CAMERA_HPP
	#define HYP_CAMERA_HPP

	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>

namespace hyp {
	class Camera {
	public:
		Camera() : m_projectionMatrix(1.0) {
		}

		virtual void setProjectionMatrix(const glm::mat4& matrix) {
			m_projectionMatrix = matrix;
		};

		const glm::mat4& getProjectionMatrix() const {
			return m_projectionMatrix;
		};

	protected:
		glm::mat4 m_projectionMatrix;
	};
}

#endif // !HYP_CAMERA_HPP
