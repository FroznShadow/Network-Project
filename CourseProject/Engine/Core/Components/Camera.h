#ifndef Camera_hpp
#define Camera_hpp

#include <vector>

#include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Managers/EntityManager.hpp>
#include <Core/Components/Transform.hpp>

namespace Engine
{
	class Camera : public Component
	{
	public:
		Camera(glm::vec3 offset = glm::vec3(0.0f));
		virtual ~Camera();

		virtual void Init();
		virtual void Cleanup();

		virtual void Update(DeltaTime deltaTime);

		glm::mat4 const GetViewMatrix();
		glm::vec3 const GetPosition();
	private:
		std::shared_ptr<Transform> _ownerTransform;
		glm::vec3 _offset;
	};

	inline Camera::Camera(glm::vec3 offset)
		: _offset(offset) {

	};

	inline Camera::~Camera() {

	};

	inline void Camera::Init() {
		_ownerTransform = _owner->GetComponent<Transform>();
	};

	inline void Camera::Cleanup() {

	};

	inline void Camera::Update(DeltaTime deltaTime) {

	};

	inline glm::mat4 const Camera::GetViewMatrix() {
		return glm::lookAt(_ownerTransform->GetPosition() + _offset, _ownerTransform->GetPosition() + _ownerTransform->GetRotationCam(), glm::vec3(0.0f, 1.0f, 0.0f));
	};

	inline glm::vec3 const Camera::GetPosition() {
		return _ownerTransform->GetPosition();
	}
}

#endif