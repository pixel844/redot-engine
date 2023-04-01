#include "display_server_3ds.h"
#include "scene/main/scene_tree.h"

DisplayServer3DS::DisplayServer3DS()
{
   gfxInitDefault();
	//gfxSet3D(true);
    consoleInit(GFX_BOTTOM,NULL);
}

DisplayServer3DS::~DisplayServer3DS()
{
    gfxExit();
}

void DisplayServer3DS::process_events() {
    bool run = aptMainLoop();
    hidScanInput();
    if (hidKeysDown() & KEY_SELECT)
    {
        printf("The select key has been pressed\n");
        run = false;
    }

    if (!run)
    {
        SceneTree::get_singleton()->quit();
    }
}

void DisplayServer3DS::swap_buffers(){
    gfxSwapBuffers();
    gspWaitForVBlank();
}

bool DisplayServer3DS::has_feature(Feature p_feature) const
{
    switch (p_feature)
    {
        case FEATURE_SWAP_BUFFERS:
            return true;
        case FEATURE_VIRTUAL_KEYBOARD:
            return true;
        default:
            return false;
    }
}