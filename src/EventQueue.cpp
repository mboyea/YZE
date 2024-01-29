#include "EventQueue.h"

EventQueue::EventNode* EventQueue::GetEventLocation(EventNode* prev, size_t timeOfEvent) {
	if (prev == nullptr || prev->timeOf >= timeOfEvent) {
		return nullptr;
	}
	if (prev->timeOf <= timeOfEvent) {
		return prev;
	}
	return GetEventLocation(prev->next, timeOfEvent);
}

void EventQueue::InsertEvent(EventNode* prev, void(*func)(), size_t timeOfEvent) {
	if (prev == nullptr) {
		eventHead = new EventNode(func, timeOfEvent, nullptr);
		return;
	}
	EventNode* eventNode = new EventNode(func, timeOfEvent, prev->next);
	prev->next = eventNode;
}

void EventQueue::DeleteEventHead() {
	EventNode* temp = eventHead;
	eventHead = eventHead->next;
	delete temp;
}

void EventQueue::Push(void(*func)(), uint32_t msToEvent) {
	uint32_t timeOfEvent = SDL_GetTicks() + msToEvent;
	InsertEvent(GetEventLocation(eventHead, timeOfEvent), func, timeOfEvent);
}

void EventQueue::HandleEvents() {
	if (eventHead == nullptr) {
		return;
	}
	if (SDL_GetTicks() >= eventHead->timeOf) {
		eventHead->func();
		DeleteEventHead();
		HandleEvents();
	}
}