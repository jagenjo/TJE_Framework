#include "SceneParser.h"
#include <fstream>


SceneParser::SceneParser(char* path)
{
	std::ifstream file(path);
	std::string input;

	while (file >> input) {
		lines.push_back(input);
	}
}






