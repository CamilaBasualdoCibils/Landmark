
#include <iostream>
#include "LandmarkEngine.hpp"
int main() {
    LandmarkEngine::Settings settings;
    LandmarkEngine::Get().Init(settings);
    LandmarkEngine::Get().Run();

}
