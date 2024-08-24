# EventSystem

## EventHandler
### Source Code:
[EventHandler.h](../../Learning2DEngine/Learning2DEngine/EventSystem/EventHandler.h)

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

## KeyboardMouseEventItem
### Source Code:
[KeyboardMouseEventItem.h](../../Learning2DEngine/Learning2DEngine/EventSystem/KeyboardMouseEventItem.h)

### Description:
It is a `EventItem` implementation. It can be used
when the developer want to subscribe to Keyboard and Mouse events.

### Header:
```cpp
class KeyboardMouseEventItem final : public virtual EventItem<int, int, int, int>
{...}
```

### Variables:
**Private:**  
**refresher**  
```cpp
System::IKeyboardMouseRefresher* refresher;
```

### Functions:
**Public:**  

**KeyboardMouseEventItem**  
```cpp
KeyboardMouseEventItem(System::IKeyboardMouseRefresher* refresher);
```

**Call**  
```cpp
void Call(int key, int scancode, int action, int mode) override;
```

## ResolutionEventItem
### Source Code:
[ResolutionEventItem.h](../../Learning2DEngine/Learning2DEngine/EventSystem/ResolutionEventItem.h)

### Description:
It is a `EventItem` implementation. It can be used
when the developer want to subscribe to (game) screen's resolution changes.

### Header:
```cpp
class ResolutionEventItem final : public virtual EventItem<Render::Resolution>
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

