#include <iostream>
#include "Landmark.hpp"


int main(void) {
    //OBJ_File_content obj_file;
    //MeshFile::ParseOBJ(IO::GetResources().GetFolder("models").GetFile("sphere_high.obj"), obj_file);
    //return 0;
    App app;
    AppProperties app_properties;
    app.Run(app_properties);
    return 0;
}