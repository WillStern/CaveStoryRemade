#pragma once
#include <string>
#include <vector>
#include <algorithm>


class Utils
{
public:

	//Split
	//splits a string txt, everywhere a certain character is found, ch
	//store resulting substring into vector strs
	//returns the size of the vector
	static unsigned int Split(const std::string &txt, std::vector<std::string> &strs, char ch)
	{
		int pos = txt.find(ch);
		int initialPos = 0;
		strs.clear();
		while (pos != std::string::npos)
		{
			strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
			initialPos = pos + 1;

			pos = txt.find(ch, initialPos);
		}
		//add last one
		strs.push_back(txt.substr(initialPos, std::min<int>(pos, txt.size() - (initialPos + 1))));

		return strs.size();
	}

private:

};