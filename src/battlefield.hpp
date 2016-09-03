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

#include "trading_card.hpp"

#include <array>

class Battlefield {
public:
	static constexpr int BATTLEFIELD_WIDTH = 5;
	static constexpr int BATTLEFIELD_HEIGHT = 5;

	Battlefield();
	~Battlefield();

	void DrawTo(SDL_Renderer* const, int camX, int camY, double zoom);

	//interact with the field
	TradingCard* PushCard(TradingCard* const, int x, int y);
	TradingCard* FindCard(int x, int y);
	TradingCard* PopCard(int x, int y);

	//accessors & mutators
	int SetX(int i);
	int SetY(int i);
	int SetCardW(int i);
	int SetCardH(int i);

	int GetX() const;
	int GetY() const;
	int GetCardW() const;
	int GetCardH() const;

private:
	TradingCard*** fieldArray;
	int posX = 0;
	int posY = 0;
	int cardW = 0;
	int cardH = 0;
};