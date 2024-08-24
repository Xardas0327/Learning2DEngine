# ParticleSimulator
- [BasicParticleSettings](ParticleSimulator.md#basicparticlesettings)
- [Particle](ParticleSimulator.md#particle)
- [ParticleSettings](ParticleSimulator.md#particlesettings)
- [ParticleSystem](ParticleSimulator.md#particlesystem)
- [ParticleSystemSettings](ParticleSimulator.md#particlesystemsettings)

##
## BasicParticleSettings
### Source Code:
[BasicParticleSettings.h](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/BasicParticleSettings.h)

### Description:
It just basic implementation of the `ParticleSettings`.
The `ParticleSystem` uses this `ParticleSettings` if the developer does not give another one.  
Please check the `ParticleSettings` too.

### Header:
```cpp
struct BasicParticleSettings : public virtual ParticleSettings
{...}
```

### Variables:
**Public:**  
**fixPositionOffset**
```cpp
glm::vec2 fixPositionOffset;
```

**randomPositionOffset**  
First is the min.  
Second is the max.
```cpp
std::pair<glm::vec2, glm::vec2> randomPositionOffset;
```

**scale**  
```cpp
glm::vec2 scale;
```

**rotation**  
```cpp
float rotation;
```

**velocity**  
```cpp
glm::vec2 velocity;
```

**color**  
```cpp
glm::vec4 color;
```

**lifeTime**  
This is in seconds.
```cpp
float lifeTime;
```

**speed**  
```cpp
float speed;
```

### Functions:
**Public:**  
**BasicParticleSettings**  
```cpp
BasicParticleSettings();
```

**SpawnParticle**  
```cpp
void SpawnParticle(Particle& particle, const System::GameObject& gameObject) override;
```

**UpdateParticle**  
```cpp
void UpdateParticle(Particle& particle, const System::GameObject& gameObject) override;
```

##
## Particle
### Source Code:
[Particle.h](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/Particle.h)

### Description:
The particles representation in the Engine.

### Header:
```cpp
struct Particle
{
    System::Transform transform;
    glm::vec2 velocity;
    glm::vec4 color;
    //In seconds. The negative number is same as 0.
    float lifeTime;
    float speed;

    Particle() 
        : transform(), velocity(0.0f), color(1.0f), lifeTime(0.0f), speed(1.0f)
    {
    }
};
```

##
## ParticleSettings
### Source Code:
[ParticleSettings.h](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/ParticleSettings.h)

### Description:
The `ParticleSettings` contains, how a particle should be initialized,
when it is spawned or how it should be updated frame by frame.

### Header:
```cpp
struct ParticleSettings
{...}
```

### Functions:
**Public:**  
**~ParticleSettings**  
```cpp
virtual ~ParticleSettings();
```

**Init**  
It is initialized the ParticleSettings.
It can be useful, if the developer want to generate or cache some data for the particles.  
It will run in the `ParticleSystem::Start()`.
```cpp
virtual void Init(const System::GameObject& gameObject);
```

**Destroy**  
It can destroy everything, which was generated or cached in the other functions. 
It will run in the `ParticleSystem::Stop()`.
```cpp
virtual void Destroy();
```

**SpawnParticle**  
It is an abstract function, which the developer has to define.
The developer has to set the life time of the particles here and any other parameters,
which the developer want to initialize on the particle.  
It will run in the `ParticleSystem::TryToSpawnNewParticles()`, which is in `ParticleSystem::Update()`.
```cpp
virtual void SpawnParticle(Particle& particle, const System::GameObject& gameObject) = 0;
```

**UpdateParticle**  
It is an abstract function, which the developer has to define.
This function will be called with that particle, which life time is bigger than 0, after it was decreased.  
It will run in the `ParticleSystem::UpdateActiveParticles()`, which is in `ParticleSystem::Update()`.
```cpp
virtual void UpdateParticle(Particle& particle, const System::GameObject& gameObject) = 0;
```

##
## ParticleSystem
### Source Code:
[ParticleSystem.h](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/ParticleSystem.h)  
[ParticleSystem.cpp](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/ParticleSystem.cpp)

### Description:
This is the heart of the Engine's particle system.
It control the particles by the `ParticleSettings` and the `ParticleSystemSettings`.  
`Update()` update the particles.
This should be called in the `Game::Update()` only ones.
It and `Draw()` work only, when the `IsRunning()` is true.

### Header:
```cpp
class ParticleSystem : public virtual Render::Renderer
{...}
```

### Variables:
**Protected:**  

**isInit**  
```cpp
bool isInit;
```

**isRunning**  
```cpp
bool isRunning;
```

**particleAmount**  
```cpp
unsigned int particleAmount;
```

**particles**  
```cpp
Particle* particles;
```

**delayTime**  
```cpp
float delayTime;
```

**nextSpawnTime**  
```cpp
float nextSpawnTime;
```

**lastUsedParticleIndex**  
```cpp
unsigned int lastUsedParticleIndex;
```

**referenceNumber**  
All `ParticleSystems` use the same shader and vertex array object.
That's why it is counted, that how many `ParticleSystems` there are in the game.
It is important, that the shader will be created if it is used and
it will be destroyed if nothing uses it.
```cpp
static int referenceNumber;
```

**shader**  
```cpp
static Render::Shader shader;
```

**vao**  
```cpp
static unsigned int vao;
```

**vbo**  
```cpp
static unsigned int vbo;
```

**ebo**  
```cpp
static unsigned int ebo;
```

**particleSettings**  
```cpp
ParticleSettings* particleSettings;
```

**Public:**  
**systemSettings**  
The systemSettings is a public variable, that the developer can change its values any time.
```cpp
ParticleSystemSettings systemSettings;
```

**texture**  
The texture, which the particle will use.
This can be useful if the developer want to change anything in the texture.
But if the developer want to remove it, they should use the `ClearTexture()` function.
```cpp
Render::Texture2D* texture;
```

### Functions:
**Protected:**  
**ParticleSystem**  
```cpp
ParticleSystem(
    System::GameObject* gameObject,
    unsigned int particleAmount,
    ParticleSettings* particleSettings = nullptr);
```
```cpp
ParticleSystem(
    System::GameObject* gameObject,
    unsigned int particleAmount,
    const Render::Texture2D& texture,
    const ParticleSystemSettings& systemSettings,
    ParticleSettings* particleSettings = nullptr);
```

**Init**  
```cpp
void Init() override;
```

**Destroy**  
```cpp
void Destroy() override;
```

**InitShader**  
```cpp
void InitShader();
```

**InitVao**  
```cpp
void InitVao();
```

**UpdateActiveParticles**  
```cpp
void UpdateActiveParticles();
```

**TryToSpawnNewParticles**  
```cpp
void TryToSpawnNewParticles();
```

**SpawnNewParticles**  
```cpp
void SpawnNewParticles();
```

**GetUnusedParticleIndex**  
```cpp
unsigned int GetUnusedParticleIndex();
```

**Public:**  
**~ParticleSystem**  
If the destructor is called. It will call the `Stop()` function
and it will delete the `texture` and the `particleSettings` pointers.
```cpp
~ParticleSystem();
```

**Draw**  
It draws the particles.  
Please check the `Render::Renderer` documentation too.
```cpp
void Draw() override;
```

**Start**  
It set everything for the running.
```cpp
void Start();
```

**Update**  
It update and create particle, when it has to do.
It works only, after the `Start()` function was called and before the `Stop()`.
```cpp
void Update();
```

**Stop**  
It stop the `ParticleSystem`'s running.
```cpp
void Stop();
```

**Restart**  
It stop and start `ParticleSystem` immeditaly.  
(It calls the `Stop()` and the `Start()` function)
```cpp
inline void Restart();
```

**IsRunning**  
It returns true, when the ParticleSystem is running.
```cpp
inline bool IsRunning();
```

**GetParticleSettings**  
It gets back the ParticleSettings, that the developer can change its parameters.
```cpp
inline ParticleSettings* const GetParticleSettings();
```

**IsUseTexture**  
It returns true if the `texture` is not null.
```cpp
inline bool IsUseTexture();
```

**ClearTexture**  
It destroys the `texture`.
```cpp
void ClearTexture();
```

##
## ParticleSystemSettings
### Source Code:
[ParticleSystemSettings.h](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/ParticleSystemSettings.h)  

### Description:
The `ParticleSystemSettings` contains how should work a `ParticleSystem`.
How common it should spawn particles and how many they should be spawned
at the sometime.


### Header:
```cpp
struct ParticleSystemSettings
{
	bool isUseBlend;
	Render::BlendFuncFactor blendFuncFactor;
	/// <summary>
	/// In seconds. The negative number is same as 0.
	/// If it is 0, it will spawn particles in every frame, which is not recommended.
	/// </summary>
	float spawnTime;
	unsigned int newParticlesPerSpawn;
	//In seconds. The negative number is same as 0.
	float delayBeforeStart;

	ParticleSystemSettings(
		bool isUseBlend = false,
		Render::BlendFuncFactor blendFuncFactor = Render::BlendFuncFactor(GL_ONE, GL_ZERO),
		float spawnTime = 1.0f, unsigned int newParticlesPerSpawn = 1, float delayBeforeStart = 0.0f)
		: isUseBlend(isUseBlend), blendFuncFactor(blendFuncFactor), spawnTime(spawnTime),
		newParticlesPerSpawn(newParticlesPerSpawn), delayBeforeStart(delayBeforeStart)
	{
	}
};
```