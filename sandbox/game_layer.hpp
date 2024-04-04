#pragma once
#include <core/application.hpp>
#include <renderer/orthographic_controller.hpp>

struct Paddle {
    glm::vec2 position;
    glm::vec2 size;

 
    // Function to update the paddle's position
    //void update(float dt) {
    //    // If moving up and reached top or moving down and reached bottom, reverse direction
    //    if ((movingUp && position.y <= 0.0f) || (!movingUp && position.y >= win_height - space - size.y)) {
    //        movingUp = !movingUp;
    //    }

    //    // Update position based on direction
    //    if (movingUp) {
    //        position.y -= speed * dt;
    //        if (position.y < 0.f) {
    //            position.y += space;
    //        }
    //    }
    //    else {
    //        position.y += speed * dt;
    //        if (position.y > win_height - space - size.y) {
    //            position.y -= space;
    //        }
    //    }
    //}
};

class GameLayer : public hyp::Layer {
public:
	GameLayer(const glm::vec2& size);
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(float dt) override;

	virtual void onEvent(hyp::Event&) {

	}

	virtual void onRender();
private:
	Paddle paddleLeft;
	Paddle paddleRight;

	hyp::Shared<hyp::OrthoGraphicCameraController> m_controller;
	float win_width;
	float win_height;

    bool movingUp = false;
};
