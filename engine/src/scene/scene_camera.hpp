#pragma once
#ifndef HYP_SCENE_CAMERA_HPP
	#define HYP_SCENE_CAMERA_HPP

	#include <camera/camera.hpp>
	#include <glm/glm.hpp>

namespace hyp {
	class SceneCamera : public Camera {
	public:
		enum class ProjectionType
		{
			OrthoGraphic,
			Perspective
		};

	public:
		SceneCamera();

		void setPerspective(float fov, float nearClip, float farClip);

		void setOrthographic(float size, float nearClip, float farClip);

		void setViewport(uint32_t width, uint32_t height);

		float getPerspectiveFOV() const { return m_fov; }

		void setPerspectiveFOV(float fov) {
			m_fov = fov;
			recalculateProjection();
		}

		float getPerspectiveNear() const {
			return m_near;
		};
		void setPerspectiveNear(float value) {
			m_near = value;
			recalculateProjection();
		}

		float getPerspectiveFar() const {
			return m_far;
		};
		void setPerspectiveFar(float value) {
			m_far = value;
			recalculateProjection();
		}

		float getOrthoSize() const {
			return m_size;
		}

		void setOrthoSize(float value) {
			m_size = value;
			recalculateProjection();
		}

		float getOrthoNear() const {
			return m_orthoNear;
		}
		void setOrthoNear(float value) {
			m_orthoNear = value;
			recalculateProjection();
		}

		float getOrthoFar() const {
			return m_orthoFar;
		}
		void setOrthoFar(float value) {
			m_orthoFar = value;
			recalculateProjection();
		}

		[[nodiscard]] ProjectionType getProjectionType() const {
			return m_projectionType;
		}

		void setProjectionType(ProjectionType type) {
			m_projectionType = type;
			recalculateProjection();
		}

	private:
		void recalculateProjection();

		ProjectionType m_projectionType = ProjectionType::OrthoGraphic;

		// perspective
		float m_fov;
		float m_near = 0.001f, m_far = 1000.f;

		// orthographic
		float m_size = 10.f;
		float m_orthoNear = -1.f, m_orthoFar = 1.f;

		float m_aspectRatio = 0.f;
	};
}

#endif