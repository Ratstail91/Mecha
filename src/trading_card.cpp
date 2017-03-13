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
#include "trading_card.hpp"

//-------------------------
//card components
//-------------------------

std::string TradingCard::SetName(std::string s) {
	return name = s;
}

std::string TradingCard::GetName() const {
	return name;
}

TradingCardTypes TradingCard::SetTypes(TradingCardTypes t) {
	return types = t;
}

TradingCardTypes TradingCard::GetTypes() const {
	return types;
}

int TradingCard::SetCost(int i) {
	return cost = i;
}

int TradingCard::GetCost() const {
	return cost;
}

std::string TradingCard::SetText(std::string s) {
	return text = s;
}

std::string TradingCard::GetText() const {
	return text;
}

int TradingCard::SetPower(int i) {
	return power = i;
}

int TradingCard::GetPower() const {
	return power;
}

int TradingCard::SetDurability(int i) {
	return durability = i;
}

int TradingCard::GetDurability() const {
	return durability;
}

//-------------------------
//graphics
//-------------------------

int TradingCard::SetPosX(int i) {
	return posX = i;
}

int TradingCard::GetPosX() const {
	return posX;
}

int TradingCard::SetPosY(int i) {
	return posY = i;
}

int TradingCard::GetPosY() const {
	return posY;
}

Image* TradingCard::GetImage() {
	return &image;
}

//-------------------------
//linked list
//-------------------------

TradingCard* TradingCard::SetNext(TradingCard* n) {
	return next = n;
}

TradingCard* TradingCard::GetNext() {
	return next;
}
