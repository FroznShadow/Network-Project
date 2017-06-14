#ifndef Engine_Renderable_hpp
#define Engine_Renderable_hpp

#include <glm/glm.hpp>
#include <Core/Managers/EntityManager.h>

namespace Engine {

	class Render : public Component
	{
	public:
		Render(std::vector<glm::vec3> vertexData, std::vector<glm::uvec3> indiceData)
			: _vertexData(vertexData), _vertexIndexData(indiceData)
		{};
		virtual ~Render() {};

		virtual void Init() override;
		virtual void Cleanup() override;

		virtual void Update(DeltaTime deltaTime) override;

		std::vector<glm::vec3> GetVertexData() { return _vertexData; };
		std::vector<glm::uvec3> GetIndiceData() { return _vertexIndexData; };
	private:
		std::vector<glm::vec3> _vertexData;
		std::vector<glm::uvec3> _vertexIndexData;
	};

	void Render::Init() {};

	void Render::Cleanup() {};

	void Render::Update(DeltaTime deltaTime) {};
};

#endif