# Object
It contains the predefined components and game objects.

- [FpsShower](Object.md#fpsshower)

##
## FpsShower
### Source Code:
[FpsShower.h](../../Learning2DEngine/Learning2DEngine/Object/FpsShower.h)  
[FpsShower.cpp](../../Learning2DEngine/Learning2DEngine/Object/FpsShower.cpp)

### Description:
It shows the FPS (frame per second).  
It uses glfwGetTime(), the Time::deltaTime is not good here, because it is multiplided with Time::timeScale.

### Header:
```cpp
class FpsShower : public System::UpdaterComponent, public UI::SimpleText2DRenderComponent
{...}
```

### Variables:
**Protected:**  
**previousTime**
```cpp
float previousTime;
```

**fps**  
FPS counter.
```cpp
int fps;
```

### Functions:
**Protected:**  
**FpsShower**  
```cpp
FpsShower(System::GameObject* gameObject, const UI::FontSizePair& fontSizePair, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```

**Init**  
```cpp
void Init() override;
```

**Destroy**  
```cpp
void Destroy() override;
```

**Update**  
```cpp
void Update() override;
```

**Public:**  
**CreateFpsShowerObject**  
It creates a GameObject with FpsShower component.
```cpp
static FpsShower* CreateFpsShowerObject(const System::Transform& transform, const UI::FontSizePair& fontSizePair, int layer = 0, glm::vec4 color = glm::vec4(1.0f));
```