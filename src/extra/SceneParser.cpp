#include "SceneParser.h"
#include <fstream>
#include "../Scene.h"

SceneParser::SceneParser(char* path)
{
	std::ifstream file(path);
	std::string input;

	while (file >> input) {
		data.push_back(input);
	}
}

void SceneParser::LoadScene() {
	std::string cur;
	int LineNum = 0;
	for (int i = 0; i < data.size(); i+=6) {
		for (int j = i; j < LineNum * 6; j++) {
			cur = data.at(j);

		}
	}
	
}





