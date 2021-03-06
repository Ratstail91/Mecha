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

#include "card_saver.hpp"
#include "render_text_texture.hpp"
#include "render_trading_card.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

MockupScene::MockupScene() {
	//card frame textures
	textureLoader.Load(GetRenderer(), "rsc/cardframes/", "Basic Tower.png");
	textureLoader.Load(GetRenderer(), "rsc/cardframes/", "Command.png");
	textureLoader.Load(GetRenderer(), "rsc/cardframes/", "Mecha Tower.png");
	textureLoader.Load(GetRenderer(), "rsc/cardframes/", "Mecha.png");
	textureLoader.Load(GetRenderer(), "rsc/cardframes/", "Singleton.png");
	textureLoader.Load(GetRenderer(), "rsc/cardframes/", "Trigger Command.png");

	//card art textures
	//#Format 3: name;const;type;text;power;durability
	CSVObject<6> cardCSV = readCSV<6>("rsc/base_set.csv", ';');
	for (auto& it : cardCSV) {
		textureLoader.Load(GetRenderer(), "rsc/art/", it[0] + ".png");
	}

	//fonts
	headerFont = TTF_OpenFont("rsc/fonts/belerensmallcaps-bold-webfont.ttf", 24);
	textFont = TTF_OpenFont("rsc/fonts/beleren-bold-webfont.ttf", 16);

	//check that the font loaded
	if (!headerFont || !textFont) {
		std::ostringstream msg;
		msg << "Failed to load a font file; " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	RenderList();
}

MockupScene::~MockupScene() {
	//cleanup
	while(cardMasterList.Peek()) {
		delete cardMasterList.Pop();
	}
	TTF_CloseFont(headerFont);
	TTF_CloseFont(textFont);
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
	//
}

void MockupScene::KeyDown(SDL_KeyboardEvent const& event) {
	//calc the size of the list
	TradingCard* card = cardMasterList.Peek(0);
	int size = 0;
	while (card) {
		size++;
		card = card->GetNext();
	}

	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
		break;
		case SDLK_UP:
			if (selection < size-1) selection++;
		break;
		case SDLK_DOWN:
			if (selection > 0) selection--;
		break;
		case SDLK_r:
		case SDLK_SPACE:
			RenderList();
		break;
		case SDLK_s:
			RenderEach();
		break;
		case SDLK_a:
			RenderAll();
		break;
	}
	std::cout << selection << std::endl;
}

void MockupScene::KeyUp(SDL_KeyboardEvent const& event) {
	//
}

//-------------------------
//window events
//-------------------------

void MockupScene::WindowResized(SDL_WindowEvent const& event) {
	//BUGFIX: There seems to be an issue when resizing the screen
	for (TradingCard* it = cardMasterList.Peek(); it != nullptr; it = it->GetNext()) {
		renderTradingCard(GetRenderer(), it, headerFont, textFont);
	}
}

//-------------------------
//Render functions
//-------------------------

void MockupScene::RenderList() {
	//clear the existing card data
	while(cardMasterList.Peek()) {
		delete cardMasterList.Pop();
	}

	//load the card data
	//#Format 3: name;const;type;text;power;durability
	CSVObject<6> cardCSV = readCSV<6>("rsc/base_set.csv", ';');

	for (auto& it : cardCSV) {
		std::cout << "Rendering: " << it[0] << "\t\t\t";
		TradingCardTypes types;
		types.Parse(it[2]);
		std::cout << types.Stringify() << std::endl;

		//make and set the new card
		TradingCard* floatingCard = new TradingCard();
		floatingCard->SetName(it[0]);
		floatingCard->SetCost(std::atoi(it[1].c_str()));
		floatingCard->SetTypes(types);
		floatingCard->SetText(it[3]);
		floatingCard->SetPower(std::atoi(it[4].c_str()));
		floatingCard->SetDurability(std::atoi(it[5].c_str()));

		renderTradingCard(GetRenderer(), floatingCard, headerFont, textFont);

		cardMasterList.Push(floatingCard);
	}
}

void MockupScene::RenderEach() {
	for (TradingCard* it = cardMasterList.Peek(); it; it = it->GetNext()) {
		saveCard(GetRenderer(), it);
	}
}

void MockupScene::RenderAll() {
	saveCardArray(GetRenderer(), cardMasterList);
}