#include "InputManager.h"


using namespace Input;

InputManager* InputManager::_instance = nullptr;

InputManager::InputManager() {

}

InputManager& InputManager::GetInstance() {
	if (_instance == nullptr) {
		_instance = new InputManager();
	}
	return  *_instance;
}

void InputManager::SetWindow(GLFWwindow* window) {
	_actualWindow = window;
	glfwSetKeyCallback(_actualWindow, InputManager::KeyCallback);
	glfwSetCursorPosCallback(_actualWindow, InputManager::MousePositionCallbackIntern); 
	glfwSetMouseButtonCallback(window, InputManager::MouseButtonCallbackIntern);

}




void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	InputManager& instance = GetInstance();

	// invoke callbacks
	for (auto& entry : instance._functions[key]) {
		if (action == GLFW_PRESS && int(entry.actions & CallbackAction::KEY_PRESSED)) {
			entry.function(CallbackAction::KEY_PRESSED);
		}

		else if (action == GLFW_RELEASE && int(entry.actions & CallbackAction::KEY_RELEASED)) {
			entry.function(CallbackAction::KEY_RELEASED);
		}

		else if (action == GLFW_REPEAT && int(entry.actions & CallbackAction::KEY_REPEATED)) {
			entry.function(CallbackAction::KEY_REPEATED);
		}
	}

	// update maps
	if (action == GLFW_PRESS) {
		instance._keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		instance._keys[key] = false;
	}
}

IdCallback InputManager::RegisterKeyboardCallback(int key, CallbackAction actions, KeyboardCallback function) {
	KeyboardCallbackInfo aux = {
		++_id, function, actions
	};
	
	_functions[key].push_back(aux);
	return _id;
}





void InputManager::MousePositionCallbackIntern(GLFWwindow* window, double xpos, double ypos) {
	for (auto& entry : _instance->_mousePos) {
		entry.function(xpos, ypos);
	}

	_instance->_mousePosition[0] = xpos;
	_instance->_mousePosition[1] = ypos;

}

IdCallback InputManager::RegisterMousePositionCallback(MousePositionCallback mouseCal) {
	MousePosCallbackInfo aux = {++_id, mouseCal};
	_mousePos.push_back(aux);
	return _id;
}




void InputManager::MouseButtonCallbackIntern(GLFWwindow* window, int button, int action, int mods){
	InputManager& instance = GetInstance();
	for (auto& entry : instance._mouseButton[button]) {
		if (action == GLFW_PRESS && int(entry.actions & CallbackAction::KEY_PRESSED)) {
			entry.function(CallbackAction::KEY_PRESSED);
		}
		else if (action == GLFW_RELEASE && int(entry.actions & CallbackAction::KEY_RELEASED)) {
			entry.function(CallbackAction::KEY_RELEASED);
		}
	}

	// update maps
	if (action == GLFW_PRESS) {
		instance._keys[button] = true;
	}
	else if (action == GLFW_RELEASE) {
		instance._keys[button] = false;
	}
}

IdCallback InputManager::RegisterMouseButtonCallback(int button, CallbackAction actions, MouseButtonCallback mouseCal) {
	MouseButtonCallbackInfo aux = {
		++_id, mouseCal, actions
	};

	_mouseButton[button].push_back(aux);
	return _id;
}





void InputManager::UnregisterCallback(IdCallback id) {
	// look in mousePos functions
	std::vector<MousePosCallbackInfo>::iterator e;
	for (e = _mousePos.begin(); e < _mousePos.end(); e++) {
		if ((*e).id == id) {
			break;
		}
	}
	if (e != _mousePos.end()) {
		_mousePos.erase(e);
		return;
	}

	// look in mouse buttons function
	for (auto& entry : _functions) {
		std::vector<KeyboardCallbackInfo>::iterator e;

		for (e = entry.second.begin(); e < entry.second.end(); e++) {
			if ((*e).id == id) {
				break;
			}
		}
		if (e != entry.second.end()) {
			entry.second.erase(e);
			return;
		}
	}

	// look in keyboard functions
	for (auto& entry : _functions) {
		std::vector<KeyboardCallbackInfo>::iterator e;

		for (e = entry.second.begin(); e < entry.second.end(); e++) {
			if ((*e).id == id) {
				break;
			}
		}
		if (e != entry.second.end()) {
			entry.second.erase(e);
			return;
		}
	}
}


bool InputManager::IsKeyPressed(int keyButton) {
	if (_keys.find(keyButton) == _keys.end()) {
		// No entry in the map
		_keys[keyButton] = false;
	}

	return _keys[keyButton];
}


void InputManager::GetMousePosition(double& x, double& y) {
	x = _mousePosition[0];
	y = _mousePosition[1];
}