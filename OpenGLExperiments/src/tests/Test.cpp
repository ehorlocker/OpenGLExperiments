#include "Test.h"

#include "imgui/imgui.h"
#include "Renderer.h"

namespace test {
	MainMenu::MainMenu(Test*& currentTestPointer)
		: currentTest(currentTestPointer) {
	}

	void MainMenu::OnRender() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void MainMenu::OnImGuiRender() {
		for (auto& test : m_Tests) {
			if (ImGui::Button(test.name.c_str())) {
				currentTest = test.getNewTest();
			}
		}
	}
}
