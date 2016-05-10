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
#include "render_trading_card.hpp"

#include "render_text_texture.hpp"
#include "texture_loader.hpp"

#include <cstring>
#include <list>
#include <stdexcept>

#ifndef itoa
char* itoa(int value, char* str, int base) {
	//ignore base
	sprintf(str, "%d", value);
	return str;

}
#endif

void renderTradingCard(SDL_Renderer* const renderer, TradingCard* card, TTF_Font* headerFont, TTF_Font* textFont) {
	//singleton
	TextureLoader& textureLoader = TextureLoader::GetSingleton();

	//background image
	SDL_Texture* backTexture = nullptr;

	switch(card->GetType()) {
		case TradingCard::Type::BASIC_TOWER:
		case TradingCard::Type::TOWER:
			backTexture = textureLoader.Find("tower.png");
		break;
		case TradingCard::Type::MECHA:
		case TradingCard::Type::MECHA_TOWER:
			backTexture = textureLoader.Find("mecha.png");
		break;
		case TradingCard::Type::COMMAND:
			backTexture = textureLoader.Find("command.png");
		break;
	}

	if (backTexture == nullptr) {
		throw(std::runtime_error("Could not find needed textures in renderTradingCard"));
	}

	//prep the image for rendering
	int w = 0, h = 0;
	SDL_QueryTexture(backTexture, nullptr, nullptr, &w, &h);
	card->GetImage()->Create(renderer, w, h);
	SDL_SetRenderTarget(renderer, card->GetImage()->GetTexture());

	//variables
	SDL_Texture* tmpPtr = nullptr;
	SDL_Rect dclip;

	//render each component

	//background
	SDL_RenderCopy(renderer, backTexture, nullptr, nullptr);

	//name
	renderTextDirect(renderer, headerFont, {255, 255, 255, 255}, card->GetName(), 20, 15);

	if (card->GetType() == TradingCard::Type::MECHA || card->GetType() == TradingCard::Type::MECHA_TOWER) {
		char tmpcstr[16];

		//cost, power, durability
		renderTextDirect(renderer, headerFont, {255, 255, 255, 255}, itoa(card->GetCost(), tmpcstr, 10), 25, 55);
		renderTextDirect(renderer, headerFont, {255, 255, 255, 255}, itoa(card->GetPower(), tmpcstr, 10), 25, 55+24);
		renderTextDirect(renderer, headerFont, {255, 255, 255, 255}, itoa(card->GetDurability(), tmpcstr, 10), 25, 55+48);
	}

	//text, splitting the string
	char buffer[512];
	std::list<std::string> linesList;
	strcpy(buffer, card->GetText().c_str());
	const char* cstr = strtok(buffer, "/");
	do {
		linesList.push_back(cstr);
	} while(cstr = strtok(nullptr, "/"));

	int increment = 0;
	for (auto& it : linesList) {
		renderTextDirect(renderer, textFont, {0, 0, 0, 255}, it, 20, 260 + increment * 16);
		increment++;
	}

	//cleanup
	SDL_SetRenderTarget(renderer, nullptr);
}