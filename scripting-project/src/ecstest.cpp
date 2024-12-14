#include "iscript.h"
#include "scriptregistry.h"

#include "FlexECS/datastructures.h"
using namespace FlexEngine::FlexECS;

#include "components.h"

#include <iostream>

class ECSTestScript : public Script
{
public:
  ECSTestScript()
  {
    ScriptRegistry::RegisterScript(this);
  }

  void Start() override
  {
    std::cout << "ECSTest: Start" << std::endl;

    std::shared_ptr<Scene> scene = Scene::CreateScene();
    Scene::SetActiveScene(scene);
    Entity entity = Scene::CreateEntity("Test Entity");
    entity.AddComponent<Vec2>({ 1.0f, 2.0f });
    std::cout << "entity.HasComponent<Vec2>(): " << std::boolalpha << entity.HasComponent<Vec2>() << std::endl;
    Vec2& vec2 = *entity.GetComponent<Vec2>();
    vec2.Print();
    vec2.Invert();
    std::string name = Scene::GetActiveScene()->Internal_StringStorage_Get(*entity.GetComponent<Scene::StringIndex>());
    std::cout << "name: " << name << std::endl;
  }

  void Stop() override
  {
    std::cout << "ECSTest: Stop" << std::endl;
  }

  std::string GetName() const override
  {
    return "ECSTest";
  }
};

// Static instance to ensure registration
static ECSTestScript ECSTest;
