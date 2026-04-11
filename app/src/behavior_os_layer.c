#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zmk/keymap.h>
#include <zmk/event_manager.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/ble.h>

/* Mac base layer index (must match keymap layer order) */
#define MAC_BASE_LAYER 4

static void update_os_layers(uint8_t profile) {
    switch (profile) {
        case 0: /* Windows: Mac overlay OFF */
            zmk_keymap_layer_deactivate(MAC_BASE_LAYER);
            break;
        case 1: /* Mac: Mac overlay ON */
            zmk_keymap_layer_activate(MAC_BASE_LAYER);
            break;
        default: /* Other profiles (2-4): treat as Windows */
            zmk_keymap_layer_deactivate(MAC_BASE_LAYER);
            break;
    }
}

static int os_layer_listener_cb(const zmk_event_t *eh) {
    const struct zmk_ble_active_profile_changed *ev =
        as_zmk_ble_active_profile_changed(eh);
    if (ev) {
        update_os_layers(ev->index);
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(os_layer_listener, os_layer_listener_cb);
ZMK_SUBSCRIPTION(os_layer_listener, zmk_ble_active_profile_changed);

static int behavior_os_layer_init(void) {
    update_os_layers(zmk_ble_active_profile_index());
    return 0;
}

SYS_INIT(behavior_os_layer_init, APPLICATION, 95);
