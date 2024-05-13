#include "Test.h"

#include "imgui/imgui.h"

namespace test {
	MainMenu::MainMenu(Test*& currentTestPointer)
		: currentTest(currentTestPointer) {
	}

	void MainMenu::OnImGuiRender() {
		for (auto& test : m_Tests) {
			if (ImGui::Button(test.name.c_str())) {
				currentTest = test.getNewTest();
			}
		}
	}
}
