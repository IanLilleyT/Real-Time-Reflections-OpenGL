#include "EventHandler.h"

EventHandler::EventHandler(){}
EventHandler::~EventHandler(){}

/*---------------------------------------------
  Input Event
---------------------------------------------*/
void EventHandler::addInputEventListener(sf::Event::EventType eventType, InputReceiver receiverFunction)
{
	InputReceiverList* inputReceivers = 0;
	InputEventMap::iterator iter = this->inputEventMap.find(eventType);
	if(iter == this->inputEventMap.end())
		inputReceivers = new InputReceiverList();
	else
		inputReceivers = iter->second;
	inputReceivers->push_back(receiverFunction);
	inputEventMap[eventType] = inputReceivers;
}
void EventHandler::removeInputEventListener(sf::Event::EventType eventType, InputReceiver receiverFunction)
{	
	InputEventMap::iterator iter = this->inputEventMap.find(eventType);
	if(iter != this->inputEventMap.end())
	{
		InputReceiverList* receiverList = iter->second;
		for(unsigned int i = 0; i < receiverList->size(); i++)
		{
			InputReceiver receiver = receiverList->at(i);
			if(receiver.object_ptr == receiverFunction.object_ptr)
			{
				receiverList->erase(receiverList->begin() + i);
				break;
			}
		}
	}
}

/*---------------------------------------------
  Enter Frame Event
---------------------------------------------*/
void EventHandler::addEnterFrameEventListener(EnterFrameReceiver receiverFunction)
{
	enterFrameEvents.push_back(receiverFunction);
}
void EventHandler::removeEnterFrameListener(EnterFrameReceiver receiverFunction)
{
	for(unsigned int i = 0; i < enterFrameEvents.size(); i++)
	{
		EnterFrameReceiver receiver = enterFrameEvents.at(i);
		if(receiver.object_ptr == receiverFunction.object_ptr)
		{
			enterFrameEvents.erase(enterFrameEvents.begin() + i);
			break;
		}
	}
}
void EventHandler::enterFrame()
{
	for(unsigned int i = 0; i < this->enterFrameEvents.size(); i++)
	{
		EnterFrameReceiver receiver = this->enterFrameEvents.at(i);
		receiver();
	}
}

/*---------------------------------------------
  SFML event processing
---------------------------------------------*/
void EventHandler::processEvent(sf::Event sfEvent)
{
	//Process events
	InputEventMap::iterator iter = this->inputEventMap.find(sfEvent.Type);
	if(iter != this->inputEventMap.end())
	{
		InputReceiverList* receiverList = iter->second;
		for(unsigned int i = 0; i < receiverList->size(); i++)
		{
			InputReceiver receiver = receiverList->at(i);
			receiver(sfEvent);
		}
	}

	//Update mouse position
	if(sfEvent.Type == sf::Event::MouseMoved)
	{
		int x = sfEvent.MouseMove.X;
		int y = sfEvent.MouseMove.Y;
		this->setMousePos(x,y);
	}
}


/*---------------------------------------------
  Keyboard
---------------------------------------------*/
bool EventHandler::isKeyDown(sf::Keyboard::Key keyCode)
{
	return sf::Keyboard::IsKeyPressed(keyCode);
}
bool EventHandler::isShiftDown()
{
	return this->isKeyDown(sf::Keyboard::LShift) || this->isKeyDown(sf::Keyboard::RShift);
}
bool EventHandler::isAltDown()
{
	return this->isKeyDown(sf::Keyboard::LAlt) || this->isKeyDown(sf::Keyboard::RAlt);
}
bool EventHandler::isControlDown()
{
	return this->isKeyDown(sf::Keyboard::LControl) || this->isKeyDown(sf::Keyboard::RControl);
}

/*---------------------------------------------
  Mouse
---------------------------------------------*/
bool EventHandler::isMouseMoving()
{
	return (!(mouseX == prevMouseX && mouseY == prevMouseY));
}
bool EventHandler::isMouseDown(sf::Mouse::Button button)
{
	return sf::Mouse::IsButtonPressed(button);
}
bool EventHandler::isLeftMouseDown()
{
	return this->isMouseDown(sf::Mouse::Left);
}
bool EventHandler::isRightMouseDown()
{
	return this->isMouseDown(sf::Mouse::Right);
}
bool EventHandler::isMiddleMouseDown()
{
	return this->isMouseDown(sf::Mouse::Middle);
}
glm::ivec2 EventHandler::getMousePos(void)
{
	return glm::ivec2(mouseX, mouseY);
}
glm::ivec2 EventHandler::getPrevMousePos(void)
{
	return glm::ivec2(prevMouseX,prevMouseY);
}
void EventHandler::setMousePos(int x, int y)
{
	this->prevMouseX = this->mouseX;
	this->prevMouseY = this->mouseY;
	this->mouseX = x;
	this->mouseY = y;
}