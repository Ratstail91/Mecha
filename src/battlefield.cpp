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
#include "battlefield.hpp"

#include <stdexcept>

Battlefield::Battlefield() {
	fieldArray = new TradingCard**[9];
	for (int i = 0; i < 9; i++) {
		fieldArray[i] = new TradingCard*[9];
		for (int j = 0; j < 9; j++) {
			fieldArray[i][j] = nullptr;
		}
	}
}

Battlefield::~Battlefield() {
	//prevent memory leaks
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			delete fieldArray[i][j];
		}
		delete fieldArray[i];
	}
	delete fieldArray;
}

void Battlefield::DrawTo(SDL_Renderer* const renderer, int camX, int camY, double zoom) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (fieldArray[i][j]) {
				fieldArray[i][j]->GetImage()->DrawTo(
					renderer,
					posX - camX + i * cardW * zoom,
					posY - camY + j * cardH * zoom,
					10.0/45.0 * zoom,
					10.0/45.0 * zoom
					);
			}
		}
	}
}

//-------------------------
//interact with the field
//-------------------------

TradingCard* Battlefield::PushCard(TradingCard* const card, int x, int y) {
	if (x < 0 || x >= 9 || y < 0 || y >= 9) {
		throw(std::out_of_range("Field space out of range"));
	}
	if (fieldArray[x][y] != nullptr) {
		throw(std::runtime_error("Field space taken"));
	}
	return fieldArray[x][y] = card;
}

TradingCard* Battlefield::FindCard(int x, int y) {
	if (x < 0 || x >= 9 || y < 0 || y >= 9) {
		throw(std::out_of_range("Field space out of range"));
	}
	return fieldArray[x][y];
}

TradingCard* Battlefield::PopCard(int x, int y) {
	if (x < 0 || x >= 9 || y < 0 || y >= 9) {
		throw(std::out_of_range("Field space out of range"));
	}
	TradingCard* ret = fieldArray[x][y];
	fieldArray[x][y] = nullptr;
	return ret;
}

//-------------------------
//accessors & mutators
//-------------------------

int Battlefield::SetX(int i) {
	return posX = i;
}

int Battlefield::SetY(int i) {
	return posY = i;
}

int Battlefield::SetCardW(int i) {
	return cardW = i;
}

int Battlefield::SetCardH(int i) {
	return cardH = i;
}

int Battlefield::GetX() const {
	return posX;
}

int Battlefield::GetY() const {
	return posY;
}

int Battlefield::GetCardW() const {
	return cardW;
}

int Battlefield::GetCardH() const {
	return cardH;
}
