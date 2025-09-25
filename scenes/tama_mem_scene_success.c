#include "../tama_mem_app_i.h"

static void tama_mem_scene_success_widget_callback(
    GuiButtonType result,
    InputType type,
    void* context) {
    SPIMemApp* app = context;
    UNUSED(result);
    if(type == InputTypeShort) {
        view_dispatcher_send_custom_event(app->view_dispatcher, SPIMemCustomEventSuccessOk);
    }
}

static void tama_mem_scene_success_vibro_timer_callback(void* context) {
    SPIMemApp* app = context;
    notification_message(app->notifications, &sequence_reset_vibro);
    app->vibro_timer = NULL;
}

void tama_mem_scene_success_on_enter(void* context) {
    SPIMemApp* app = context;
    notification_message(app->notifications, &sequence_display_backlight_on);
    notification_message(app->notifications, &sequence_blink_start_blue);
    
    // Turn on vibration
    notification_message(app->notifications, &sequence_set_vibro_on);
    
    // Create a timer to turn off vibration after 0.2 seconds (200ms)
    if(app->vibro_timer) {
        furi_timer_free(app->vibro_timer);
    }
    app->vibro_timer = furi_timer_alloc(tama_mem_scene_success_vibro_timer_callback, FuriTimerTypeOnce, app);
    furi_timer_start(app->vibro_timer, 200);
    
    // Use widget instead of popup to show OK button clearly
    widget_add_icon_element(app->widget, 16, 5, &I_DolphinNice_96x59);
    widget_add_string_element(app->widget, 5, 7, AlignLeft, AlignTop, FontPrimary, "Success!");
    widget_add_button_element(
        app->widget,
        GuiButtonTypeCenter,
        "OK",
        tama_mem_scene_success_widget_callback,
        app);
    view_dispatcher_switch_to_view(app->view_dispatcher, SPIMemViewWidget);
}

bool tama_mem_scene_success_on_event(void* context, SceneManagerEvent event) {
    SPIMemApp* app = context;
    bool success = false;
    if(event.type == SceneManagerEventTypeBack) {
        success = true;
        // Go back to start scene when back button is pressed
        scene_manager_search_and_switch_to_another_scene(app->scene_manager, SPIMemSceneStart);
    } else if(event.type == SceneManagerEventTypeCustom) {
        success = true;
        if(event.event == SPIMemCustomEventSuccessOk) {
            // Always go back to start scene when OK is pressed
            scene_manager_search_and_switch_to_another_scene(app->scene_manager, SPIMemSceneStart);
        }
    }
    return success;
}

void tama_mem_scene_success_on_exit(void* context) {
    SPIMemApp* app = context;
    notification_message(app->notifications, &sequence_blink_stop);
    widget_reset(app->widget);
}

