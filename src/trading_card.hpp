/* Copyright: (c) Kayne Ruse 2016
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 
 * 3. This notice may not be removed or altered from any source
 * distribution.
*/
#pragma once

#include "image.hpp"

#include <string>

class TradingCard {
public:
	enum Type {
		BASIC_TOWER,
		MECHA,
		COMMAND,
		TOWER,
		MECHA_TOWER
	};

	enum Rarity {
		COMMON,
		UNCOMMON,
		RARE
	};

	TradingCard() = default;
	~TradingCard() = default;

	//card components
	std::string SetName(std::string name);
	std::string GetName() const;

	Type SetType(Type t);
	Type GetType() const;

	int SetCost(int i);
	int GetCost() const;

	std::string SetText(std::string text);
	std::string GetText() const;

	int SetPower(int i);
	int GetPower() const;

	int SetDurability(int i);
	int GetDurability() const;

	Rarity SetRarity(Rarity r);
	Rarity GetRarity() const;

	//graphics
	int SetPosX(int i);
	int GetPosX() const;

	int SetPosY(int i);
	int GetPosY() const;

	Image* GetImage();

	//linked list
	TradingCard* SetNext(TradingCard* n);
	TradingCard* GetNext();

private:
	//card components
	std::string name;
	Type type;
	int cost = 0;
	std::string text;
	int power = 0;
	int durability = 0;
	Rarity rarity;

	//graphics
	int posX = 0;
	int posY = 0;
	Image image;

	//linked list
	TradingCard* next = nullptr;
};