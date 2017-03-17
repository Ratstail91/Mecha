/* Copyright: (c) Kayne Ruse 2017
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
#include "card_saver.hpp"

void saveCard(SDL_Renderer* const renderer, TradingCard* card) {
	//point to the image
	SDL_SetRenderTarget(renderer, card->GetImage()->GetTexture());

	//Create an empty RGB surface that will be used to create the screenshot bmp file
	SDL_Surface* surface = SDL_CreateRGBSurface(0, card->GetImage()->GetClipW(), card->GetImage()->GetClipH(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	if(surface) {
		//Read the pixels from the current render target and save them onto the surface
		SDL_RenderReadPixels(renderer, NULL, 0, surface->pixels, surface->pitch);

		//Create the bmp screenshot file
		SDL_SaveBMP(surface, (std::string() + "img/" + card->GetName() + ".bmp").c_str());

		//Destroy the screenshot surface
		SDL_FreeSurface(surface);
	}

	//cleanup
	SDL_SetRenderTarget(renderer, nullptr);
}

SDL_Surface* makeSurfaceFromCard(SDL_Renderer* const renderer, TradingCard* card) {
	//point to the image
	SDL_SetRenderTarget(renderer, card->GetImage()->GetTexture());

	//Create an empty RGB surface that will be used to create the screenshot bmp file
	SDL_Surface* surface = SDL_CreateRGBSurface(0, card->GetImage()->GetClipW(), card->GetImage()->GetClipH(), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	if(surface) {
		//Read the pixels from the current render target and save them onto the surface
		SDL_RenderReadPixels(renderer, NULL, 0, surface->pixels, surface->pitch);
	}

	//cleanup
	SDL_SetRenderTarget(renderer, nullptr);

	return surface;
}

//a custom written blit function, because the built-in one won't work for some reason
//TODO: Why did I write this? remove it.
void customBlit(SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dst, SDL_Rect* dstRect) {
	SDL_LockSurface(src);
	SDL_LockSurface(dst);

	//iterate over each pixel in src
	for (int i = srcRect->x; i < srcRect->w; i++) {
		for (int j = srcRect->y; j < srcRect->h; j++) {
			((int*)dst->pixels)[j*dst->w + i + dst->w*dstRect->y + dstRect->x] = ((int*)src->pixels)[j*src->w + i];
		}
	}

	SDL_UnlockSurface(src);
	SDL_UnlockSurface(dst);
}

#undef SDL_BlitSurface
#define SDL_BlitSurface customBlit


void saveCardArray(SDL_Renderer* const renderer, CardList<TradingCard, NullSorter<TradingCard>, CardShuffler<TradingCard>> cardList) {
	//calculate the width & height of the dest surface
	int w = cardList.Peek()->GetImage()->GetClipW();
	int h = cardList.Peek()->GetImage()->GetClipH();
	int length = 0;
	for (TradingCard* it = cardList.Peek(); it; it = it->GetNext()) {
		length++;
	}

	//10 cards wide
	w *= 10;
	h *= (length / 10 + (length % 10 ? 1:0));

	SDL_Surface* destSurface = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	//render each card in place
	TradingCard* it = cardList.Peek();
	for (int j = 0; it; j++) {
		for (int i = 0; i < 10 && it; i++) {
			//render the card in place
			SDL_Surface* cardSurface = makeSurfaceFromCard(renderer, it);
			SDL_Rect src = {
				0, 0, cardList.Peek()->GetImage()->GetClipW(), cardList.Peek()->GetImage()->GetClipH()
			};
			SDL_Rect dst = {
				cardList.Peek()->GetImage()->GetClipW() * i,
				cardList.Peek()->GetImage()->GetClipH() * j,
				cardList.Peek()->GetImage()->GetClipW(),
				cardList.Peek()->GetImage()->GetClipH()
			};
			SDL_BlitSurface(cardSurface, &src, destSurface, &dst);

			//iterate the card
			it = it->GetNext();
		}
	}

	SDL_SaveBMP(destSurface, "img/array.bmp");

	SDL_FreeSurface(destSurface);
}
