#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <SDL2/SDL_events.h>
#include <queue>

class EventManager {
private:
  std::queue<SDL_Event> events;

public:
  EventManager();
  ~EventManager();

  void poll();
  bool hasEvent() const;
  SDL_Event getNext();
};

#endif
