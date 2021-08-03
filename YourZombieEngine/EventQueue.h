#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#pragma once

#include <SDL.h>

class EventQueue {
protected:
	struct EventNode {
		void (*func)();
		Uint32 timeOf;
		EventNode* next;

		EventNode(void(*func)(), uint32_t timeOf, EventNode* next)
			: func(func), timeOf(timeOf), next(next) {}
	};
	EventNode* eventHead = nullptr;
	EventNode* GetEventLocation(EventNode* prev, size_t timeOfEvent);
	void InsertEvent(EventNode* prev, void (*func)(), size_t timeOfEvent);
	void DeleteEventHead();
public:
	void Push(void (*func)(), uint32_t msToEvent);
	void HandleEvents();
};

#endif // !EVENT_QUEUE_H