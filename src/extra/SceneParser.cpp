#include "SceneParser.h"



SceneParser::SceneParser(char* path)
{
	FILE* f;
	struct stat stbuffer;

	stat(path, &stbuffer);
	f = fopen(path, "rb");
	size = (int)stbuffer.st_size;
	data = new char[size];
	sl = 0;
	fread(data, size, 1, f);
	fclose(f);
	for (int i = 0; i < size; i++) {
		strData.push_back(data[i]);
	}
}




