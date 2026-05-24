# EventSystem
- [CursorEnterEventItem](EventSystem.md#cursorentereventitem)
- [CursorPositionEventItem](EventSystem.md#cursorpositioneventitem)
- [EventHandler](EventSystem.md#eventhandler)
- [EventItem](EventSystem.md#eventitem)
- [KeyboardEventItem](EventSystem.md#keyboardeventitem)
- [MouseButtonEventItem](EventSystem.md#mousebuttoneventitem)
- [ResolutionEventItem](EventSystem.md#resolutioneventitem)
- [ScrollEventItem](EventSystem.md#scrolleventitem)

##
## CursorEnterEventItem
### Source Code:
[CursorEnterEventItem.h](../../Learning2DEngine/Learning2DEngine/EventSystem/CursorEnterEventItem.h)

### Description:
It is an `EventItem` implementation. It can be used
when the developer want to subscribe to Cursor Enter events.  
It does not recommend to use. Please use the Game::IsCursorInWindow().

### Header:
```cpp
class CursorEnterEventItem final : public EventItem<bool>
{...}
```

### Variables:
**Private:**  
**refresher**  
```cpp
System::ICursorRefresher* refresher;
```

### Functions:
**Public:**  
**CursorEnterEventItem**  
```cpp
CursorEnterEventItem(System::ICursorRefresher* refresher);
```

**Call**  
```cpp
void Call(bool entered) override;
```

##
## CursorPositionEventItem
### Source Code:
[CursorPositionEventItem.h](../../Learning2DEngine/Learning2DEngine/EventSystem/CursorPositionEventItem.h)

### Description:
It is an `EventItem` implementation. It can be used
when the developer want to subscribe to Cursor Position events.  
It does not recommend to use. Please use the Game::GetCursorPosition().

### Header:
```cpp
class CursorPositionEventItem final : public EventItem<double, double>
{...}
```

### Variables:
**Private:**  
**refresher**  
```cpp
System::ICursorRefresher* refresher;
```

### Functions:
**Public:**  
**CursorPositionEventItem**  
```cpp
CursorPositionEventItem(System::ICursorRefresher* refresher);
```

**Call**  
```cpp
void Call(double xpos, double ypos) override;
```

##
## EventHandler
### Source Code:
[EventHandler.h](../../Learning2DEngine/Learning2DEngine/EventSystem/EventHandler.h)  
[EventHandlerTest.cpp](../../Learning2DEngineTest/EventSystem/EventHandlerTest.cpp)

### Description:
It is an event handler, which is a bit similar like the C# EventHandler.
The developer can subscribe/unsubscribe to event by `EventItems`.

### Header:
```cpp
template<class ...Params>
class EventHandler
{...}
```

### Variables:
**Private:**  
**events**  
```cpp
std::list<EventItem<Params...>*> events;
```

### Functions:
**Public:**  
**Add**  
It adds an `EventItem` to `EventHandler`.
```cpp
inline void Add(EventItem<Params...>* e);
```

**Remove**  
It removes an `EventItem` from `EventHandler`.
```cpp
inline void Remove(EventItem<Params...>* e);
```

**Clear**  
It removes all `EventItems` from `EventHandler`.
```cpp
inline void Clear();
```

**Invoke**  
It call all `EventItems`, which the `EventHandler` has.
```cpp
void Invoke(Params... params);
```

##
## EventItem
### Source Code:
[EventItem.h](../../Learning2DEngine/Learning2DEngine/EventSystem/EventItem.h)

### Description:
The developer can subscribe/unsubscribe to `EventHandlers` with their `EventItem`.

### Header:
```cpp
template<class ...Params>
class EventItem
{...}
```

### Functions:
**Public:**  
**~EventItem**  
```cpp
virtual ~EventItem();
```

**Call**  
It is an abstract function, which the developer has to define.
If the `EventItem` was added (subscribed) to `EventHandler`,
the `Invoke(...)` function of `EventHandler` will call this function.
```cpp
virtual void Call(Params... params) = 0;
```

##
## KeyboardEventItem
### Source Code:
[KeyboardEventItem.h](../../Learning2DEngine/Learning2DEngine/EventSystem/KeyboardEventItem.h)

### Description:
It is an `EventItem` implementation. It can be used
when the developer want to subscribe to Keyboard events.  
It does not recommend to use. Please use the Game::GetKeyboardButtonStatus(int key).

### Header:
```cpp
class KeyboardEventItem final : public EventItem<int, int, int, int>
{...}
```

### Variables:
**Private:**  
**refresher**  
```cpp
System::IKeyboardRefresher* refresher;
```

### Functions:
**Public:**  
**KeyboardEventItem**  
```cpp
KeyboardEventItem(System::IKeyboardRefresher* refresher);
```

**Call**  
```cpp
void Call(int key, int scancode, int action, int mode) override;
```

##
## MouseButtonEventItem
### Source Code:
[MouseButtonEventItem.h](../../Learning2DEngine/Learning2DEngine/EventSystem/MouseButtonEventItem.h)

### Description:
It is an `EventItem` implementation. It can be used
when the developer want to subscribe to Mouse Buttons events.  
It does not recommend to use. Please use the Game::GetMouseButtonStatus(int key).

### Header:
```cpp
class MouseButtonEventItem final : public EventItem<int, int, int>
{...}
```

### Variables:
**Private:**  
**refresher**  
```cpp
System::ICursorRefresher* refresher;
```

### Functions:
**Public:**  
**MouseButtonEventItem**  
```cpp
MouseButtonEventItem(System::ICursorRefresher* refresher);
```

**Call**  
```cpp
void Call(int button, int action, int mods) override;
```

##
## ResolutionEventItem
### Source Code:
[ResolutionEventItem.h](../../Learning2DEngine/Learning2DEngine/EventSystem/ResolutionEventItem.h)

### Description:
It is an `EventItem` implementation. It can be used
when the developer want to subscribe to (game) screen's resolution changes.

### Header:
```cpp
class ResolutionEventItem final : public EventItem<Render::Resolution>
{...}
```

### Variables:
**Private:**  
**refresher**  
```cpp
Render::IResolutionRefresher* refresher;
```

### Functions:
**Public:**  

**ResolutionEventItem**  
```cpp
ResolutionEventItem(Render::IResolutionRefresher* refresher);
```

**Call**  
```cpp
void Call(Render::Resolution resolution) override;
```

##
## ScrollEventItem
### Source Code:
[ScrollEventItem.h](../../Learning2DEngine/Learning2DEngine/EventSystem/ScrollEventItem.h)

### Description:
It is an `EventItem` implementation. It can be used
when the developer want to subscribe to Scroll events.  
It does not recommend to use. Please use the Game::GetScroll().

### Header:
```cpp
class ScrollEventItem final : public EventItem<double, double>
{...}
```

### Variables:
**Private:**  
**refresher**  
```cpp
System::ICursorRefresher* refresher;
```

### Functions:
**Public:**  
**ScrollEventItem**  
```cpp
ScrollEventItem(System::ICursorRefresher* refresher);
```

**Call**  
```cpp
void Call(double xoffset, double yoffset) override;
```
