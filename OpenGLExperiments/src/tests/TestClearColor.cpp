#include "TestClearColor.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test {
	test::TestClearColor::TestClearColor()
		: m_Color{ 0.2f, 0.3f, 0.8f, 1.0f }
	{
	}

	test::TestClearColor::~TestClearColor()
	{
	}

	void test::TestClearColor::OnUpdate(float deltaTime)
	{
	}

	void test::TestClearColor::OnRender()
	{
		GLCall(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void test::TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Screen Color", m_Color);
	}
}

