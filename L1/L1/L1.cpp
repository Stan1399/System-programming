// L1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main()
{
	
	std::string prefix, filename;
	getline(std::cin, prefix);
	getline(std::cin, filename);
	std::ofstream out(filename, std::ios_base::app);
	std::string path = "./";
	for (auto &p : fs::directory_iterator(path))
	{
		const fs::path filepath = p.path();
		const fs::path fileName = filepath.filename();
		std::string stringFileName = fileName.u8string();
		if (stringFileName.substr(0, prefix.length()) == prefix) {
			std::ifstream in(fileName);
			std::string s;
			while (getline(in, s))
			{
				out << s;
			}
		}
	}

	//system("pause");
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
