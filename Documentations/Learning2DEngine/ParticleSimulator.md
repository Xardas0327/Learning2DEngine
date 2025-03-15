# ParticleSimulator
- [BasicParticleSettings](ParticleSimulator.md#basicparticlesettings)
- [Particle](ParticleSimulator.md#particle)
- [ParticleRenderData](ParticleSimulator.md#particlerenderdata)
- [ParticleRenderer](ParticleSimulator.md#particlerenderer)
- [ParticleSettings](ParticleSimulator.md#particlesettings)
- [ParticleSystemComponent](ParticleSimulator.md#particlesystemcomponent)
- [ParticleSystemSettings](ParticleSimulator.md#particlesystemsettings)

##
## BasicParticleSettings
### Source Code:
[BasicParticleSettings.h](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/BasicParticleSettings.h)

### Description:
It just basic implementation of the `ParticleSettings`.
The `ParticleSystemComponent` uses this `ParticleSettings` if the developer does not give another one.  
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
## ParticleRenderData
### Source Code:
[ParticleRenderData.h](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/ParticleRenderData.h)

### Description:
It contains the data, which is important to render the particles.

### Header:
```cpp
class ParticleRenderData final : public Render::RenderData
{...}
```

### Variables:
**Private:**  
**isRenderable**
```cpp
bool isRenderable;
```

**particleAmount**
```cpp
unsigned int particleAmount;
```

**particles**  
The developer has to init it, when they didn't give the particleAmount
```cpp
Particle* particles;
```

**Public:**  
**systemSettings**
```cpp
ParticleSystemSettings systemSettings;
```

**texture**
```cpp
Render::Texture2D* texture;
```

### Functions:
**Public:**  
**ParticleRenderData**  
```cpp
ParticleRenderData(const System::Component* component);
```
```cpp
ParticleRenderData(const System::Component* component, unsigned int particleAmount, const ParticleSystemSettings& systemSettings, const Render::Texture2D& texture)
```

**~ParticleRenderData**  
```cpp
~ParticleRenderData() override;
```

**IsUseTexture**  
```cpp
inline bool IsUseTexture() const;
```

**ClearTexture**  
```cpp
inline void ClearTexture();
```

**IsRenderable**  
```cpp
inline bool IsRenderable() const;
```

**GetParticles**  
```cpp
inline const Particle* GetParticles() const;
```

**GetParticleAmount**  
```cpp
inline unsigned int GetParticleAmount() const;
```

##
## ParticleRenderer
### Source Code:
[ParticleRenderer.h](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/ParticleRenderer.h)

### Description:
It can render the particles.
Note: The projection and the view matrix are came from Game::mainCamera.

### Header:
```cpp
class ParticleRenderer : public Render::IRenderer, public virtual System::Singleton<ParticleRenderer>
{...}
```

### Variables:
**Private:**  
**shader**
```cpp
Render::Shader shader;
```

**vao**
```cpp
GLuint vao;
```

**vbo**
```cpp
GLuint vbo;
```

**ebo**
```cpp
GLuint ebo;
```

### Functions:
**Protected:**  
**ParticleRenderer**  
```cpp
ParticleRenderer();
```

**InitShader**  
```cpp
void InitShader();
```

**InitVao**  
```cpp
void InitVao();
```

**Public:**  
**Init**  
```cpp
void Init() override;
```

**Destroy**  
```cpp
void Destroy() override;
```

**Draw**  
```cpp
void Draw(std::vector<Render::RenderData*> renderData) override;
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
It will run in the `ParticleSystemComponent::Start()`.
```cpp
virtual void Init(const System::GameObject& gameObject);
```

**Destroy**  
It can destroy everything, which was generated or cached in the other functions. 
It will run in the `ParticleSystemComponent::Stop()`.
```cpp
virtual void Destroy();
```

**SpawnParticle**  
It is an abstract function, which the developer has to define.
The developer has to set the life time of the particles here and any other parameters,
which the developer want to initialize on the particle.  
It will run in the `ParticleSystemComponent::TryToSpawnNewParticles()`, which is in `ParticleSystemComponent::Update()`.
```cpp
virtual void SpawnParticle(Particle& particle, const System::GameObject& gameObject) = 0;
```

**UpdateParticle**  
It is an abstract function, which the developer has to define.
This function will be called with that particle, which life time is bigger than 0, after it was decreased.  
It will run in the `ParticleSystemComponent::UpdateActiveParticles()`, which is in `ParticleSystemComponent::Update()`.
```cpp
virtual void UpdateParticle(Particle& particle, const System::GameObject& gameObject) = 0;
```

##
## ParticleSystemComponent
### Source Code:
[ParticleSystemComponent.h](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/ParticleSystemComponent.h)  
[ParticleSystemComponent.cpp](../../Learning2DEngine/Learning2DEngine/ParticleSimulator/ParticleSystemComponent.cpp)

### Description:
This is the heart of the Engine's particle system.
It control the particles by the `ParticleSettings` and the `ParticleSystemSettings`.  
Please check the `Render::RendererComponent` and `System::UpdaterComponent` documentation too.

### Header:
```cpp
class ParticleSystemComponent final : public virtual Render::RendererComponent<ParticleRenderData, ParticleRenderer>, public virtual System::UpdaterComponent
{...}
```

### Variables:
**Private:**  
**isRunning**  
```cpp
bool isRunning;
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

**mutex**  
```cpp
std::mutex mutex;
```

**particleSettings**  
```cpp
ParticleSettings* particleSettings;
```

**id**  
It is used by the `Render::RendererComponentHandler`.
```cpp
static const std::string id;
```

**refrenceNumber**  
It is counted, that how many ParticleSystem exist.
```cpp
static int refrenceNumber;
```

### Functions:
**Private:**  
**ParticleSystemComponent**  
```cpp
ParticleSystemComponent(System::GameObject* gameObject, unsigned int particleAmount, ParticleSettings* particleSettings = nullptr, int renderLayer = 0);
```
```cpp
ParticleSystemComponent(System::GameObject* gameObject, unsigned int particleAmount, const Render::Texture2D& texture, const ParticleSystemSettings& systemSettings, ParticleSettings* particleSettings = nullptr, int renderLayer = 0);
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
It update and create particle, when it has to do.
It works only, after the `Start()` function was called and before the `Stop()`.
```cpp
void Update() override;
```

**GetId**  
```cpp
const std::string& GetId() const override;
```

**GetRenderer**  
```cpp
ParticleRenderer* GetRenderer() const override;
```

**DestroyObject**  
The `ParticleSystemComponent::Destroy()` call it with or without mutex.
```cpp
void DestroyObject();
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
**~ParticleSystemComponent**  
If the destructor is called. It will call the `Stop()` function
and it will delete  the `particleSettings` pointers.  
```cpp
~ParticleSystemComponent() override;
```

**Start**  
It sets everything for the running.
```cpp
void Start();
```

**Stop**  
It stops the `ParticleSystemComponent`'s running.
```cpp
void Stop();
```
**Restart**  
It stops and start `ParticleSystemComponent` immeditaly.  
(It calls the `Stop()` and the `Start()` functions.)
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