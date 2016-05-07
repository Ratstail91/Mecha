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
	textureLoader.Load(GetRenderer(), "rsc\\", "button_blue.png");
	textureLoader.Load(GetRenderer(), "rsc\\", "button_back_30.png");

	font = TTF_OpenFont("rsc\\coolvetica rg.ttf", 12);

	//check that the font loaded
	if (!font) {
		std::ostringstream msg;
		msg << "Failed to load a font file; " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	//setup the members
	battlefield.SetCardW(100);
	battlefield.SetCardH(100);

	backImage.SetTexture(textureLoader.Find("back.png"));

	tmpCard.GetImage()->SetTexture(textureLoader.Find("mecha.png"));

	//setup the buttons
	handOneButton.SetBackgroundTexture(GetRenderer(), textureLoader.Find("button_back_30.png"));
	handTwoButton.SetBackgroundTexture(GetRenderer(), textureLoader.Find("button_back_30.png"));
	hideButton.SetBackgroundTexture(GetRenderer(), textureLoader.Find("button_back_30.png"));

	handOneButton.SetText(GetRenderer(), font, {255, 255, 255, 255}, "Player One");
	handTwoButton.SetText(GetRenderer(), font, {255, 255, 255, 255}, "Player Two");
	hideButton.SetText(GetRenderer(), font, {255, 255, 255, 255}, "Hide");
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
	backImage.DrawTo(renderer, battlefield.GetX() - camera.x, battlefield.GetY() - camera.y, camera.zoom * 10.0/45.0, camera.zoom * 10.0/45.0);
	battlefield.DrawTo(renderer, camera.x, camera.y, camera.zoom);

	//adjust to any changes in screen size
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);

	handOneButton.SetX(w - 3 * handOneButton.GetImage()->GetClipW());
	handOneButton.DrawTo(renderer);
	handTwoButton.SetX(w - 2 * handTwoButton.GetImage()->GetClipW());
	handTwoButton.DrawTo(renderer);
	hideButton.SetX(w - 1 * hideButton.GetImage()->GetClipW());
	hideButton.DrawTo(renderer);
}

//-------------------------
//input events
//-------------------------

void ExampleScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	handOneButton.MouseMotion(event);
	handTwoButton.MouseMotion(event);
	hideButton.MouseMotion(event);

	if (event.state & SDL_BUTTON_RMASK) {
		camera.x -= event.xrel;
		camera.y -= event.yrel;
	}
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
	int clickX = event.x - battlefield.GetX() + camera.x;
	int clickY = event.y - battlefield.GetY() + camera.y;
	if (clickX < 0 || clickY < 0) {
		return;
	}

	int slotX = clickX / camera.zoom / battlefield.GetCardW();
	int slotY = clickY / camera.zoom / battlefield.GetCardH();

	std::cout << slotX << "\t" << slotY << "\t";

	if (slotX < 0 || slotX >= 9 || slotY < 0 || slotY >= 9) {
		std::cout << std::endl;
		return;
	}

	if (battlefield.FindCard(slotX, slotY) == nullptr) {
		std::cout << "Push" << std::endl;
		battlefield.PushCard(&tmpCard, slotX, slotY);
	}
	else {
		std::cout << "Pop" << std::endl;
		battlefield.PopCard(slotX, slotY);
	}
}

void ExampleScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	handOneButton.MouseButtonUp(event);
	handTwoButton.MouseButtonUp(event);
	hideButton.MouseButtonUp(event);
}

void ExampleScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	if (event.y < 0) {
		camera.zoom /= 1.1;
	}
	if (event.y > 0) {
		camera.zoom *= 1.1;
	}

	//set min/max
	if (camera.zoom < 0.5) {
		camera.zoom = 0.5;
	}
	if (camera.zoom > 3.0) {
		camera.zoom = 3.0;
	}
}

void ExampleScene::KeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
		break;

		case SDLK_SPACE:
			//reset the camera
			camera.x = 0;
			camera.y = 0;
			camera.zoom = 1.0;
		break;
	}
}

void ExampleScene::KeyUp(SDL_KeyboardEvent const& event) {
	//
}
