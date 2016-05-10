/* Copyright: (c) Kayne Ruse 2013-2016
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
#include "mockup_scene.hpp"

#include "render_text_texture.hpp"
#include "render_trading_card.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

//utility functions
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
	if (!stricmp(s.c_str(), "basic tower")) return TradingCard::Type::BASIC_TOWER;
	if (!stricmp(s.c_str(), "mecha")) return TradingCard::Type::MECHA;
	if (!stricmp(s.c_str(), "command")) return TradingCard::Type::COMMAND;
	if (!stricmp(s.c_str(), "tower")) return TradingCard::Type::TOWER;
	if (!stricmp(s.c_str(), "mecha tower")) return TradingCard::Type::MECHA_TOWER;
	std::ostringstream msg;
	msg << "Failed to read the card type: " << s;
	throw(std::runtime_error(msg.str()));
}

MockupScene::MockupScene() {
	//textures
	textureLoader.Load(GetRenderer(), "rsc\\", "tower.png");
	textureLoader.Load(GetRenderer(), "rsc\\", "mecha.png");
	textureLoader.Load(GetRenderer(), "rsc\\", "command.png");

	//fonts
	headerFont = TTF_OpenFont("rsc\\belerensmallcaps-bold-webfont.ttf", 24);
	textFont = TTF_OpenFont("rsc\\beleren-bold-webfont.ttf", 16);

	//check that the font loaded
	if (!headerFont || !textFont) {
		std::ostringstream msg;
		msg << "Failed to load a font file; " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	//load the card data
	CSVObject<6> cardCSV = readCSV<6>("rsc/base_set.csv", ';');

	for (auto& it : cardCSV) {
		std::cout << it[0] << std::endl;

		//make and set the new card
		TradingCard* floatingCard = new TradingCard();
		floatingCard->SetName(it[0]);
		floatingCard->SetType(readType(it[1]));
		floatingCard->SetCost(std::atoi(it[2].c_str()));
		floatingCard->SetText(it[3]);
		floatingCard->SetPower(std::atoi(it[4].c_str()));
		floatingCard->SetDurability(std::atoi(it[5].c_str()));

		renderTradingCard(GetRenderer(), floatingCard, headerFont, textFont);

		cardMasterList.Push(floatingCard);
	}
}

MockupScene::~MockupScene() {
	//cleanup
	while(cardMasterList.Peek()) {
		delete cardMasterList.Pop();
	}
}

//-------------------------
//frame phases
//-------------------------

void MockupScene::FrameStart() {
	//
}

void MockupScene::Update() {
	//
}

void MockupScene::FrameEnd() {
	//
}

void MockupScene::RenderFrame(SDL_Renderer* renderer) {
	//debug
	TradingCard* card = cardMasterList.Peek(selection);

	if (card) {
		card->GetImage()->DrawTo(renderer, 0, 0);
	}
}

//-------------------------
//input events
//-------------------------

void MockupScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	//
}

void MockupScene::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	//
}

void MockupScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	//
}

void MockupScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	if (event.y < 0 && selection > 0) {
		selection--;
	}
	if (event.y > 0 && selection < 24) {
		selection++;
	}
	std::cout << selection << std::endl;
}

void MockupScene::KeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
		break;
	}
}

void MockupScene::KeyUp(SDL_KeyboardEvent const& event) {
	//
}
