#pragma once
#include <pch.h>
#include "EditorObjectGroup.h"

class EditorTopBar : public EditorObjectGroup  {

    const std::string file_group_name = "File",
    edit_group_name = "Edit",
    view_group_name = "View",
    panel_group_name = "Panel";
    public:

    EditorTopBar() : EditorObjectGroup("EDITOR_MAIN_OBJECT_GROUP") {
        PushObject<EditorObjectGroup>(INT64_MIN,file_group_name);
        PushObject<EditorObjectGroup>(INT64_MIN+1,edit_group_name);
        PushObject<EditorObjectGroup>(INT64_MIN+2,view_group_name);
        PushObject<EditorObjectGroup>(INT64_MIN+3,panel_group_name);
    }
    std::shared_ptr<EditorObjectGroup> GetFileGroup( ) {return GetOrPushGroup(file_group_name);}
    std::shared_ptr<EditorObjectGroup> GetEditGroup( ) {return GetOrPushGroup(edit_group_name);}
    std::shared_ptr<EditorObjectGroup> GetViewGroup( ) {return GetOrPushGroup(view_group_name);}
    std::shared_ptr<EditorObjectGroup> GetPanelsGroup( ) {return GetOrPushGroup(panel_group_name);}

};