//
// Created by camila on 11/10/25.
//

#ifndef LANDMARK_LANDMARKENGINE_HPP
#define LANDMARK_LANDMARKENGINE_HPP



#include "Utility/Singleton.hpp"
#include "Graphics/Graphics.hpp"

class LandmarkEngine : public Singleton<LandmarkEngine>{
public:
    struct Settings {
        Graphics::APIMode mode;
    };
private:
    Settings _settings;
    bool ShouldClose = false;
public:
    LandmarkEngine();

    void Init(const Settings& settings);
    void Run();

    bool GetShouldClose() const;
    void SetShouldClose(bool value);
};


#endif //LANDMARK_LANDMARKENGINE_HPP