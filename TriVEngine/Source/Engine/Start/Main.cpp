#include <stdexcept>
#include <iostream>
#include "../Core/TriVEngine.hpp"

int main() {
	TriV::Engine::Core::TriVEngine engine;

	try {
		engine.StartEngine();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
