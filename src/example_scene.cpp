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
#include "example_scene.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

ExampleScene::ExampleScene() {
	//load the resource files
	textureLoader.Load(GetRenderer(), "rsc\\", "tower.png");

	textureLoader.Load(GetRenderer(), "rsc\\", "mecha.png");
	textureLoader.Load(GetRenderer(), "rsc\\", "command.png");
	textureLoader.Load(GetRenderer(), "rsc\\", "back.png");
	textureLoader.Load(GetRenderer(), "rsc\\", "button_back_30.png");

	buttonFont = TTF_OpenFont("rsc\\coolvetica rg.ttf", 12);

	//check that the font loaded
	if (!buttonFont) {
		std::ostringstream msg;
		msg << "Failed to load a font file; " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	//setup the members
	battlefield.SetCardW(100);
	battlefield.SetCardH(100);

	backImage.SetTexture(textureLoader.Find("back.png"));

	//reused code
	SetupButtons();

	//DEBUG
	//...
}

ExampleScene::~ExampleScene() {
	//
}

//-------------------------
//frame phases
//-------------------------

void ExampleScene::FrameStart() {
	//
}

void ExampleScene::Update() {
	//
}

void ExampleScene::FrameEnd() {
	//
}

void ExampleScene::RenderFrame(SDL_Renderer* renderer) {
	//NOTE: Magic numbers
	backImage.DrawTo(renderer, battlefield.GetX(), battlefield.GetY(), 10.0/45.0, 10.0/45.0);
	battlefield.DrawTo(renderer, 0, 0, 1);

	handOneButton.DrawTo(renderer);
	handTwoButton.DrawTo(renderer);
	hideButton.DrawTo(renderer);

	switch(handState) {
		case HandState::PLAYER_ONE:
			RenderHand(renderer, &playerOneHand);
		break;
		case HandState::PLAYER_TWO:
			RenderHand(renderer, &playerTwoHand);
		break;
	}
}

//-------------------------
//input events
//-------------------------

void ExampleScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	handOneButton.MouseMotion(event);
	handTwoButton.MouseMotion(event);
	hideButton.MouseMotion(event);
}

void ExampleScene::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	int b = 0;
	b |= handOneButton.MouseButtonDown(event);
	b |= handTwoButton.MouseButtonDown(event);
	b |= hideButton.MouseButtonDown(event);

	//left button control
	if (event.button != SDL_BUTTON_LEFT || b & Button::State::PRESSED) {
		return;
	}

	//BUGFIX: off-by-one error caused by dividing negatives
	int clickX = event.x - battlefield.GetX();
	int clickY = event.y - battlefield.GetY();
	if (clickX < 0 || clickY < 0 ||
		clickX >= Battlefield::BATTLEFIELD_WIDTH * battlefield.GetCardW() ||
		clickY >= Battlefield::BATTLEFIELD_HEIGHT * battlefield.GetCardH()
		) {
		return;
	}

	int slotX = clickX / battlefield.GetCardW();
	int slotY = clickY / battlefield.GetCardH();

	std::cout << slotX << "\t" << slotY << "\t" << std::endl;

	if (battlefield.FindCard(slotX, slotY) == nullptr) {
		//TODO: push
	}
	else {
		//TODO: pop
	}
}

void ExampleScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	if (handOneButton.MouseButtonUp(event) == Button::State::RELEASED) {
		handState = HandState::PLAYER_ONE;
	}
	if (handTwoButton.MouseButtonUp(event) == Button::State::RELEASED) {
		handState = HandState::PLAYER_TWO;
	}
	if (hideButton.MouseButtonUp(event) == Button::State::RELEASED) {
		handState = HandState::HIDE;
	}
}

void ExampleScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	//
}

void ExampleScene::KeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
		break;
	}
}

void ExampleScene::KeyUp(SDL_KeyboardEvent const& event) {
	//
}

//-------------------------
//window events
//-------------------------

void ExampleScene::WindowResized(SDL_WindowEvent const& event) {
	//BUGFIX: There seems to be an issue when resizing the screen
	SetupButtons();
}

//-------------------------
//utility methods
//-------------------------

void ExampleScene::RenderHand(SDL_Renderer* const renderer, TradingCardList* hand) {
	//don't render an empty hand
	if (!hand || hand->Peek() == nullptr) {
		return;
	}

	//get the screen size
	int screenW, screenH;
	SDL_GetRendererOutputSize(renderer, &screenW, &screenH);

	//where to draw the hand
	constexpr int increment = 40;
	constexpr double scale = 0.5;
	int destX = screenW - hand->Peek()->GetImage()->GetClipW() * scale;
	int destY = increment;

	for (TradingCard* it = hand->Peek(); it != nullptr; it = it->GetNext()) {
		it->GetImage()->DrawTo(renderer, destX, destY, scale, scale);
		destY += increment;
	}
}

void ExampleScene::SetupButtons() {
	//setup the buttons
	handOneButton.SetBackgroundTexture(GetRenderer(), textureLoader.Find("button_back_30.png"));
	handTwoButton.SetBackgroundTexture(GetRenderer(), textureLoader.Find("button_back_30.png"));
	hideButton.SetBackgroundTexture(GetRenderer(), textureLoader.Find("button_back_30.png"));

	handOneButton.SetText(GetRenderer(), buttonFont, {255, 255, 255, 255}, "Player One");
	handTwoButton.SetText(GetRenderer(), buttonFont, {255, 255, 255, 255}, "Player Two");
	hideButton.SetText(GetRenderer(), buttonFont, {255, 255, 255, 255}, "Hide");

	//adjust to any changes in screen size
	int w, h;
	SDL_GetRendererOutputSize(GetRenderer(), &w, &h);

	handOneButton.SetX(w - 3 * handOneButton.GetImage()->GetClipW());
	handTwoButton.SetX(w - 2 * handTwoButton.GetImage()->GetClipW());
	hideButton.SetX(w - 1 * hideButton.GetImage()->GetClipW());

	handOneButton.SetY(0);
	handTwoButton.SetY(0);
	hideButton.SetY(0);
}