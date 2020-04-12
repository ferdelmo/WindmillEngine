#include "LuaComponent.h"

#include <iostream>

#include "LuaInstance.h"

LuaComponent::LuaComponent(std::string pathScript) : _pathScript(pathScript) {
	//std::cout << ScriptName() << std::endl;
	_name = ScriptName();
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
}

void LuaComponent::Start() {
	// call LuaStart
	LuaInstance::GetInstance().ExecuteProcedure((_name + "_Start").c_str(), _owner);
}

void LuaComponent::Update(float deltaTime) {
	// call LuaUpdate
	LuaInstance::GetInstance().ExecuteProcedure((_name + "_Update").c_str(), _owner, deltaTime);
}

void LuaComponent::End() {
	// call LuaEnd
	LuaInstance::GetInstance().ExecuteProcedure((_name + "_End").c_str(), _owner);

}
