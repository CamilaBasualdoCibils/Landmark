#include <iostream>
#include "Landmark.hpp"

class DemoModule : public AppModule {
    public:
    DemoModule(){
        
    };
    std::vector<EngineCallInject> GetInjections() override{
       
        return {};}

    
};


int main(void) {
    //OBJ_File_content obj_file;
    //MeshFile::ParseOBJ(IO::GetResources().GetFolder("models").GetFile("sphere_high.obj"), obj_file);
    //return 0;
    App app;
    app.AttachModule<DemoModule>();

    AppProperties app_properties;
    app_properties.app_name = "Demo App";
    app.Run(app_properties);
    return 0;
}