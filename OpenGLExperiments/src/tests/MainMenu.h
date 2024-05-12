#pragma once

#include "Test.h"

namespace test {
	class MainMenu {
	public:
		MainMenu();
		~MainMenu();

		void OnUpdate(float deltaTime);
		void OnRender();
		void OnImGuiRender(Test*& currentTest);
		bool InMainMenu();
		void SetInMainMenu(bool inMainMenu);
	private:
		bool m_InMainMenu;
	};
}