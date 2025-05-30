# Animator
- [AnimationController](Animator.md#animationcontroller)
- [AnimationFrame](Animator.md#animationframe)

##
## AnimationController
### Source Code:
[AnimationController.h](../../Learning2DEngine/Learning2DEngine/Animator/AnimationController.h)  
[AnimationController.cpp](../../Learning2DEngine/Learning2DEngine/Animator/AnimationController.cpp)  

### Description:
It can control an animation.  
It has to get a `Texture2DContainer` in its contructor, because it will change the textures on that
`Texture2DContainer`.

### Header:
```cpp
class AnimationController : public Learning2DEngine::System::LateUpdaterComponent
{...}
```

### Variables:
**Private:**  
**textureContainer**  
```cpp
Render::Texture2DContainer* textureContainer;
```

**frames**  
```cpp
std::vector<AnimationFrame> frames;
``` 

**currentIndex**  
```cpp
size_t currentIndex;
``` 

**currentTime**  
```cpp
float currentTime;
``` 

**isPlaying**  
```cpp
bool isPlaying;
``` 

**Public:**  
**speed**  
```cpp
float speed;
```

**isLoop**  
```cpp
bool isLoop;
```

### Functions:
**Protected:**  
**AnimationController**  
```cpp
AnimationController(System::GameObject* gameObject, Render::Texture2DContainer* textureContainer, bool isLoop = false);
```
```cpp
AnimationController(System::GameObject* gameObject, Render::Texture2DContainer* textureContainer, size_t minFrameSize, bool isLoop = false);
```

**LateUpdate**  
```cpp
void LateUpdate() override;
```

**Public:**  
**Play**  
```cpp
void Play(bool reset = false);
```

**Stop**  
```cpp
void Stop();
```

**Add**  
```cpp
void Add(const AnimationFrame& frame);
```
```cpp
void Add(AnimationFrame&& frame);
```

**Remove**  
```cpp
void Remove(size_t index);
```

**Clear**  
```cpp
void Clear();
```

**operator[]**  
```cpp
AnimationFrame& operator[](size_t index);
```
```cpp
const AnimationFrame& operator[](size_t index) const;
```

**JumpToFrame**  
If the time is less then 0.0f, it will use the index frame time.
```cpp
void JumpToFrame(size_t index, float time = -1.0f);
```

**GetCurrentFrame**  
```cpp
const inline AnimationFrame& GetCurrentFrame() const;
```

**GetCurrentIndex**  
```cpp
inline size_t GetCurrentIndex() const;
```

**GetCurrentTime**  
```cpp
inline float GetCurrentTime() const;
```

##
## AnimationFrame
### Source Code:
[AnimationFrame.h](../../Learning2DEngine/Learning2DEngine/Animator/AnimationFrame.h)  

### Description:
It contains an animation frame.

### Header:
```cpp
struct AnimationFrame
{
	Render::Texture2D* texture;
	float time;
};
```