# Physics
This namespace really simple. It has only some really basic functionality.

- [BaseBoxColliderComponent](Physics.md#baseboxcollidercomponent)
- [BaseCircleColliderComponent](Physics.md#basecirclecollidercomponent)
- [BaseColliderComponent](Physics.md#basecollidercomponent)
- [BaseRigidbody](Physics.md#baserigidbody)
- [BoxColliderComponent](Physics.md#boxcollidercomponent)
- [CircleColliderComponent](Physics.md#circlecollidercomponent)
- [ColliderComponentHandler](Physics.md#collidercomponenthandler)
- [ColliderMode](Physics.md#collidermode)
- [ColliderType](Physics.md#collidertype)
- [Collision](Physics.md#collision)
- [CollisionData](Physics.md#collisiondata)
- [CollisionHelper](Physics.md#collisionhelper)
- [Rigidbody](Physics.md#rigidbody)

##
## BaseBoxColliderComponent
### Source Code:
[BaseBoxColliderComponent.h](../../Learning2DEngine/Learning2DEngine/Physics/BaseBoxColliderComponent.h)

### Description:
The `BaseBoxColliderComponent` is really basic. It doesn't rotate,
scale with the gameobject.  
It has attributes for box collider, but this is a support
class only. Please use `BoxColliderComponent` instead of this.  
Please check for more info about `BoxColliderComponent` and `BaseColliderComponent`.

### Header:
```cpp
class BaseBoxColliderComponent : public virtual BaseColliderComponent
{...}
```

### Variables:
**Public:**  
**colliderSize**
```cpp
glm::vec2 colliderSize;
```

### Functions:
**Protected:**  
**BaseBoxColliderComponent**  
```cpp
BaseBoxColliderComponent(System::GameObject* gameObject, glm::vec2 size, ColliderType type = ColliderType::DYNAMIC, ColliderMode mode = ColliderMode::TRIGGER, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
```

**Public:**  
**GetColliderCenter**  
```cpp
glm::vec2 GetColliderCenter() const override;
```

##
## BaseCircleColliderComponent
### Source Code:
[BaseCircleColliderComponent.h](../../Learning2DEngine/Learning2DEngine/Physics/BaseCircleColliderComponent.h)

### Description:
The `BaseCircleColliderComponent` is really basic. It doesn't rotate,
scale with the gameobject.  
It has attributes for circle collider, but this is a support
class only. Please use `CircleColliderComponent` instead of this.  
Please check for more info about `CircleColliderComponent` and `BaseColliderComponent`.

### Header:
```cpp
class BaseCircleColliderComponent : public virtual BaseColliderComponent
{...}
```

### Variables:
**Public:**  
**colliderRadius**
```cpp
float colliderRadius;
```

### Functions:
**Protected:**  
**BaseCircleColliderComponent**  
```cpp
BaseCircleColliderComponent(System::GameObject* gameObject, float radius, ColliderType type = ColliderType::DYNAMIC, ColliderMode mode = ColliderMode::TRIGGER, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
```

**Public:**  
**GetColliderCenter**  
```cpp
glm::vec2 GetColliderCenter() const override;
```

##
## BaseColliderComponent
### Source Code:
[BaseColliderComponent.h](../../Learning2DEngine/Learning2DEngine/Physics/BaseColliderComponent.h)

### Description:
It is a base class for the `BaseBoxColliderComponent` and
`BaseCircleColliderComponent` classes.
It has some basic funcionality, which is essential for the colliders.  
The classes, which are inherited from this `BaseBoxColliderComponent`
has to have a constructor, which first parameter is `GameObject*` for gameObject member.  
Please check more info about `System::Component`.

### Header:
```cpp
class BaseColliderComponent : public virtual System::Component
{...}
```

### Variables:
**Private:**  
**rigidbody**   
```cpp
BaseRigidbody* rigidbody;
```

**Public:**  
**type**   
A collider can be dynamic or kinematic. 2 kinematic colliders can't collide with each other, 
but an dynamic collider can collide with them.  
For example: there are 3 circles (A, B and C) with 1.0f range. A is dynamic and another two are kinematic.
If they are on the same position, the A will be triggered twice (one with B and one with C),
but the B and the C will be triggered with A only.
```cpp
const ColliderType type;
```

**mode**   
It contains, that the collider is trigger or not.
```cpp
ColliderMode mode;
```
 
**colliderOffset**  
The center point will be shifted by this one.
```cpp
glm::vec2 colliderOffset;
```

**maskLayer**  
The maskLayer is int32_t, so it is always 32bit. That's why the developer should use it
like a bit array. The colliders can trigger each other only if (maskLayer1 & maskLayer2) > 0.
```cpp
int32_t maskLayer;
```

### Functions:
**Protected:**  
**BaseColliderComponent**  
```cpp
BaseColliderComponent(System::GameObject* gameObject, ColliderType type = ColliderType::DYNAMIC, ColliderMode mode = ColliderMode::TRIGGER, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
```

**Public:**  
**GetColliderCenter**  
It returns the center of Collider, which is middle of the object by shifted the offset.
```cpp
virtual glm::vec2 GetColliderCenter() const = 0;
```

**OnCollision**  
If 2 colliders trigger each other, their OnCollision function will be called.  
It does nothing by default.
```cpp
virtual void OnCollision(const Collision& collision);
```

**InitRigidbody**  
If the rigidbody is inited, the collider can use it in collision.
```cpp
void InitRigidbody();
```

**ClearRigidbody**  
```cpp
inline void ClearRigidbody();
```

**GetRigidbody**  
```cpp
inline BaseRigidbody* GetRigidbody() const;
```

##
## BaseRigidbody
### Source Code:
[BaseRigidbody.h](../../Learning2DEngine/Learning2DEngine/Physics/BaseRigidbody.h)

### Description:
It is an base class, which the colliders can use.

### Header:
```cpp
class BaseRigidbody
{...}
```

### Variables:
**Public:**  
**velocity**   
```cpp
glm::vec2 velocity;
```

### Functions:
**Public:**  
**BaseRigidbody**  
```cpp
BaseRigidbody(glm::vec2 velocity);
```

**~IRigidbody**  
```cpp
virtual ~BaseRigidbody() = default;
```

##
## BoxColliderComponent
### Source Code:
[BoxColliderComponent.h](../../Learning2DEngine/Learning2DEngine/Physics/BoxColliderComponent.h)

### Description:
The `BoxColliderComponent` is really basic. It doesn't rotate,
scale with the gameobject.  
It knows everything like the `BaseBoxColliderComponent`, just it subscribes/unsubscribes
for `ComponentManager`.  
Please check for more info about `BaseBoxColliderComponent` and `BaseColliderComponent`.

### Header:
```cpp
class BoxColliderComponent : public virtual BaseBoxColliderComponent
{...}
```

### Variables:
**Public:**  
**debugTool**   
It is available only, when the `L2DE_DEBUG_SHOW_COLLIDER` macro is defined.  
It can show/hide the collider in the game.
```cpp
DebugTool::DebugBoxColliderRenderComponent* debugTool;
```

### Functions:
**Protected:**  
**BoxColliderComponent**  
```cpp
BoxColliderComponent(System::GameObject* gameObject, glm::vec2 size, ColliderType type = ColliderType::DYNAMIC, ColliderMode mode = ColliderMode::TRIGGER, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
```

**Init**  
If this function is override, it must call the BoxColliderComponent::Init() in the first line.
```cpp
virtual void Init() override;
```

**Destroy**  
If this function is override, it must call the BoxColliderComponent::Destroy() in the first line.
```cpp
virtual void Destroy() override;
```

##
## CircleColliderComponent
### Source Code:
[CircleColliderComponent.h](../../Learning2DEngine/Learning2DEngine/Physics/CircleColliderComponent.h)

### Description:
The `CircleColliderComponent` is really basic. It doesn't rotate,
scale with the gameobject.  
It knows everything like the `BaseCircleColliderComponent`, just it subscribes/unsubscribes
for `ComponentManager`.  
Please check for more info about `BaseCircleColliderComponent` and `BaseColliderComponent`.

### Header:
```cpp
class CircleColliderComponent : public virtual BaseCircleColliderComponent
{...}
```

### Variables:
**Public:**  
**debugTool**   
It is available only, when the `L2DE_DEBUG_SHOW_COLLIDER` macro is defined.  
It can show/hide the collider in the game.
```cpp
DebugTool::DebugCircleColliderRenderComponent* debugTool;
```

### Functions:
**Protected:**  
**CircleColliderComponent**  
```cpp
CircleColliderComponent(System::GameObject* gameObject, float radius, ColliderType type = ColliderType::DYNAMIC, ColliderMode mode = ColliderMode::TRIGGER, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
```

**Init**  
If this function is override, it must call the CircleColliderComponent::Init() in the first line.
```cpp
virtual void Init() override;
```

**Destroy**  
If this function is override, it must call the CircleColliderComponent::Destroy() in the first line.
```cpp
virtual void Destroy() override;
```

##
## ColliderComponentHandler
### Source Code:
[ColliderComponentHandler.h](../../Learning2DEngine/Learning2DEngine/Physics/ColliderComponentHandler.h)  
[ColliderComponentHandler.cpp](../../Learning2DEngine/Learning2DEngine/Physics/ColliderComponentHandler.cpp)

### Description:
It can handle the collision between the `BaseBoxColliderComponent`
and the `BaseCircleColliderComponent` objects.  
The `ComponentManager` has one from it.

### Header:
```cpp
class ColliderComponentHandler : public System::IComponentHandler
{...}
```

### Variables:
**Protected:**  
**dynamicBoxColliders**  
```cpp
std::vector<BaseBoxColliderComponent*> dynamicBoxColliders;
```

**kinematicBoxColliders**  
```cpp
std::vector<BaseBoxColliderComponent*> kinematicBoxColliders;
```

**newBoxColliders**  
```cpp
std::vector<BaseBoxColliderComponent*> newBoxColliders;
```

**removableBoxColliders**  
```cpp
std::vector<BaseBoxColliderComponent*> removableBoxColliders;
```

**dynamicCircleColliders**  
```cpp
std::vector<BaseCircleColliderComponent*> dynamicCircleColliders;
```

**kinematicCircleColliders**  
```cpp
std::vector<BaseCircleColliderComponent*> kinematicCircleColliders;
```

**newCircleColliders**  
```cpp
std::vector<BaseCircleColliderComponent*> newCircleColliders;
```

**removableCircleColliders**  
```cpp
std::vector<BaseCircleColliderComponent*> removableCircleColliders;
```

**boxMutex**  
```cpp
std::mutex boxMutex;
```

**circleMutex**  
```cpp
std::mutex circleMutex;
```

**threads**  
```cpp
std::vector<std::thread> threads;
```

**maxColliderPerThread**  
If it is 0, the class will not use threads.
```cpp
unsigned int maxColliderPerThread;
```

### Functions:
**Protected:**  
**CheckCollisions**  
The function iterates on colliders from startIndex with the collider.  
IMPORTANT: The functions return, that the first object is still active
after the OnCollision(s).
```cpp
template<class T, class U>
bool CheckCollisions(T* collider, std::vector<U*> colliders, size_t startIndex);
```

**CheckCollision**  
The function checks only the second object is still active and they are collided
or not. If yes, It will call their OnCollision function.  
IMPORTANT: The functions return, that the first object is still active
after the OnCollision(s).
```cpp
template<class T, class U>
bool CheckCollision(T* first, U* second);
```

**RemoveItem**  
They will remove the item from the new colliders or they will add the item into removable colliders.  
They are used by Remove functions.
```cpp
void RemoveItem(BaseBoxColliderComponent* component);
```
```cpp
void RemoveItem(BaseCircleColliderComponent* component);
```

**RefreshBoxColliders**  
It removes the `removableBoxColliders` and adds the `newBoxColliders` to
the `activeBoxColliders` and the `passiveBoxColliders`.
After this, it clears the `newBoxColliders` and the `removableBoxColliders`.
```cpp
void RefreshBoxColliders();
```

**RefreshCircleColliders**  
It removes the `removableCircleColliders` and adds the `newCircleColliders` to
the `activeCircleColliders` and the `passiveCircleColliders`.
After this, it clears the `newCircleColliders` and the `removableCircleColliders`.
```cpp
void RefreshCircleColliders();
```

**RunDynamicColliderPart**  
It iterates on the all dynamic box and circle colliders in [startIndex, endIndex) range
and check these colliders have collision(s) with other dynamic colliders.  
Note: Firstly the function checks the box colliders and after the circle colliders.  
For example: If there are 5 box colliders and 2 circle colliders. The box colliders' index is [0..4] and
the circle colliders' index is [5..6].
```cpp
void RunDynamicColliderPart(size_t startIndex, size_t endIndex);
```

**RunKinematicColliderPart**  
It iterates on the all kinematic box and circle colliders in [startIndex, endIndex) range
and check these colliders have collision(s) with dynamic colliders.  
Note: Firstly the function checks the box colliders and after the circle colliders.  
For example: If there are 5 box colliders and 2 circle colliders. The box colliders' index is [0..4] and
the circle colliders' index is [5..6].
```cpp
void RunKinematicColliderPart(size_t startIndex, size_t endIndex);
```

**RunOnThreads**  
```cpp
void RunOnThreads();
```

**GetDynamicColliderNumber**  
```cpp
inline size_t GetDynamicColliderNumber();
```

**GetKinematicColliderNumber**  
```cpp
inline size_t GetKinematicColliderNumber();
```

**Public:**  
**ColliderComponentHandler**  
```cpp
ColliderComponentHandler();
```

**Add**  
If the isThreadSafe is true, their mutex will be used.
```cpp
void Add(BaseBoxColliderComponent* collider, bool isThreadSafe);
```
```cpp
void Add(BaseCircleColliderComponent* collider, bool isThreadSafe);
```

**Remove**  
If the isThreadSafe is true, their mutex will be used.
```cpp
void Remove(BaseBoxColliderComponent* collider, bool isThreadSafe);
```
```cpp
void Remove(BaseCircleColliderComponent* collider, bool isThreadSafe);
```

**Clear**  
```cpp
void Clear() override;
```

**Run**  
Firstly it calls the RefreshBoxColliders() and RefreshCircleColliders()
functions and it will call the RunActiveColliderPart, RunPassiveColliderPart and/or RunOnThreads functions.  
How many threads will run, it is depend on the number of colliders and maxColliderPerThread.
```cpp
void Run() override;
```

**SetMaxColliderPerThread**  
If it is 0, the class will not use threads.
```cpp
inline void SetMaxColliderPerThread(unsigned int value);
```

**GetMaxColliderPerThread**  
If it is 0, the class will not use threads.
```cpp
inline unsigned int GetMaxColliderPerThread();
```

##
## ColliderMode
### Source Code:
[ColliderMode.h](../../Learning2DEngine/Learning2DEngine/Physics/ColliderMode.h)

### Description:
The collider types.

### Header:
```cpp
enum class ColliderMode
{
    TRIGGER,
    COLLIDER
};
```

##
## ColliderType
### Source Code:
[ColliderType.h](../../Learning2DEngine/Learning2DEngine/Physics/ColliderType.h)

### Description:
The collider types.

### Header:
```cpp
enum class ColliderType
{
    DYNAMIC,
    KINEMATIC
};
```

##
## Collision
### Source Code:
[Collision.h](../../Learning2DEngine/Learning2DEngine/Physics/Collision.h)

### Description:
When 2 object is collided, the `ColliderComponentHandler` calls the objects' OnCollision
 with this type of class.  
 The edges can useful, if the developer want to calculate rebound or similar.

### Header:
```cpp
struct Collision
{
    // The closest point of current object to collided object
    glm::vec2 edge;
    // The closest point of collided object to current object
    glm::vec2 edgeOfCollidedObject;
    System::GameObject* collidedObject;

    Collision(
        glm::vec2 edge,
        glm::vec2 edgeOfCollidedObject,
        System::GameObject* collidedObject)
        : edge(edge), edgeOfCollidedObject(edgeOfCollidedObject),
        collidedObject(collidedObject)
    {

    }
};
```

##
## CollisionHelper
### Source Code:
[CollisionHelper.h](../../Learning2DEngine/Learning2DEngine/Physics/CollisionHelper.h)  
[CollisionHelper.cpp](../../Learning2DEngine/Learning2DEngine/Physics/CollisionHelper.cpp)

### Description:
It is a static class, which has functions to detect the collisions.  
The `ColliderComponentHandler` use it to check every collision.

### Header:
```cpp
class CollisionHelper final
{...}
```

### Auxiliary class
**Private:**  
```cpp
enum HitDirection { UP, RIGHT, DOWN, LEFT };
```

### Functions:
**Private:**  
**CollisionHelper**  
```cpp
CollisionHelper();
```

**GetEdge**  
It returns the closest point of collider object from other object.
```cpp
static glm::vec2 GetEdge(const BaseBoxColliderComponent& boxCollider, glm::vec2 distanceBetweenCenters);
```

```cpp
static glm::vec2 GetEdge(const BaseCircleColliderComponent& circleCollider, glm::vec2 distanceBetweenCenters);
```

**GetDirection**  
```cpp
static HitDirection GetDirection(glm::vec2 vector);
```

**FixPosition**  
It fixed the gameobject's position, which has this collider.  
The fixMultiplier should be 1.0f if another object is Kinematic or 0.5f if it is Dynamic.
```cpp
static void FixPosition(const BaseBoxColliderComponent& boxCollider, glm::vec2 edgeOfCollidedObject, float fixMultiplier);
```

```cpp
static void FixPosition(const BaseCircleColliderComponent& circleCollider, glm::vec2 edgeOfCollidedObject, float fixMultiplier);
```

**GetLength2**  
It does not use sqrt.
```cpp
static inline float GetLength2(glm::vec2 distance);
```

**Public:**  
**CheckCollision**  
It checks, that 2 colliders have been collided.
```cpp
static CollisionData CheckCollision(const BaseBoxColliderComponent& collider1, const BaseBoxColliderComponent& collider2);
```

```cpp
static CollisionData CheckCollision(const BaseCircleColliderComponent& collider1, const BaseCircleColliderComponent& collider2);
```

```cpp
static CollisionData CheckCollision(const BaseCircleColliderComponent& circleCollider, const BaseBoxColliderComponent& boxCollider);
```

```cpp
static CollisionData CheckCollision(const BaseBoxColliderComponent& boxCollider, const BaseCircleColliderComponent& circleCollider);
```

**FixPosition**  
Fix the position of the game objects, if they are dynamic with collider mode
and another collider has collider mode too.  
If both colliders are dynamic with collider mode, they will moved only half of the distance.  
When the function fix the position, it can reset the velocity of the rigidbody if it is inited.
```cpp
template<class T, class U>
static void FixPosition(T& first, glm::vec2 edge1, U& second, glm::vec2 edge2);
```

##
## CollisionData
### Source Code:
[CollisionData.h](../../Learning2DEngine/Learning2DEngine/Physics/CollisionData.h)

### Description:
It contains, that there was a collision or not if yes,
it has the collider edges, where they are collide.  
It is used by `CollisionHelper` and the checker will create
`Collision` for collided objects.

### Header:
```cpp
struct CollisionData
{
    bool isCollided;
    // The closest point of object1 to object2
    glm::vec2 edge1;
    // The closest point of object2 to object1
    glm::vec2 edge2;

    CollisionData(bool isCollided = false,
        glm::vec2 edge1 = glm::vec2(0.0f, 0.0f),
        glm::vec2 edge2 = glm::vec2(0.0f, 0.0f))
    : isCollided(isCollided), edge1(edge1), edge2(edge2)
    {

    }
};
```

##
## Rigidbody
### Source Code:
[Rigidbody.h](../../Learning2DEngine/Learning2DEngine/Physics/Rigidbody.h)  
[Rigidbody.cpp](../../Learning2DEngine/Learning2DEngine/Physics/Rigidbody.cpp)

### Description:
A really basic Rigidbody for moving.

### Header:
```cpp
class Rigidbody : public virtual System::UpdaterComponent, public BaseRigidbody
{...}
```

### Macros:
**L2DE_DEFAULT_GRAVITY**    
Its value is glm::vec2(0.0f, 9.81f).

### Variables:
**Public:**  
**velocity**  
```cpp
glm::vec2 velocity;
```

**isFrozen**  
```cpp
bool isFrozen;
```

**isGravityEnabled**  
```cpp
bool isGravityEnabled;
```

**gravityMultiplier**  
```cpp
float gravityMultiplier;
```

**gravity**  
It's default value is `L2DE_DEFAULT_GRAVITY`.
```cpp
static glm::vec2 gravity;
```

### Functions:
**Protected:**  
**Rigidbody**  
```cpp
Rigidbody(System::GameObject* gameObject, glm::vec2 velocity = glm::vec2(0.0f, 0.0f), bool isGravityEnabled = false, bool isFrozen = false);
```
```cpp
Rigidbody(System::GameObject* gameObject, bool isFrozen);
```

**Update()**  
If the object is not frozen, the position of gameobject will be updated
by `velocity` * `Game::GetDeltaTime()`.
```cpp
virtual void Update() override;
```

**Public:**  
**ResetGravity**  
It resets the gravity to `L2DE_DEFAULT_GRAVITY`.
```cpp
static inline void ResetGravity();
```
