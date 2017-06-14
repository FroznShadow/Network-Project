#ifndef Engine_AABB_hpp
#define Engine_AABB_hpp

#include <glm/glm.hpp>
#include <Core/Managers/EntityManager.h>

namespace Engine {

	class AABB : public Component
	{
	public:
		friend class PhysicsSystem;

		AABB() {};
		virtual ~AABB() {};

		virtual void Init() override;
		virtual void Cleanup() override;

		virtual void Update(DeltaTime deltaTime) override;

		void UpdateAABB(glm::vec3 min, glm::vec3 max);

		std::vector<glm::vec3> GetVertexData() { return _AABBVetrexData; };
		std::vector<glm::uvec3> GetIndiceData() { return _AABBIndiceData; };
	private:
		std::vector<glm::vec3> _AABBVetrexData;
		std::vector<glm::uvec3> _AABBIndiceData;

		glm::vec3 _min;
		glm::vec3 _max;
	};

	inline void AABB::Init() {
		_AABBIndiceData.push_back(glm::uvec3(0, 1, 2));
		_AABBIndiceData.push_back(glm::uvec3(2, 3, 0));
		_AABBIndiceData.push_back(glm::uvec3(0, 5, 6));
		_AABBIndiceData.push_back(glm::uvec3(1, 6, 7));
		_AABBIndiceData.push_back(glm::uvec3(2, 7, 4));
		_AABBIndiceData.push_back(glm::uvec3(3, 4, 5));
	}

	inline void AABB::Cleanup() {}

	inline void AABB::Update(DeltaTime deltaTime) {}

	inline void AABB::UpdateAABB(glm::vec3 min, glm::vec3 max) {
		_min = min;
		_max = max;

		_AABBVetrexData.clear();

		_AABBVetrexData.push_back(glm::vec3(max.x, max.y, max.z));
		_AABBVetrexData.push_back(glm::vec3(max.x, min.y, max.z));
		_AABBVetrexData.push_back(glm::vec3(min.x, min.y, max.z));
		_AABBVetrexData.push_back(glm::vec3(min.x, max.y, max.z));
		_AABBVetrexData.push_back(glm::vec3(min.x, max.y, min.z));
		_AABBVetrexData.push_back(glm::vec3(max.x, max.y, min.z));
		_AABBVetrexData.push_back(glm::vec3(max.x, min.y, min.z));
		_AABBVetrexData.push_back(glm::vec3(min.x, min.y, min.z));
	}
};

#endif