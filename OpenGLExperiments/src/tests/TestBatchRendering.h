#pragma once

#include "Test.h"
#include "Renderer.h"
#include "Texture.h"

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
		Texture* m_Fireplace;
		Texture* m_Logs;
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_Model;
	};
}