#include "../tama_mem_app_i.h"

static void tama_mem_scene_verify_error_widget_callback(
    GuiButtonType result,
    InputType type,
    void* context) {
    SPIMemApp* app = context;
    if(type == InputTypeShort) {
        view_dispatcher_send_custom_event(app->view_dispatcher, result);
    }
}

void tama_mem_scene_verify_error_on_enter(void* context) {
    SPIMemApp* app = context;
    widget_add_button_element(
        app->widget, GuiButtonTypeLeft, "Back", tama_mem_scene_verify_error_widget_callback, app);
    widget_add_string_element(
        app->widget, 64, 9, AlignCenter, AlignBottom, FontPrimary, "Verification error");
    widget_add_string_element(
        app->widget, 64, 21, AlignCenter, AlignBottom, FontSecondary, "Data mismatch");
    view_dispatcher_switch_to_view(app->view_dispatcher, SPIMemViewWidget);
}

bool tama_mem_scene_verify_error_on_event(void* context, SceneManagerEvent event) {
    SPIMemApp* app = context;
    bool success = false;
    if(event.type == SceneManagerEventTypeBack) {
        success = true;
        scene_manager_search_and_switch_to_previous_scene(
            app->scene_manager, SPIMemSceneChipDetect);
    } else if(event.type == SceneManagerEventTypeCustom) {
        success = true;
        if(event.event == GuiButtonTypeLeft) {
            scene_manager_search_and_switch_to_previous_scene(
                app->scene_manager, SPIMemSceneChipDetect);
        }
    }
    return success;
}
void tama_mem_scene_verify_error_on_exit(void* context) {
    SPIMemApp* app = context;
    widget_reset(app->widget);
}

