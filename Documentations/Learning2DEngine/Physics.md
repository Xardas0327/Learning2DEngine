# Physics
This namespace really simple. It has only some really basic functionality.

- [BaseBoxColliderComponent](Physics.md#baseboxcollidercomponent)
- [BaseCircleColliderComponent](Physics.md#basecirclecollidercomponent)
- [BaseColliderComponent](Physics.md#basecollidercomponent)
- [BoxColliderComponent](Physics.md#boxcollidercomponent)
- [CircleColliderComponent](Physics.md#circlecollidercomponent)
- [ColliderComponentHandler](Physics.md#collidercomponenthandler)
- [Collision](Physics.md#collision)
- [CollisionChecker](Physics.md#collisionchecker)
- [CollisionData](Physics.md#collisiondata)
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
BaseBoxColliderComponent(System::GameObject* gameObject, glm::vec2 size, bool isActiveCollider = true, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
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
BaseCircleColliderComponent(System::GameObject* gameObject, float radius, bool isActiveCollider = true, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
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
**isActiveCollider**  
Note: The isActive is a different variable, which came from `System::Component`.  
A collider can be active or passive. 2 passive colliders can't collide with each other, 
but an active collider can collide with them.  
For example: there are 3 circles (A, B and C) with 1.0f range. A is active and another two are passive.
If they are on the same position, the A will be triggered twice (one with B and one with C),
but the B and the C will be triggered with A only.
```cpp
const bool isPassive;
```

**Public:**  
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
BaseColliderComponent(System::GameObject* gameObject, bool isActiveCollider = true, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
```

**Public:**  
**GetColliderCenter**  
It returns the center of Collider, which is middle of the object by shifted the offset.
```cpp
glm::vec2 GetColliderCenter() const;
```

**IsPassive**  
```cpp
inline bool IsPassive() const;
```

**OnCollision**  
If 2 colliders trigger each other, their OnCollision function will be called.  
It does nothing by default.
```cpp
virtual void OnCollision(Collision collision);
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

### Functions:
**Protected:**  
**BoxColliderComponent**  
```cpp
BoxColliderComponent(System::GameObject* gameObject, glm::vec2 size, bool isActiveCollider = true, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
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

### Functions:
**Protected:**  
**CircleColliderComponent**  
```cpp
CircleColliderComponent(System::GameObject* gameObject, float radius, bool isActiveCollider = true, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
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
**activeBoxColliders**  
```cpp
std::vector<BaseBoxColliderComponent*> activeBoxColliders;
```

**passiveBoxColliders**  
```cpp
std::vector<BaseBoxColliderComponent*> passiveBoxColliders;
```

**newBoxColliders**  
```cpp
std::vector<BaseBoxColliderComponent*> newBoxColliders;
```

**removableBoxColliders**  
```cpp
std::vector<BaseBoxColliderComponent*> removableBoxColliders;
```

**activeCircleColliders**  
```cpp
std::vector<BaseCircleColliderComponent*> activeCircleColliders;
```

**passiveCircleColliders**  
```cpp
std::vector<BaseCircleColliderComponent*> passiveCircleColliders;
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

**RunActiveColliderPart**  
It iterates on the all active box and circle colliders in [startIndex, endIndex) range
and check these colliders have collision(s) with other active colliders.  
Note: Firstly the function checks the box colliders and after the circle colliders.  
For example: If there are 5 box colliders and 2 circle colliders. The box colliders' index is [0..4] and
the circle colliders' index is [5..6].
```cpp
void RunActiveColliderPart(size_t startIndex, size_t endIndex);
```

**RunPassiveColliderPart**  
It iterates on the all passive box and circle colliders in [startIndex, endIndex) range
and check these colliders have collision(s) with active colliders.  
Note: Firstly the function checks the box colliders and after the circle colliders.  
For example: If there are 5 box colliders and 2 circle colliders. The box colliders' index is [0..4] and
the circle colliders' index is [5..6].
```cpp
void RunPassiveColliderPart(size_t startIndex, size_t endIndex);
```

**RunOnThreads**  
```cpp
void RunOnThreads();
```

**GetActiveColliderNumber**  
```cpp
inline size_t GetActiveColliderNumber();
```

**GetPassiveColliderNumber**  
```cpp
inline size_t GetPassiveColliderNumber();
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
## CollisionChecker
### Source Code:
[CollisionChecker.h](../../Learning2DEngine/Learning2DEngine/Physics/CollisionChecker.h)

### Description:
It is a static class, which has functions to detect the collisions.  
The `ColliderComponentHandler` use it to check every collision.

### Header:
```cpp
class Collision final
{...}
```

### Functions:
**Private:**  
**Collision**  
```cpp
Collision();
```

**GetEdge**  
It returns the closest point of collider object from other object.
```cpp
static glm::vec2 GetEdge(const BaseBoxColliderComponent& boxCollider, glm::vec2 distanceBetweenCenters);
```

```cpp
static glm::vec2 GetEdge(const BaseCircleColliderComponent& circleCollider, glm::vec2 distanceBetweenCenters);
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

##
## CollisionData
### Source Code:
[CollisionData.h](../../Learning2DEngine/Learning2DEngine/Physics/CollisionData.h)

### Description:
It contains, that there was a collision or not if yes,
it has the collider edges, where they are collide.  
It is used by `CollisionChecker` and the checker will create
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

### Description:
A really basic Rigidbody for moving.

### Header:
```cpp
class Rigidbody : public virtual System::UpdaterComponent
{...}
```

### Variables:
**Public:**  
**velocity**  
Velocity of the object.
```cpp
glm::vec2 velocity;
```

**isFrozen**  
The object is frozen or not.
```cpp
bool isFrozen;
```

### Functions:
**Protected:**  
**Rigidbody**  
```cpp
Rigidbody(System::GameObject* gameObject, glm::vec2 velocity = glm::vec2(0.0f, 0.0f), bool isFrozen = false);
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
