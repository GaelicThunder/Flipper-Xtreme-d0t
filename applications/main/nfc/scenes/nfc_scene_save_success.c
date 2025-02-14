#include "../nfc_i.h"
#include "../../../settings/desktop_settings/desktop_settings_app.h"

void nfc_scene_save_success_popup_callback(void* context) {
    Nfc* nfc = context;
    view_dispatcher_send_custom_event(nfc->view_dispatcher, NfcCustomEventViewExit);
}

void nfc_scene_save_success_on_enter(void* context) {
    Nfc* nfc = context;
    DesktopSettings* settings = malloc(sizeof(DesktopSettings));
    DESKTOP_SETTINGS_LOAD(settings);

    // Setup view
    Popup* popup = nfc->popup;
    if (settings->sfw_mode) {
        popup_set_icon(popup, 32, 5, &I_DolphinNice_96x59_sfw);
    }
    else {
        popup_set_icon(popup, 32, 5, &I_DolphinNice_96x59);
    }
    popup_set_header(popup, "Saved!", 13, 22, AlignLeft, AlignBottom);
    popup_set_timeout(popup, 1500);
    popup_set_context(popup, nfc);
    popup_set_callback(popup, nfc_scene_save_success_popup_callback);
    popup_enable_timeout(popup);
    view_dispatcher_switch_to_view(nfc->view_dispatcher, NfcViewPopup);
    free(settings);
}

bool nfc_scene_save_success_on_event(void* context, SceneManagerEvent event) {
    Nfc* nfc = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == NfcCustomEventViewExit) {
            if(scene_manager_has_previous_scene(nfc->scene_manager, NfcSceneMfClassicKeys)) {
                consumed = scene_manager_search_and_switch_to_previous_scene(
                    nfc->scene_manager, NfcSceneMfClassicKeys);
            } else if(scene_manager_has_previous_scene(nfc->scene_manager, NfcSceneSavedMenu)) {
                consumed = scene_manager_search_and_switch_to_previous_scene(
                    nfc->scene_manager, NfcSceneSavedMenu);
            } else if(scene_manager_has_previous_scene(nfc->scene_manager, NfcScenePassportAuth)) {
                consumed = scene_manager_search_and_switch_to_previous_scene(
                    nfc->scene_manager, NfcScenePassportAuth);
            } else if(scene_manager_has_previous_scene(nfc->scene_manager, NfcSceneNfcDataInfo)) {
                consumed = scene_manager_search_and_switch_to_previous_scene(
                    nfc->scene_manager, NfcSceneNfcDataInfo);
            } else {
                consumed = scene_manager_search_and_switch_to_another_scene(
                    nfc->scene_manager, NfcSceneFileSelect);
            }
        }
    }
    return consumed;
}

void nfc_scene_save_success_on_exit(void* context) {
    Nfc* nfc = context;

    // Clear view
    popup_reset(nfc->popup);
}
