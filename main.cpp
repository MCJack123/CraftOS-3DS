#include <craftos.h>
#include <unistd.h>
extern "C" {
#include <3ds/archive.h>
#include <3ds/console.h>
#include <3ds/gfx.h>
#include <3ds/os.h>
#include <3ds/romfs.h>
#include <3ds/services/apt.h>
#include <3ds/services/hid.h>
#include "bios_lua.h"
}
#include "hbkb.h"

static craftos_machine_t machine;

class HBKB_CC : public HB_Keyboard {
protected:
    virtual void ChangeString(u8 &Key) override {
        if (Key == 42) {
            if (isShift) craftos_event_key_up(machine, 42);
            else craftos_event_key(machine, 42, 0);
            return;
        }
        if (Key & 0x80) craftos_event_key(machine, 42, 0);
        craftos_event_key(machine, Key & 0x7F, 0);
        craftos_event_key_up(machine, Key & 0x7F);
        if (Key & 0x80) craftos_event_key_up(machine, 42);
    }
};

static double timestamp() {
    return (double)(osGetTime() - 2208988800000ULL); /* number = milliseconds between 1/1/00 and 1/1/70 (including leap days) */
}

static unsigned long convertPixelValue(unsigned char index, unsigned char r, unsigned char g, unsigned char b) {
    return (r << 16) | (g << 8) | b;
}

const craftos_func_t funcs = {
    timestamp,
    convertPixelValue,
    NULL
};

int main() {
    bool new3ds;
    APT_CheckNew3DS(&new3ds);
    osSetSpeedupEnable(new3ds);
    gfxInitDefault();
    hidInit();
    //consoleInit(GFX_BOTTOM, NULL);
    chdir("sdmc:/");
    romfsMountSelf("rom");
    craftos_init(&funcs);
    craftos_machine_config_t config = {
        0,
        NULL,
        NULL,
        (const char *)bios_lua,
        400,
        240,
        1,
        "sdmc:"
    };
    machine = craftos_machine_create(&config);
    craftos_machine_mount_real(machine, "rom:", "rom", true);
    HBKB_CC keyboard;
    touchPosition touch = {0, 0};
    keyboard.HBKB_CallKeyboard(touch); // draw initial
    while (aptMainLoop()) {
        craftos_status_t status = craftos_machine_run(machine);
        if (status == CRAFTOS_MACHINE_STATUS_SHUTDOWN) break;
        else if (status == CRAFTOS_MACHINE_STATUS_ERROR) {
            uint16_t width, height;
            craftos_terminal_render_rot90(machine->term, gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &width, &height), width * 3, 24, 1, 1);
            gfxSwapBuffers();
            while (!(hidKeysDown() & KEY_START) && aptMainLoop()) {
                gspWaitForVBlank();
                hidScanInput();
            }
            break;
        }
        uint16_t width, height;
        machine->term->changed = 1;
        craftos_terminal_render_rot90(machine->term, gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &width, &height), width * 3, 24, 1, 1);
        hidScanInput();
        if (hidKeysDown() & KEY_START) break;
        hidTouchRead(&touch);
        keyboard.HBKB_CallKeyboard(touch);
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }
    craftos_machine_destroy(machine);
    hidExit();
    gfxExit();
    return 0;
}
