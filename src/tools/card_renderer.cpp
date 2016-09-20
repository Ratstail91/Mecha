#include "card_list.hpp"
#include "card_shuffler.hpp"
#include "card_sorter.hpp"
#include "csv_tool.hpp"
#include "render_trading_card.hpp"
#include "texture_loader.hpp"
#include "trading_card.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <iostream>
#include <sstream>

typedef CardList<TradingCard, NullSorter<TradingCard>, CardShuffler<TradingCard>> CardListType;

enum class Mode {
	BASIC,
	ARRAY
}mode;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

extern int stricmp(const char*, const char*);
extern char* itoa(int value, char* str, int base);

//-------------------------
//specialized card saving utilities
//-------------------------

void saveCardImage(SDL_Renderer* const renderer, TradingCard* card) {
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

void saveCardArray(SDL_Renderer* const, CardListType cardList) {
	TradingCard* masterIterator = cardList.Peek();
	int nameCounter = 0;

	//iterate over cards six at a time
	while(masterIterator) {
		//get a pointer to each of the six cards
		TradingCard *elements[6];

		for (int i = 0; i < 6; i++) {
			elements[i] = masterIterator;
			if (masterIterator) {
				masterIterator = masterIterator->GetNext();
			}
		}

		//Create an empty RGB surface that will be used to create the screenshot bmp file
		SDL_Surface* targetSurface = SDL_CreateRGBSurface(0, elements[0]->GetImage()->GetClipW()*2, elements[0]->GetImage()->GetClipH()*3, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

		if (!targetSurface) {
			throw(std::runtime_error("saveCardArray error"));
		}

		//surface pointer
		SDL_Surface* srcSurface = nullptr;
		SDL_Rect srcRect = {0, 0, 450, 450};
		SDL_Rect dstRect = {0, 0, 450, 450};

		//each element in it's own place
		if (elements[0]) {
			srcSurface = makeSurfaceFromCard(renderer, elements[0]);
			dstRect = {0, 0, 450, 450};
			SDL_BlitSurface(srcSurface, &srcRect, targetSurface, &dstRect);
			SDL_FreeSurface(srcSurface);
		}

		if (elements[1]) {
			srcSurface = makeSurfaceFromCard(renderer, elements[1]);
			dstRect = {450, 0, 450, 450};
			SDL_BlitSurface(srcSurface, &srcRect, targetSurface, &dstRect);
			SDL_FreeSurface(srcSurface);
		}

		if (elements[2]) {
			srcSurface = makeSurfaceFromCard(renderer, elements[2]);
			dstRect = {0, 450, 450, 450};
			SDL_BlitSurface(srcSurface, &srcRect, targetSurface, &dstRect);
			SDL_FreeSurface(srcSurface);
		}

		if (elements[3]) {
			srcSurface = makeSurfaceFromCard(renderer, elements[3]);
			dstRect = {450, 450, 450, 450};
			SDL_BlitSurface(srcSurface, &srcRect, targetSurface, &dstRect);
			SDL_FreeSurface(srcSurface);
		}

		if (elements[4]) {
			srcSurface = makeSurfaceFromCard(renderer, elements[4]);
			dstRect = {0, 900, 450, 450};
			SDL_BlitSurface(srcSurface, &srcRect, targetSurface, &dstRect);
			SDL_FreeSurface(srcSurface);
		}

		if (elements[5]) {
			srcSurface = makeSurfaceFromCard(renderer, elements[5]);
			dstRect = {450, 900, 450, 450};
			SDL_BlitSurface(srcSurface, &srcRect, targetSurface, &dstRect);
			SDL_FreeSurface(srcSurface);
		}

		char nameBuffer[32];
		SDL_SaveBMP(targetSurface, (std::string(itoa(nameCounter, nameBuffer, 10)) + ".bmp").c_str());
		nameCounter++;

		SDL_FreeSurface(targetSurface);
	}
}

//-------------------------
//Main parts of the program
//-------------------------

void init(int argc, char** argv) {
	//create the singletons
	TextureLoader::CreateSingleton();

	//create and check the window
	int screenWidth = 800;
	int screenHeight = 600;

	window = SDL_CreateWindow(
		"Example Caption",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth,
		screenHeight,
		SDL_WINDOW_RESIZABLE);

	if (!window) {
		std::ostringstream msg;
		msg << "Failed to create the window: " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	//create and check the renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		std::ostringstream msg;
		msg << "Failed to create the renderer: " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	//screen scaling
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);

	//setting up SDL2_ttf
	if (TTF_Init()) {
		std::ostringstream msg;
		msg << "Failed to initialize SDL_ttf 2.0: " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}
}

void proc(int argc, char** argv) {
	//textures
	TextureLoader& textureLoader = TextureLoader::GetSingleton();

	textureLoader.Load(renderer, "rsc\\", "tower.png");
	textureLoader.Load(renderer, "rsc\\", "mecha.png");
	textureLoader.Load(renderer, "rsc\\", "command.png");

	//fonts
	TTF_Font* headerFont = TTF_OpenFont("rsc\\belerensmallcaps-bold-webfont.ttf", 24);
	TTF_Font* textFont = TTF_OpenFont("rsc\\beleren-bold-webfont.ttf", 16);

	//check that the font loaded
	if (!headerFont || !textFont) {
		std::ostringstream msg;
		msg << "Failed to load a font file; " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	//load the card data
	//#Format 2: rarity;name;type;cost;power;durability;text
	CSVObject<7> cardCSV = readCSV<7>("rsc/base_set.csv", ';');

	//create the storage container for mode ARRAY
	CardListType cardList;

	//create each card
	for (auto& it : cardCSV) {
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

		renderTradingCard(renderer, floatingCard, headerFont, textFont);

		cardList.Push(floatingCard);
	}

	//process each card based on mode
	switch(mode) {
		case Mode::BASIC:
			for (TradingCard* floatingCard = cardList.Peek(); floatingCard; floatingCard = floatingCard->GetNext()) {
				saveCardImage(renderer, floatingCard);
			}
		break;
		case Mode::ARRAY:
			saveCardArray(renderer, cardList);
		break;
	}

	//clean up
	while (cardList.Peek()) {
		delete cardList.Pop();
	}

	TTF_CloseFont(headerFont);
	TTF_CloseFont(textFont);
}

void quit(int argc, char** argv) {
	//close the APIs
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	//destroy the singletons
	TextureLoader::DeleteSingleton();
}

int main(int argc, char** argv) {
	std::cout << "Beginning " << argv[0] << std::endl;

	if (argc == 1) {
		mode = Mode::BASIC;
	}
	else {
		if (!stricmp(argv[1], "array")) {
			mode = Mode::ARRAY;
		}
	}
	try {
		//open the application
		init(argc, argv);

		//run the main body
		proc(argc, argv);

		//close the application
		quit(argc, argv);
	}
	catch(std::exception& e) {
		std::cerr << "Fatal Error: " << e.what() << std::endl;
		return 1;
	}
	std::cout << "Clean exit from " << argv[0] << std::endl;
	return 0;
}