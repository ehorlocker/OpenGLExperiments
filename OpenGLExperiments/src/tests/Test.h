#pragma once

#include <vector>
#include <string>
#include <functional>

namespace test {
	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	struct TestObject {
		std::string name;
		std::function<Test* ()> getNewTest;

		TestObject(std::string testName, std::function<Test* ()> testFunction) :
			name(testName), getNewTest(testFunction) {
		}
	};

	class MainMenu : public Test {
	public:
		MainMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;
		
		template<typename T>
		void RegisterTest(const std::string& name) {
			TestObject testObject(name, []() { return new T(); });
			m_Tests.push_back(testObject);
		}
	private:
		Test*& currentTest;
		std::vector<TestObject> m_Tests;
	};
}