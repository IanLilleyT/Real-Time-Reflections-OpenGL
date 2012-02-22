#pragma once

#include <SFML/Window/Event.hpp> //For general events
#include <SFML/Window/Mouse.hpp> //For mouse events
#include <SFML/Window/Keyboard.hpp> //For key events
#include <srutil/delegate/delegate.hpp> //For delegates
#include <glm/glm.hpp> //For math

#include <map> //For mapping events to receivers
#include <vector> //For listing receivers

/*---------------------------------------------
EventHandler: Handles enterFrame and input events.
Also provides helpers for mouseDown, keyDown, etc
---------------------------------------------*/

//Input usage:
//Singleton<EventHandler>::Instance()->addEnterFrameEventListener(EnterFrameReceiver::from_method<GameState,&GameState::enterFrame>(this));

//EnterFrame usage:
//Singleton<EventHandler>::Instance()->addInputEventListener(sf::Event::MouseMoved, InputReceiver::from_method<World,&World::mouseMoved>(this));

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