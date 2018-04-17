#include <iostream>
#include<fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <vector>

void ReadFile(const std::string& fileName, std::multimap<std::string, std::string>& words, std::vector<std::string>& sortedWords);
void FilterAnagrams(std::multimap<std::string, std::string>& anagrams, std::multimap<std::string, std::string>& words, const std::vector<std::string> sortedWords);
void FindLongest(const std::multimap<std::string, std::string>& anagrams);
void FindMost(const std::multimap<std::string, std::string>& anagrams);
void FindUserInput(const std::multimap<std::string, std::string>& anagrams);

bool IsAlpha(const char& a);
bool IsBigger(const std::pair<std::string, std::string> i, const std::pair<std::string, std::string> j);
bool IsMore(const std::multimap<std::string, std::string> i, const std::multimap<std::string, std::string> j);

int main()
{
	std::string fileName{ "Resources/words.txt" };
	std::multimap<std::string, std::string> words{};
	std::vector<std::string> sortedWords{};
	std::multimap<std::string, std::string> anagrams{};
	ReadFile(fileName, words, sortedWords);
	FilterAnagrams(anagrams, words, sortedWords);
	FindLongest(anagrams);
	FindMost(anagrams);
	FindUserInput(anagrams);
	std::cin.ignore();
}

void ReadFile(const std::string& fileName, std::multimap<std::string, std::string>& words, std::vector<std::string>& sortedWords)
{
	std::ifstream input{ fileName };
	if (!input)
	{
		std::cout << "Failed to load " << fileName << std::endl;
		return;
	}
	else
	{
		std::cout << "Reading and processing " << fileName << "..." << std::endl;
	}

	std::string temp;
	while (input.good())
	{
		input >> temp;

		std::string::iterator it{};
		it = find_if(temp.begin(), temp.end(), IsAlpha);
		if (it == temp.end() && int(temp.size()) >= 5)
		{
			std::string unsorted{ temp };
			std::sort(temp.begin(), temp.end());
			words.insert(std::pair<std::string, std::string>(temp, unsorted));
			sortedWords.push_back(temp);
		}
	}

	std::cout << "Reading file completed" << std::endl;
	std::cout << std::endl;
}

void FilterAnagrams(std::multimap<std::string, std::string>& anagrams, std::multimap<std::string, std::string>& words, const std::vector<std::string> sortedWords)
{
	std::cout << "Filtering all anagrams..." << std::endl;
	for (std::vector<std::string>::const_iterator it{ sortedWords.begin() }; it != sortedWords.end(); ++it)
	{
		std::pair<std::multimap<std::string, std::string>::const_iterator, std::multimap<std::string, std::string>::const_iterator> bounds{};
		bounds = words.equal_range(*it);

		std::multimap<std::string, std::string> temp{};
		for (std::multimap<std::string, std::string>::const_iterator it2{ bounds.first }; it2 != bounds.second; ++it2)
		{
			temp.insert(*it2);
		}
		if (temp.size() > 1)
		{
			anagrams.insert(temp.begin(), temp.end());
		}
		words.erase(bounds.first, bounds.second);
	}
	std::cout << "Filtering anagrams completed" << std::endl;
	std::cout << std::endl;
}

void FindLongest(const std::multimap<std::string, std::string>& anagrams)
{
	std::multimap<std::string, std::string>::const_iterator it{};
	it = std::max_element(anagrams.begin(), anagrams.end(), IsBigger);

	std::pair<std::multimap<std::string, std::string>::const_iterator, std::multimap<std::string, std::string>::const_iterator> longest{};
	longest = anagrams.equal_range((*it).first);
	std::cout << "Longest anagram: ";
	for(std::multimap<std::string, std::string>::const_iterator it2{ longest.first }; it2 != longest.second; ++it2)
	{
		std::cout << (*it2).second << ' ';
	}
	std::cout << std::endl;
}

void FindMost(const std::multimap<std::string, std::string>& anagrams)
{
	std::vector<std::multimap<std::string, std::string>> groups{};
	for (std::multimap<std::string, std::string>::const_iterator it{ anagrams.begin() }; it != anagrams.end(); ++it)
	{
		std::pair<std::multimap<std::string, std::string>::const_iterator, std::multimap<std::string, std::string>::const_iterator> same{};
		same = anagrams.equal_range((*it).first);

		std::multimap<std::string, std::string> temp{};
		for (std::multimap<std::string, std::string>::const_iterator it2{ same.first }; it2 != same.second; ++it2)
		{
			temp.insert(*it2);
		}
		
		groups.push_back(temp);
	}

	std::vector<std::multimap<std::string, std::string>>::const_iterator most{};
	most = std::max_element(groups.begin(), groups.end(), IsMore);

	std::pair<std::multimap<std::string, std::string>::const_iterator, std::multimap<std::string, std::string>::const_iterator> longest{};
	longest = anagrams.equal_range((*(*most).begin()).first);
	std::cout << "Most anagrams: ";
	for (std::multimap<std::string, std::string>::const_iterator it2{ longest.first }; it2 != longest.second; ++it2)
	{
		std::cout << (*it2).second << ' ';
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

void FindUserInput(const std::multimap<std::string, std::string>& anagrams)
{
	std::string input{};
	do 
	{
		std::cout << '>';
		std::cin >> input;
		std::sort(input.begin(), input.end());

		std::pair<std::multimap<std::string, std::string>::const_iterator, std::multimap<std::string, std::string>::const_iterator> all{};
		all = anagrams.equal_range(input);
		
		if(std::distance(all.first, all.second) == 0)
		{
			std::cout << "No anagrams found..." << std::endl;
		}
		else
		{
			std::cout << "Anagrams: ";
			for (std::multimap<std::string, std::string>::const_iterator it2{ all.first }; it2 != all.second; ++it2)
			{
				std::cout << (*it2).second << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	} while (int(input.size()) != 1);
}

bool IsAlpha(const char& a)
{
	if (!isalpha(a))
	{
		return true;
	}
	return false;
}

bool IsBigger(const std::pair<std::string, std::string> i, const std::pair<std::string, std::string> j)
{
	return i.first.size() < j.first.size();
}

bool IsMore(const std::multimap<std::string, std::string> i, const std::multimap<std::string, std::string> j)
{
	return i.size() < j.size();
}
