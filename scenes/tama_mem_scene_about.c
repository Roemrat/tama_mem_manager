#include "../tama_mem_app_i.h"
#include "../lib/spi/tama_mem_chip.h"

#define tama_mem_VERSION_APP FAP_VERSION
#define tama_mem_DEVELOPER "DrunkBatya"
#define tama_mem_GITHUB "https://github.com/flipperdevices/flipperzero-good-faps"
#define tama_mem_NAME "\e#\e!       tama mem Manager        \e!\n"
#define tama_mem_BLANK_INV "\e#\e!                                                      \e!\n"

void tama_mem_scene_about_on_enter(void* context) {
    SPIMemApp* app = context;
    FuriString* tmp_string = furi_string_alloc();

    widget_add_text_box_element(
        app->widget, 0, 0, 128, 14, AlignCenter, AlignBottom, tama_mem_BLANK_INV, false);
    widget_add_text_box_element(
        app->widget, 0, 2, 128, 14, AlignCenter, AlignBottom, tama_mem_NAME, false);
    furi_string_printf(tmp_string, "\e#%s\n", "Information");
    furi_string_cat_printf(tmp_string, "Version: %s\n", tama_mem_VERSION_APP);
    furi_string_cat_printf(tmp_string, "Developed by: %s\n", tama_mem_DEVELOPER);
    furi_string_cat_printf(tmp_string, "Github: %s\n\n", tama_mem_GITHUB);
    furi_string_cat_printf(tmp_string, "\e#%s\n", "Description");
    furi_string_cat_printf(
        tmp_string,
        "tama memory dumper\n"
        "Originally written by Hedger, ghettorce and x893 at\n"
        "Flipper Hackathon 2021\n\n");
    widget_add_text_scroll_element(app->widget, 0, 16, 128, 50, furi_string_get_cstr(tmp_string));

    furi_string_free(tmp_string);
    view_dispatcher_switch_to_view(app->view_dispatcher, SPIMemViewWidget);
}

bool tama_mem_scene_about_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}
void tama_mem_scene_about_on_exit(void* context) {
    SPIMemApp* app = context;
    widget_reset(app->widget);
}


