#pragma once

#include <future>
#include <functional>
#include <string>
#include <deque>

namespace fastball {

	class Section {
		std::future<bool> result;
	public:

		Section(const std::string& description, std::function<bool()> body) {
			result = std::async(std::launch::async, body);
		}
		bool get() {
			std::cout << "waiting\n";
			return result.get();
		}
		~Section() { std::cout << "section destroyed\n"; }

	};

	class Test {
		std::deque<Section> sections;
	public:

		Test(const std::string& description, std::function<void(Test* test)> body) : sections{} {
			body(this);
		}
		void addSection(const Section&& section) {
			sections.emplace_back(section);
		}
		~Test() {
			std::cout << "test complete\n";
			for (auto s = begin(sections); s != end(sections); ++s) {
				(*s).get();
				std::cout << "section complete\n";
			}
		}
	};

	class Assertion {
		bool result;
	public:
		Assertion(bool assertion) : result{ assertion } {}
		bool operator()() {
			return result;
		}
	};

	class Require : public Assertion {
	public:
		Require(bool assertion) : Assertion{ assertion } {}
	};

}
