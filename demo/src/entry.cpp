#include <iostream>
#include "Landmark.h"
#include "Containers/ObjectAllocator-new.h"
class DemoApp : public Landmark::IAttachableModule {
    public:
    DemoApp(){
        
    };

    void Init() override {
        auto camera = EntityManager::CreateEntity("camera");
	    camera->AddComponent<Transform>();
	    camera->AddComponent<Camera>();
        auto entity = EntityManager::CreateEntity("Cube");
        entity->AddComponent<Transform>();
        
    }
    void PreRender() override {}
    void Update() override {}
    void Exit() override {}
};


int main(void) {

    std::cout << "test\n";
    Landmark::Application app({"Demo"});
    app.AttachModule<DemoApp>();
    app.Run();
    return 0;
}