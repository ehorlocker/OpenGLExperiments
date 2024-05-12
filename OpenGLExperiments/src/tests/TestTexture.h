#pragma once

#include "Test.h"
#include "Renderer.h"
#include "Texture.h"
#include "glm/glm.hpp"

namespace test {
	class TestTexture : public Test {
	public:
		TestTexture();
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		VertexArray* m_VertexArray;
		VertexBuffer* m_VertexBuffer;
		Shader* m_Shader;
		Renderer* m_Renderer;
		IndexBuffer* m_IndexBuffer;
		Texture* m_Texture;
		glm::vec3 m_PositionA, m_PositionB;
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_Model;
	};
}