#pragma once

#include "TriVEngine.h"
#include <stdexcept>
#include <iostream>

int main() {
	TriV::TriVEngine engine;

	try {
		engine.initiateEngine();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
