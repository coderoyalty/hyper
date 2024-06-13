#pragma once
#ifndef HYP_ORTHOGRAPHIC_CONTROLLER

	#define HYP_ORTHOGRAPHIC_CONTROLLER

	#include <event/event.hpp>
	#include <event/mouse_event.hpp>
	#include <event/key_event.hpp>
	#include <event/win_event.hpp>
	#include <io/input.hpp>
	#include <camera/orthographic_camera.hpp>
	#include <glm/glm.hpp>

namespace hyp {

	class OrthoGraphicCameraController {
	public:
		typedef void (*OrthoGraphicOnProjectionCB)(OrthoGraphicCameraController* controller, float width, float height);

		OrthoGraphicCameraController(float viewport_width, float viewport_height, OrthoGraphicOnProjectionCB projectionCB = nullptr);

		void onEvent(hyp::Event& event);

		void onUpdate(float dt);

		void onResize(float width, float height);

		void setProjectionCallBack(OrthoGraphicOnProjectionCB cb);

		hyp::OrthoGraphicCamera& getCamera() {
			return m_camera;
		}

		void setCamera(hyp::OrthoGraphicCamera& camera) {
			m_camera = camera;
		}

		float getZoomLevel() const { return m_zoomLevel; }

		void setZoomLevel(float zoom) {
			m_zoomLevel = std::max(zoom, 0.25f);
		}

		float getCameraSpeed() const { return m_cameraSpeed; }

		void setCameraSpeed(float speed) { m_cameraSpeed = speed; }

	private:
		bool onMouseScrolled(hyp::MouseScrolledEvent& event);
		bool onWindowResized(hyp::WindowResizeEvent& event);

	private:
		float m_aspectRatio;
		float m_width = 0.f, m_height = 0.f;
		float m_zoomLevel = 1.f;

		float m_cameraSpeed = 2.f;
		float m_cameraRotationSpeed = 45.f;

		hyp::OrthoGraphicCamera m_camera;
		OrthoGraphicOnProjectionCB m_projectionCB = nullptr;
	};
}

#endif // !HYP_ORTHOGRAPHIC_CONTROLLER