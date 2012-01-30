#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <srutil/delegate/delegate.hpp>
#include <glm/glm.hpp>

#include <map>
#include <utility>
#include <vector>

/*---------------------------------------------
EventHandler: Handles enterFrame and input events.
Also provides helpers for mouseDown, keyDown, etc
---------------------------------------------*/

typedef srutil::delegate<void (void)> EnterFrameReceiver;
typedef std::vector<EnterFrameReceiver> EnterFrameReceiverList;

typedef srutil::delegate<void (sf::Event)> InputReceiver;
typedef std::vector<InputReceiver> InputReceiverList;
typedef std::map<sf::Event::EventType,InputReceiverList*> InputEventMap;

class EventHandler
{
public:
	EventHandler();
	~EventHandler();

	//Events
	void enterFrame();
	void processEvent(sf::Event sfEvent);

	//Enter Frame
	void addEnterFrameEventListener(EnterFrameReceiver receiverFunction);
	void removeEnterFrameListener(EnterFrameReceiver receiverFunction);

	//Input
	void addInputEventListener(sf::Event::EventType eventType, InputReceiver receiverFunction);
	void removeInputEventListener(sf::Event::EventType eventType, InputReceiver receiverFunction);

	//Keyboard
	bool isKeyDown(sf::Keyboard::Key keyCode);
	bool isShiftDown();
	bool isAltDown();
	bool isControlDown();

	//Mouse
	bool isMouseMoving();
	bool isLeftMouseDown();
	bool isRightMouseDown();
	bool isMiddleMouseDown();

	glm::ivec2 getMousePos(void);
	glm::ivec2 getPrevMousePos(void);
	
private:
	InputEventMap inputEventMap;
	EnterFrameReceiverList enterFrameEvents;

	bool isMouseDown(sf::Mouse::Button button);
	void setMousePos(int x, int y);

	int mouseX;
	int mouseY;
	int prevMouseX;
	int prevMouseY;
};