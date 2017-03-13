/* Copyright: (c) Kayne Ruse 2016-2017
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
#include "trading_card_types.hpp"

#include <algorithm>

TradingCardTypes TradingCardTypes::Parse(std::string s) {
	//convert s to lower case
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);

	if (s.find("basic") != std::string::npos) {
		basic = true;
	}
	if (s.find("singleton") != std::string::npos) {
		singleton = true;
	}
	if (s.find("trigger") != std::string::npos) {
		trigger = true;
	}
	if (s.find("command") != std::string::npos) {
		command = true;
	}
	if (s.find("mecha") != std::string::npos) {
		mecha = true;
	}
	if (s.find("tower") != std::string::npos) {
		tower = true;
	}
	return *this;
}

std::string TradingCardTypes::Stringify() const {
	int count = 0;
	std::string ret;

	//basic, singleton and trigger are mutually exclusive, normally
	if (basic) {
		ret += "Basic";
		count++;
	}
	if (singleton) {
		ret += "Singleton";
		count++;
	}
	if (trigger) {
		ret += "Trigger";
		count++;
	}

	if (command) {
		if (count) {
			ret += ' ';
		}
		ret += "Command";
		count++;
	}
	if (mecha) {
		if (count) {
			ret += ' ';
		}
		ret += "Mecha";
		count++;
	}
	if (tower) {
		if (count) {
			ret += ' ';
		}
		ret += "Tower";
		count++;
	}
	return ret;
}

//accessors & mutators
bool TradingCardTypes::SetBasic(bool b) {
	return basic = b;
}

bool TradingCardTypes::GetBasic() const {
	return basic;
}

bool TradingCardTypes::SetSingleton(bool b) {
	return singleton = b;
}

bool TradingCardTypes::GetSingleton() const {
	return singleton;
}

bool TradingCardTypes::SetTrigger(bool b) {
	return trigger = b;
}

bool TradingCardTypes::GetTrigger() const {
	return trigger;
}

bool TradingCardTypes::SetCommand(bool b) {
	return command = b;
}

bool TradingCardTypes::GetCommand() const {
	return command;
}

bool TradingCardTypes::SetMecha(bool b) {
	return mecha = b;
}

bool TradingCardTypes::GetMecha() const {
	return mecha;
}

bool TradingCardTypes::SetTower(bool b) {
	return tower = b;
}

bool TradingCardTypes::GetTower() const {
	return tower;
}

