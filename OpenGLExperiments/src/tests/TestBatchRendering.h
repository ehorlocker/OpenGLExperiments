#pragma once

#include "Test.h"
#include "Renderer.h"

namespace test {
	class TestBatchRendering : public Test {
	public:
		TestBatchRendering();
		~TestBatchRendering();

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
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_Model;
	};
}