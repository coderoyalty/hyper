#include "components.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

glm::mat4 hyp::TransformComponent::getTransform() const {
	glm::mat4 model(1.0);
	glm::mat4 rotate = glm::toMat4(glm::quat(rotation));
	auto& translate = glm::translate(model, position);
	auto& scaleMat = glm::scale(glm::mat4(1.0), scale);

	model = translate * rotate * scaleMat;

	return model;
}
