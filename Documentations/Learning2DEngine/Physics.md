# Physics
This namespace really simple. It has only some really basic functionality.

- [BoxCollider](Physics.md#boxcollider)
- [CircleCollider](Physics.md#circlecollider)
- [Collider](Physics.md#collider)
- [Collision](Physics.md#collision)
- [CollisionData](Physics.md#collisiondata)
- [Rigidbody](Physics.md#rigidbody)

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
## Collider
### Source Code:
[Collider.h](../../Learning2DEngine/Learning2DEngine/Physics/Collider.h)

### Description:
The `Collider` is really basic. It does not recommended to use.
The developer should use the `BoxCollider` and/or `CircleCollider`.

### Header:
```cpp
class Collider : public virtual System::Component
{...}
```

### Variables:
**Public:**  
**offset**
```cpp
glm::vec2 offset;
```

### Functions:
**Protected:**  
**Collider**  
```cpp
Collider(System::GameObject* gameObject, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
```

**Public:**  
**GetCenter**  
It returns the center of Collider, which is middle of the object with the offset.
```cpp
glm::vec2 GetCenter() const;
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
