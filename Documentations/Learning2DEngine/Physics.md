# Physics
This namespace really simple. It has only some really basic functionality.

- [BaseColliderComponent](Physics.md#basecollidercomponent)
- [BoxCollider](Physics.md#boxcollider)
- [CircleCollider](Physics.md#circlecollider)
- [Collision](Physics.md#collision)
- [CollisionData](Physics.md#collisiondata)
- [Rigidbody](Physics.md#rigidbody)

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
**isPassive**  
Note: The isActive is a different variable, which came from `System::Component`.  
A collider can be passive or active. 2 passive colliders can't collide with each other, 
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
BaseColliderComponent(System::GameObject* gameObject, bool isPassive, glm::vec2 offset = glm::vec2(0.0f, 0.0f), int32_t maskLayer = ~0);
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
If 2 colliders trigger each other, Their OnCollision function will be called.
```cpp
virtual void OnCollision(Collision collision);
```


##
## BoxCollider
### Source Code:
[BoxCollider.h](../../Learning2DEngine/Learning2DEngine/Physics/BoxCollider.h)

### Description:
The `BoxCollider` is really basic. It doesn't rotate, scale with the gameobject.  
Please check the `Collider` class documentation too.

### Header:
```cpp
class BoxCollider : public virtual Collider
{...}
```

### Variables:
**Public:**  
**size**
```cpp
glm::vec2 size;
```

### Functions:
**Protected:**  
**BoxCollider**  
```cpp
BoxCollider(System::GameObject* gameObject, glm::vec2 size, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
```

##
## CircleCollider
### Source Code:
[CircleCollider.h](../../Learning2DEngine/Learning2DEngine/Physics/CircleCollider.h)

### Description:
The `CircleCollider` is really basic. It doesn't rotate, scale with the gameobject.  
Please check the `Collider` class documentation too.

### Header:
```cpp
class CircleCollider : public virtual Collider
{...}
```

### Variables:
**Public:**  
**radius**
```cpp
float radius;
```

### Functions:
**Protected:**  
**CircleCollider**  
```cpp
CircleCollider(System::GameObject* gameObject, float radius, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
```

##
## Collision
### Source Code:
[Collision.h](../../Learning2DEngine/Learning2DEngine/Physics/Collision.h)

### Description:
It is a static class, which has functions to detect the collisions.

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
static glm::vec2 GetEdge(const BoxCollider& boxCollider, glm::vec2 distanceBetweenCenters);
```

```cpp
static glm::vec2 GetEdge(const CircleCollider& circleCollider, glm::vec2 distanceBetweenCenters);
```

**Public:**  
**IsCollisoned**  
It checks, that 2 colliders have been collided.
```cpp
static CollisionData IsCollisoned(const BoxCollider& collider1, const BoxCollider& collider2);
```

```cpp
static CollisionData IsCollisoned(const CircleCollider& collider1, const CircleCollider& collider2);
```

```cpp
static CollisionData IsCollisoned(const CircleCollider& circleCollider, const BoxCollider& boxCollider);
```

##
## CollisionData
### Source Code:
[CollisionData.h](../../Learning2DEngine/Learning2DEngine/Physics/CollisionData.h)

### Description:
It contains, that there was a collision or not.
If yes, it has the collider edges, where they are collide.
The edges can useful, if the developer want to calculate rebound or similar.

### Header:
```cpp
struct CollisionData
{
    bool isCollisoned;
    // The closest point of object1 to object2
    glm::vec2 edge1;
    // The closest point of object2 to object1
    glm::vec2 edge2;

    CollisionData(bool isCollisoned = false,
        glm::vec2 edge1 = glm::vec2(0.0f, 0.0f),
        glm::vec2 edge2 = glm::vec2(0.0f, 0.0f))
    : isCollisoned(isCollisoned), edge1(edge1), edge2(edge2)
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
class Rigidbody : public virtual System::Component
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
Rigidbody(System::GameObject* gameObject, glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
```
```cpp
Rigidbody(System::GameObject* gameObject, bool isFrozen);
```

**Public:**  
**Update()**  
If the object is not frozen, the position of gameobject will be updated
by `velocity` * `Game::GetDeltaTime()`.
```cpp
void Update();
```
