#include "MainMenu.h"

#include "imgui/imgui.h"

#include "TestClearColor.h"
#include "TestColoredQuad.h"
#include "TestTexture.h"
#include "Renderer.h"

namespace test {
	MainMenu::MainMenu()
		: m_InMainMenu(true)
	{
	}

	MainMenu::~MainMenu()
	{
	}

	void MainMenu::OnUpdate(float deltaTime)
	{
	}

	void MainMenu::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void MainMenu::OnImGuiRender(Test*& currentTest)
	{
		if (ImGui::Button("Clear Color")) {
			delete currentTest;
			currentTest = new TestClearColor();
			m_InMainMenu = false;
		}

		if (ImGui::Button("Colored Quad")) {
			delete currentTest;
			currentTest = new TestColoredQuad();
			m_InMainMenu = false;
		}

		if (ImGui::Button("Textured Quad")) {
			delete currentTest;
			currentTest = new TestTexture();
			m_InMainMenu = false;
		}
	}

	bool MainMenu::InMainMenu()
	{
		return m_InMainMenu;
	}

	void MainMenu::SetInMainMenu(bool inMainMenu) {
		m_InMainMenu = inMainMenu;
	}
}

