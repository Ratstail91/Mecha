#include "csv_tool.hpp"
#include "render_trading_card.hpp"
#include "texture_loader.hpp"


#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <iostream>
#include <sstream>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void init() {
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

void proc() {
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

		//NOTE: img/ directory must already exist
		saveCardImage(renderer, floatingCard, (std::string() + "img/" + floatingCard->GetName() + ".bmp").c_str());

		delete floatingCard;
	}

	TTF_CloseFont(headerFont);
	TTF_CloseFont(textFont);
}

void quit() {
	//close the APIs
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	//destroy the singletons
	TextureLoader::DeleteSingleton();
}

int main(int argc, char** argv) {
	std::cout << "Beginning " << argv[0] << std::endl;
	try {
		//open the application
		init();

		//run the main body
		proc();

		//close the application
		quit();
	}
	catch(std::exception& e) {
		std::cerr << "Fatal Error: " << e.what() << std::endl;
		return 1;
	}
	std::cout << "Clean exit from " << argv[0] << std::endl;
	return 0;
}