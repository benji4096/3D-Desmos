#include <iostream>
#include <optional>

#include "Grapher3D.hpp"

std::optional<Grapher3D> graphApplication = std::optional<Grapher3D>(); // Use of std::optional for delayed instantiation


int main(int argc, char* argv[])
{
	try
	{
		graphApplication.emplace(600, 600); // instantiate grapher application
		graphApplication.value().run(); // run main loop
		graphApplication.reset(); // cleanup
	}
	catch (const std::exception e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "Unknown Error!" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
