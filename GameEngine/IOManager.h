#pragma once
#include <vector>
//provide extra funcitonality for reading and writing files
//static class, only need 1 io manager

namespace GameEngine
{
	static class IOManager
	{
	public:
		//true if successful
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	};
}

