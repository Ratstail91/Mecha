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
#include <sstream>
#include <stdexcept>

//utility functions
#ifndef itoa
char* itoa(int value, char* str, int base) {
	//ignore base
	sprintf(str, "%d", value);
	return str;

}
#endif

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

void renderTradingCard(SDL_Renderer* const renderer, TradingCard* card, TTF_Font* headerFont, TTF_Font* textFont) {
	//TODO: image file

	//singleton
	TextureLoader& textureLoader = TextureLoader::GetSingleton();

	//background image
	SDL_Texture* backTexture = nullptr;

	const TradingCardTypes types = card->GetTypes();

	//forget singleton overlay for now
	if (types.GetBasic() && types.GetTower()) {
		backTexture = textureLoader.Find("Basic Tower.png");
	}
	if (types.GetMecha() && types.GetTower()) {
		backTexture = textureLoader.Find("Mecha Tower.png");
	}

	if (types.GetTrigger() && types.GetCommand()) {
		backTexture = textureLoader.Find("Trigger Command.png");
	}
	if (types.GetCommand() && !types.GetTrigger()) {
		backTexture = textureLoader.Find("Command.png");
	}

	if (types.GetMecha() && !types.GetTower()) {
		backTexture = textureLoader.Find("Mecha.png");
	}

	if (backTexture == nullptr) {
		throw(std::runtime_error("Could not find needed textures in renderTradingCard"));
	}

	//prep the image for rendering
	int w = 0, h = 0;
	SDL_QueryTexture(backTexture, nullptr, nullptr, &w, &h);
	card->GetImage()->Create(renderer, w, h);
	SDL_SetRenderTarget(renderer, card->GetImage()->GetTexture());

	//render each component

	//background
	SDL_RenderCopy(renderer, backTexture, nullptr, nullptr);

	//singleton overlay
	if (types.GetSingleton()) {
		SDL_RenderCopy(renderer, textureLoader.Find("Singleton.png"), nullptr, nullptr);
	}

	//name
	renderTextDirect(renderer, headerFont, {0, 0, 0, 255}, card->GetName(), 60, 10);

	if (types.GetMecha()) {
		char tmpcstr[16];

		//cost, power, durability
		renderTextDirect(renderer, headerFont, {0, 0, 0, 255}, itoa(card->GetCost(), tmpcstr, 10), 15, 10);
		renderTextDirect(renderer, headerFont, {0, 0, 0, 255}, itoa(card->GetPower(), tmpcstr, 10), 15, 45);
		renderTextDirect(renderer, headerFont, {0, 0, 0, 255}, itoa(card->GetDurability(), tmpcstr, 10), 15, 70);
	}

	//type line
	if (!types.GetBasic()) {
		renderTextDirect(renderer, textFont, {0, 0, 0, 255}, types.Stringify(), 20, 295);
	}
	else {
		renderTextDirect(renderer, textFont, {0, 0, 0, 255}, types.Stringify(), 20, 495);
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
		renderTextDirect(renderer, textFont, {0, 0, 0, 255}, it, 20, 325 + increment * 16);
		increment++;
	}

	//cleanup
	SDL_SetRenderTarget(renderer, nullptr);
}
