#include "LuaComponent.h"

#include <iostream>

#include "LuaInstance.h"

LuaComponent::LuaComponent(std::string pathScript) : _pathScript(pathScript) {
	std::cout << ScriptName() << std::endl;
}

std::string LuaComponent::ScriptName() {
	size_t end = _pathScript.find(".lua");

	std::string resul = _pathScript.substr(0, end);

	size_t pos = resul.find('/');
	while (pos != std::string::npos) {
		resul = resul.substr(pos+1, resul.size());
		pos = resul.find('/');
	}

	return resul;
}

void LuaComponent::Initialize() {
	LuaInstance::GetInstance().LoadScript(_pathScript.c_str());
	LuaInstance::GetInstance().LoadScript("enemy.lua");
	LuaInstance::GetInstance().LoadScript("../enemy.lua");
	LuaInstance::GetInstance().LoadScript("../resources/lua/enemy.lua");
}

void LuaComponent::Start() {
	// call LuaStart
}

void LuaComponent::Update(float deltaTime) {
	// call LuaUpdate
	LuaInstance::GetInstance().ExecuteProcedure((ScriptName() + "_Update").c_str(), deltaTime);
}

void LuaComponent::End() {
	// call LuaEnd

}