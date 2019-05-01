#include "Firework.h"

Firework::Firework(const GLfloat gravity, const glm::vec3 &windForce,
  const ExplosionType explosionType, const std::string explosionVertexShaderPath, const std::string explosionFragmentShaderPath,
  const TrailType trailType, const std::string trailVertexShaderPath, const std::string trailFragmentShaderPath,
  const SmokeType smokeType, const std::string smokeVertexShaderPath, const std::string smokeFragmentShaderPath)
{
  this->externalForces = windForce + gravity * glm::vec3(0.0f, 1.0f, 0.0f);

  explosion = nullptr;
  trail = nullptr;
  smoke = nullptr;

  CreateParticleEmitters(
    explosionType, explosionVertexShaderPath, explosionFragmentShaderPath,
    trailType, trailVertexShaderPath, trailFragmentShaderPath,
    smokeType, smokeVertexShaderPath, smokeFragmentShaderPath);

  simulationRunning = false;
}

void Firework::CreateParticleEmitters(
  const ExplosionType explosionType, const std::string explosionVertexShaderPath, const std::string explosionFragmentShaderPath,
  const TrailType trailType, const std::string trailVertexShaderPath, const std::string trailFragmentShaderPath,
  const SmokeType smokeType, const std::string smokeVertexShaderPath, const std::string smokeFragmentShaderPath)
{
  switch (explosionType)
  {
  case DEFAULT_ET:
    CreateDefaultExplosion(explosionVertexShaderPath, explosionFragmentShaderPath);
    break;
  default:
    std::cout << "Unknown explosion type." << std::endl;
  }
  switch (trailType)
  {
  case DEFAULT_TT:
    CreateDefaultTrail(trailVertexShaderPath, trailFragmentShaderPath);
    break;
  default:
    std::cout << "Unknown trail type." << std::endl;
  }
  switch (smokeType)
  {
  case DEFAULT_ST:
    CreateDefaultSmoke(smokeVertexShaderPath, smokeFragmentShaderPath);
    break;
  default:
    std::cout << "Unknown smoke type." << std::endl;
  }
}

Firework::~Firework()
{
  if (explosion != nullptr)
    delete explosion;
  if (trail != nullptr)
    delete trail;
  if (smoke != nullptr)
    delete smoke;
}

void Firework::CreateDefaultExplosion(const std::string vertexShaderPath, const std::string fragmentShaderPath)
{
  explosion = new ParticleEmitter(50, vertexShaderPath, fragmentShaderPath);
  glm::vec3 pmOrigin(0.0f);
  GLfloat pmSphereRadius = 2.0f;
  GLfloat pmParticleSpawnRate = 50;
  GLboolean pmEmitOverTime = false;
  GLboolean pmIsSpinningTrail = false;

  glm::vec3 pmStartSize(1.0f);
  glm::vec3 pmStartColor(1.0f, 0.0f, 0.0f);
  GLfloat pmStartLifetime = 2.0f;
  GLfloat pmStartSpeed = 20.0f;

  glm::vec3 pmGravity(0.0f, -9.8f, 0.0f);

  explosion->SetEmitterVariables(
    pmOrigin, pmSphereRadius, pmParticleSpawnRate, pmEmitOverTime, pmIsSpinningTrail,
    pmStartSize, pmStartColor, pmStartLifetime, pmStartSpeed,
    pmGravity);
}

void Firework::CreateDefaultTrail(const std::string vertexShaderPath, const std::string fragmentShaderPath)
{
  trail = new ParticleEmitter(500, vertexShaderPath, fragmentShaderPath);
  glm::vec3 pmOrigin(0.0f);
  GLfloat pmSphereRadius = 0.4f;
  GLfloat pmParticleSpawnRate = 200;
  GLboolean pmEmitOverTime = true;
  GLboolean pmIsSpinningTrail = false;

  glm::vec3 pmStartSize = glm::vec3(1.5f);
  glm::vec3 pmStartColor = glm::vec3(0.529f, 0.808f, 0.922f);
  GLfloat pmStartLifetime = 1.0f;
  GLfloat pmStartSpeed = 0.0f;

  glm::vec3 pmGravity = glm::vec3(0.0f, -9.8f, 0.0f);

  trail->SetEmitterVariables(
    pmOrigin, pmSphereRadius, pmParticleSpawnRate, pmEmitOverTime, pmIsSpinningTrail,
    pmStartSize, pmStartColor, pmStartLifetime, pmStartSpeed,
    pmGravity);
}

void Firework::CreateDefaultSmoke(const std::string vertexShaderPath, const std::string fragmentShaderPath)
{

}

void Firework::ConfigureShaders(const float width, const float height)
{
  if (explosion != nullptr)
    explosion->ConfigureShader(width, height);
  if (trail != nullptr)
    trail->ConfigureShader(width, height);
  if (smoke != nullptr)
    smoke->ConfigureShader(width, height);
}

void Firework::Start(const glm::vec3 &origin, const glm::vec3 &initialVelocity)
{
  simulationRunning = true;
  explosionStarted = false;
  duration = 0;
  mainBodyPos = origin;
  velocity = initialVelocity;
  if (trail != nullptr)
    trail->Start();
}

void Firework::Update(const GLfloat deltaTime)
{
  if (!simulationRunning)
    return;

  duration += deltaTime;

  velocity += deltaTime * externalForces;
  mainBodyPos += deltaTime * velocity;

  if (trail != nullptr)
  {
    trail->SetOrigin(mainBodyPos);
    trail->Update(deltaTime);
  }
  if (!explosionStarted && duration > 1.1f)
  {
    if (explosion != nullptr)
    {
      explosion->SetOrigin(mainBodyPos);
      explosion->Start();
    }
    explosionStarted = true;
  }
  if (explosion != nullptr)
    explosion->Update(deltaTime);
}

void Firework::Draw()
{
  if (!simulationRunning)
    return;

  if (trail != nullptr)
    trail->Draw();
  if (explosion != nullptr)
    explosion->Draw();
}

void Firework::Stop()
{
  simulationRunning = false;
  if (explosion != nullptr)
    explosion->Stop();
  if (trail != nullptr)
    trail->Stop();
  if (smoke != nullptr)
    smoke->Stop();
}