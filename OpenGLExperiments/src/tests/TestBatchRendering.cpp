#include "TestBatchRendering.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	test::TestBatchRendering::TestBatchRendering() 
		: m_Projection(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
		m_View(glm::mat4(1.0f)), m_Model(glm::mat4(1.0f)), m_Color{ 0.2f, 0.3f, 0.8f, 1.0f } {
        float positions[] = {
             835.0f, 490.0f, // 0
             935.0f, 490.0f, // 1
             935.0f, 590.0f, // 2
             835.0f, 590.0f, // 3
             985.0f, 490.0f, // 4
            1085.0f, 490.0f, // 5
            1085.0f, 590.0f, // 6
             985.0f, 590.0f  // 7
        };

        unsigned int indicies[] = {
            0, 1, 2, // quad 1
            2, 3, 0,
            4, 5, 6, // quad 2
            6, 7, 4
        };

        m_VertexBuffer = new VertexBuffer(positions, 8 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        m_VertexArray = new VertexArray();
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = new IndexBuffer(indicies, 12);

        m_Renderer = new Renderer();

        glm::mat4 mvp = m_Projection * m_Model * m_View;

        m_Shader = new Shader("res/shaders/BatchRender.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
        m_Shader->SetUniformMat4f("u_MVP", mvp);


        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
	}

	test::TestBatchRendering::~TestBatchRendering() {
        delete m_VertexArray;
        delete m_VertexBuffer;
        delete m_Shader;
        delete m_Renderer;
        delete m_IndexBuffer;
	}

	void test::TestBatchRendering::OnUpdate(float deltaTime) {
	}

	void test::TestBatchRendering::OnRender() {
        m_Renderer->Clear();

        {
            //m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(850, 490, 0));
            glm::mat4 mvp = m_Projection * m_Model * m_View;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
            m_Renderer->Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }
	}

	void test::TestBatchRendering::OnImGuiRender() {

	}
}