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
#pragma once

#include <string>

class TradingCardTypes {
public:
	TradingCardTypes Parse(std::string);
	std::string Stringify() const;

	//accessors & mutators
	bool SetBasic(bool b);
	bool GetBasic() const;

	bool SetSingleton(bool b);
	bool GetSingleton() const;

	bool SetTrigger(bool b);
	bool GetTrigger() const;

	bool SetCommand(bool b);
	bool GetCommand() const;

	bool SetMecha(bool b);
	bool GetMecha() const;

	bool SetTower(bool b);
	bool GetTower() const;

private:
	bool basic = false;
	bool singleton = false;
	bool trigger = false;
	bool command = false;
	bool mecha = false;
	bool tower = false;
};