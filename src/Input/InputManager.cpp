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
	for (auto& entry : _instance->_functions[key]) {
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
}

IdCallback InputManager::RegisterMousePositionCallback(MousePositionCallback mouseCal) {
	MousePosCallbackInfo aux = {++_id, mouseCal};
	_mousePos.push_back(aux);
	return _id;
}




void InputManager::MouseButtonCallbackIntern(GLFWwindow* window, int button, int action, int mods){
	for (auto& entry : _instance->_mouseButton[button]) {
		if (action == GLFW_PRESS && int(entry.actions & CallbackAction::KEY_PRESSED)) {
			entry.function(CallbackAction::KEY_PRESSED);
		}
		else if (action == GLFW_RELEASE && int(entry.actions & CallbackAction::KEY_RELEASED)) {
			entry.function(CallbackAction::KEY_RELEASED);
		}
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