#pragma once

#include "Test.h"
#include "Renderer.h"
#include "glm/glm.hpp"

namespace test {
	class TestColoredQuad : public Test {
	public:
		TestColoredQuad();
		~TestColoredQuad();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_Color[4];
		VertexArray* m_VertexArray;
		VertexBuffer* m_VertexBuffer;
		Shader* m_Shader;
		Renderer* m_Renderer;
		IndexBuffer* m_IndexBuffer;
		glm::vec3 m_Position;
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_Model;
	};
}