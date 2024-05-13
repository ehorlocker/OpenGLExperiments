#include "TestTexture.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestTexture::TestTexture()
        : m_Projection(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
        m_View(glm::mat4(1.0f)),m_Model(glm::mat4(1.0f)),
        m_PositionA(glm::vec3(850, 490, 0)), m_PositionB(glm::vec3(970, 490, 0))
    {
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
             50.0f, -50.0f, 1.0f, 0.0f, // 1
             50.0f,  50.0f, 1.0f, 1.0f, // 2
            -50.0f,  50.0f, 0.0f, 1.0f  // 3
        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_VertexBuffer = new VertexBuffer(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VertexArray = new VertexArray();
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = new IndexBuffer(indicies, 6);

        glm::mat4 mvp = m_Projection * m_Model * m_View;

        m_Shader = new Shader("res/shaders/basic.shader");
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        m_Texture = new Texture("res/texture/fireplace.png");
        m_Texture->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
    }

    TestTexture::~TestTexture()
    {
        delete m_VertexArray;
        delete m_VertexBuffer;
        delete m_Shader;
        delete m_Renderer;
        delete m_IndexBuffer;
        delete m_Texture;
        GLCall(glDisable(GL_BLEND));
    }

    void TestTexture::OnUpdate(float deltaTime)
    {
    }

    void TestTexture::OnRender()
    {
        m_Renderer->Clear();

        {
            m_Model = glm::translate(glm::mat4(1.0f), m_PositionA);
            glm::mat4 mvp = m_Projection * m_Model * m_View;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Renderer->Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }

        {
            m_Model = glm::translate(glm::mat4(1.0f), m_PositionB);
            glm::mat4 mvp = m_Projection * m_Model * m_View;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Renderer->Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }
    }

    void TestTexture::OnImGuiRender()
    {
        ImGui::Text("Position A");
        ImGui::SliderFloat("Ax", &m_PositionA.x, 0.0f, 1920.0f);
        ImGui::SliderFloat("Ay", &m_PositionA.y, 0.0f, 1080.0f);
        ImGui::Text("Position B");
        ImGui::SliderFloat("Bx", &m_PositionB.x, 0.0f, 1920.0f);
        ImGui::SliderFloat("By", &m_PositionB.y, 0.0f, 1080.0f);
    }
}