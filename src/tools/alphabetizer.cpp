#include "card_list.hpp"
#include "card_shuffler.hpp"
#include "card_sorter.hpp"
#include "csv_tool.hpp"
#include "trading_card.hpp"

#include <algorithm>
#include <iostream>
#include <list>

//-------------------------
//utility functions
//-------------------------

#ifndef itoa
char* itoa(int value, char* str, int base) {
	//ignore base
	sprintf(str, "%d", value);
	return str;

}
#endif

std::string ItoA(int v, int base) {
	char buffer[64];
	itoa(v, buffer, base);
	return buffer;
}

#ifndef stricmp
//https://thedarkgod.wordpress.com/2009/08/12/stricmp-comparing-two-strings-in-c-case-insensitively/
int stricmp (const char *s1, const char *s2) {
	if (s1 == NULL) return s2 == NULL ? 0 : -(*s2);
	if (s2 == NULL) return *s1;

	char c1, c2;
	while ((c1 = tolower (*s1)) == (c2 = tolower (*s2))) {
		if (*s1 == '\0') break;
		++s1; ++s2;
	}

	return c1 - c2;
}
#endif

TradingCard::Type readType(std::string s) {
	if (!stricmp(s.c_str(), "basic tower"))	return TradingCard::Type::BASIC_TOWER;
	if (!stricmp(s.c_str(), "mecha"))		return TradingCard::Type::MECHA;
	if (!stricmp(s.c_str(), "command"))		return TradingCard::Type::COMMAND;
	if (!stricmp(s.c_str(), "tower"))		return TradingCard::Type::TOWER;
	if (!stricmp(s.c_str(), "mecha tower"))	return TradingCard::Type::MECHA_TOWER;
	std::ostringstream msg;
	msg << "Failed to read the card type: " << s;
	throw(std::runtime_error(msg.str()));
}

std::string writeType(TradingCard::Type type) {
	switch (type) {
		case TradingCard::Type::BASIC_TOWER:	return "basic tower";
		case TradingCard::Type::MECHA:			return "mecha";
		case TradingCard::Type::COMMAND:		return "command";
		case TradingCard::Type::TOWER:			return "tower";
		case TradingCard::Type::MECHA_TOWER:	return "mecha tower";
	}
	std::ostringstream msg;
	msg << "Failed to write the card type: " << type;
	throw(std::runtime_error(msg.str()));
}

TradingCard::Rarity readRarity(std::string s) {
	if (!stricmp(s.c_str(), "common"))		return TradingCard::Rarity::COMMON;
	if (!stricmp(s.c_str(), "uncommon"))	return TradingCard::Rarity::UNCOMMON;
	if (!stricmp(s.c_str(), "rare"))		return TradingCard::Rarity::RARE;
	std::ostringstream msg;
	msg << "Failed to read the card rarity: " << s;
	throw(std::runtime_error(msg.str()));
}

std::string writeRarity(TradingCard::Rarity rarity) {
	switch (rarity) {
		case TradingCard::Rarity::COMMON:	return "common";
		case TradingCard::Rarity::UNCOMMON:	return "uncommon";
		case TradingCard::Rarity::RARE:		return "rare";
	}
	std::ostringstream msg;
	msg << "Failed to write the card rarity: " << rarity;
	throw(std::runtime_error(msg.str()));
}

//-------------------------
//Important stuff
//-------------------------

template<typename Card>
class AlphabetSorter : public CardSorter<Card> {
	//DOCS: reverse, since the linked list inherently reverses shit.
protected:
	int Compare(Card* lhs, Card* rhs) override {
		return stricmp(lhs->GetName().c_str(), rhs->GetName().c_str());
	}
};

typedef CardList<TradingCard, AlphabetSorter<TradingCard>, CardShuffler<TradingCard>> CardListDef;

//DOCS: alphabetize the csv file
int main(int, char**) {
	//format
	//#rarity;name;type;cost;power;durability;text

	//the intermediate format
	CardListDef cardMasterList;

	//read in the old format
	CSVObject<7> cardFormatOne = readCSV<7>("base_set.csv", ';');

	for (auto& it : cardFormatOne) {
		std::cout << it[1] << std::endl;

		//make and set the new card
		TradingCard* floatingCard = new TradingCard();
		floatingCard->SetRarity(readRarity(it[0]));
		floatingCard->SetName(it[1]);
		floatingCard->SetType(readType(it[2]));
		floatingCard->SetCost(std::atoi(it[3].c_str()));
		floatingCard->SetPower(std::atoi(it[4].c_str()));
		floatingCard->SetDurability(std::atoi(it[5].c_str()));
		floatingCard->SetText(it[6]);

		cardMasterList.Push(floatingCard);
	}

	cardMasterList.Sort();

	//write the new format
	CSVObject<7> cardFormatTwo;

	for (TradingCard* ptr = cardMasterList.Peek(); ptr; ptr = ptr->GetNext()) {
		//common;name;type;cost;power;durability;text

		CSVElement<7> element;

		element[0] = writeRarity(ptr->GetRarity());
		element[1] = ptr->GetName().c_str();
		element[2] = writeType(ptr->GetType());
		element[3] = ItoA(ptr->GetCost(), 10);
		element[4] = ItoA(ptr->GetPower(), 10);
		element[5] = ItoA(ptr->GetDurability(), 10);
		element[6] = ptr->GetText();

		cardFormatTwo.push_back(element);
	}

	writeCSV<7>("base_set.csv", cardFormatTwo, ';');

	return 0;
}