#pragma once

#include "RenderEngine/VulkanInstance.h"
#include <functional>
#include <map>


class GLFWwindow;
namespace Input {

	enum class CallbackAction {
		KEY_PRESSED = 1,
		KEY_RELEASED = 2,
		KEY_REPEATED = 8
	};

	typedef std::function<void(CallbackAction)> KeyboardCallback;

	typedef std::function<void(double xpos, double ypos)> MousePositionCallback;

	typedef std::function<void(CallbackAction)> MouseButtonCallback;

	// OR operator
	inline CallbackAction operator|(CallbackAction a, CallbackAction b)
	{
		return static_cast<CallbackAction>(static_cast<int>(a) | static_cast<int>(b));
	}

	// AND operator
	inline CallbackAction operator&(CallbackAction a, CallbackAction b)
	{
		return static_cast<CallbackAction>(static_cast<int>(a) & static_cast<int>(b));
	}

	// OR operator
	inline int operator|(int a, CallbackAction b)
	{
		return (static_cast<int>(a) | static_cast<int>(b));
	}

	// AND operator
	inline int operator&(int a, CallbackAction b)
	{
		return (static_cast<int>(a) & static_cast<int>(b));
	}

	typedef unsigned int IdCallback;

	/*
		Singleton to register input callbacks to a window and to ask about keys and mouse position
	*/
	class InputManager
	{
	public:

		static InputManager& GetInstance();

		/*
			Set the actual window to work with
		*/
		void SetWindow(GLFWwindow* window);

		/*
			Add a function to be called when the key, return an id to erase the callback
		*/
		IdCallback RegisterKeyboardCallback(int key, CallbackAction actions, KeyboardCallback function);


		/*
			Add a function to be called when the mouse moves 
		*/
		IdCallback RegisterMousePositionCallback(MousePositionCallback mouseCal);

		/*
			Add a function to be called when a mouse button is pressed
		*/
		IdCallback RegisterMouseButtonCallback(int button, CallbackAction actions, MouseButtonCallback mouseCal);

		/*
			Add a function to be called when the mouse moves
		*/
		IdCallback RegisterMouseScrollCallback(MousePositionCallback mouseCal);

		/*
			Unregister a callback previusly added
		*/
		void UnregisterCallback(IdCallback id);

		/*
			Check if a key or mouse button is pressed (use GLFW sintax
		*/
		bool IsKeyPressed(int keyButton);

		/*
			Return the last mouse position
		*/
		void GetMousePosition(double& x, double& y);

		/*
			Return the last scroll position
		*/
		void GetScrollPosition(double& x, double& y);

		/*
			Reset the cursor position
		*/
		void ResetCursorPosition();

	private:
		struct KeyboardCallbackInfo {
			IdCallback id;
			KeyboardCallback function;
			CallbackAction actions;
		};

		struct MousePosCallbackInfo{
			IdCallback id;
			MousePositionCallback function;
		};
		struct MouseButtonCallbackInfo {
			IdCallback id;
			MouseButtonCallback function;
			CallbackAction actions;
		};


		InputManager();

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static void MousePositionCallbackIntern(GLFWwindow* window, double xpos, double ypos);

		static void MouseButtonCallbackIntern(GLFWwindow* window, int button, int action, int mods);

		static void MouseScrollCallbackIntern(GLFWwindow* window, double xpos, double ypos);


		// singleton instance
		static InputManager* _instance;

		//window in use
		GLFWwindow* _actualWindow;

		//map to store the callbacks for keyboard
		std::map<int, std::vector<KeyboardCallbackInfo>> _functions;

		IdCallback _id = 0;

		//vector for the mouse position callbacks
		std::vector<MousePosCallbackInfo> _mousePos;

		//vector for the mouse scroll callbacks
		std::vector<MousePosCallbackInfo> _scrollPos;

		//map to store the callbacks for mouse buttons
		std::map<int, std::vector<MouseButtonCallbackInfo>> _mouseButton;

		// map to store the state of keys/buttons
		std::map<int, bool> _keys;

		double _mousePosition[2] = { 0, 0 };
		double _scrollPosition[2] = { 0, 0 };
	};
}

