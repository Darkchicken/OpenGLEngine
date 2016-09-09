#include "IOManager.h"

#include<fstream>

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
{
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail())
	{
		perror(filePath.c_str());
		return false;
	}

	//seek to end of file
	file.seekg(0,std::ios::end);
	//figure out how many bytes file is
	int fileSize = file.tellg();
	//return to beginning to begin read
	file.seekg(0, std::ios::beg);

	//reduce the file size by any header bytes that might be present
	fileSize -= file.tellg();

	buffer.resize(fileSize);
	//reads a number of bytes into a buffer
	file.read((char *)&(buffer[0]),fileSize);

	file.close();
	return true;
}