#include "LootTable.h"
#include <iostream>

void LootTable::LoadingIntoLootTableArray()
{
	file.open("LootTable.txt");
	std::string line, m_Name;
	size_t pos = 0;
	std::string delim = "-";

	for (int i = 0; i < 11 && std::getline(file, line); i++)
	{
		while ((pos = line.find(delim)) != std::string::npos)
		{
			m_Name = line.substr(0, pos);
			m_LootTableData[i].ItemName = m_Name;
			line.erase(0, pos + delim.length());
			m_LootTableData[i].DropChance = std::stoi(line);
		}
	}
	if (m_LootTableData[0].ItemName == "commonArmour")
	{
		std::cout << "Working\n";
	}
	RollLoot();
}

std::string LootTable::RollLoot()
{
	
	//Roll random number 0-100. rand % 100
	srand(time(0));

	//random number
	int random = (rand() % 100) + 1;
	int temp = 0;
	for (int i = 0; i < 9; i++)
	{
		temp += m_LootTableData[i].DropChance;
		if (random < temp)
		{
			//Output the item
			std::cout << m_LootTableData[i].ItemName;
			std::cout << random;
			std::string rolledLoot = m_LootTableData[i].ItemName;// this string return ting
			std::cout << "\n";
			return rolledLoot;
			break;
		}
	}
	
}
