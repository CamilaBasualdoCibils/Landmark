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

    App app;
    app.AttachModule<DemoModule>();

    AppProperties app_properties;
    app_properties.app_name = "Demo App";
    app.Run(app_properties);
    return 0;
}