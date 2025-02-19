#pragma once
#include <memory>
#include <string>
#include "Component.h"
#include <glm.hpp>

namespace dae
{
	class Texture2D;

	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* pOwner, const std::string& filename, glm::vec3 position);
		~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		void Update() override {};
		void Render() const override;

		void SetTexture(const std::string& filename);

	private:
		bool m_needsUpdate;
		std::shared_ptr<Texture2D> m_Texture;
		glm::vec3 m_Position;
	};
}

