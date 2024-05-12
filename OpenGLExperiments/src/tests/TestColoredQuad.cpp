#include "TestColoredQuad.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	TestColoredQuad::TestColoredQuad()
        : m_Color{ 0.2f, 0.3f, 0.8f, 1.0f }, m_Projection(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))), m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_Position(glm::vec3(910, 490, 0))
	{
        float positions[] = {
            -50.0f, -50.0f,  // 0 - index count for index buffer
             50.0f, -50.0f,  // 1
             50.0f,  50.0f,  // 2
            -50.0f,  50.0f  // 3
        };

        /* Index buffer for drawing the same points in
           the vertex buffer multiple times.*/
        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_VertexBuffer = new VertexBuffer(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        m_VertexArray = new VertexArray();
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = new IndexBuffer(indicies, 6);

        glm::mat4 mvp = m_Projection * m_Model * m_View;

        m_Shader = new Shader("res/shaders/ColoredQuad.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
	}

	TestColoredQuad::~TestColoredQuad()
	{
        delete m_VertexArray;
        delete m_VertexBuffer;
        delete m_Shader;
        delete m_Renderer;
        delete m_IndexBuffer;
        GLCall(glDisable(GL_BLEND));
	}

	void TestColoredQuad::OnUpdate(float deltaTime)
	{
	}

	void TestColoredQuad::OnRender()
	{
        m_Renderer->Clear();
        m_Model = glm::translate(glm::mat4(1.0f), m_Position);
        glm::mat4 mvp = m_Projection * m_Model * m_View;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
        m_Renderer->Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
	}

	void TestColoredQuad::OnImGuiRender()
	{
		ImGui::ColorEdit4("Box Color", m_Color);
        ImGui::SliderFloat("x", &m_Position.x, 0.0f, 1920.0f);
        ImGui::SliderFloat("y", &m_Position.y, 0.0f, 1080.0f);
	}
}