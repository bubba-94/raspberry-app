#include "EventManager.hpp"

EventManager::EventManager() : events{} {}

EventManager::~EventManager() {}

void EventManager::poll() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    events.push(sdlEvent);
  }
}

bool EventManager::hasEvent() const { return !events.empty(); }

SDL_Event EventManager::getNext() {
  if (events.empty())
    return SDL_Event{}; // return empty event if none
  SDL_Event e = events.front();
  events.pop();
  return e;
}