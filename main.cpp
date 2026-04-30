#include <craftos.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
extern "C" {
#include <3ds/archive.h>
#include <3ds/console.h>
#include <3ds/gfx.h>
#include <3ds/allocator/linear.h>
#include <3ds/os.h>
#include <3ds/romfs.h>
#include <3ds/ndsp/ndsp.h>
#include <3ds/ndsp/channel.h>
#include <3ds/services/apt.h>
#include <3ds/services/dsp.h>
#include <3ds/services/hid.h>
#include <lua.h>
#include <lauxlib.h>
#include "bios_lua.h"
}
#include "hbkb.h"

static craftos_machine_t machine;
static bool enable3D = false;
static gfx3dSide_t screenSide = GFX_LEFT;

class HBKB_CC : public HB_Keyboard {
protected:
    virtual void ChangeString(u8 &Key) override {
        if (Key == 95) return;
        if (Key == 42) {
            if (isCaps) craftos_event_key(machine, 42, 0);
            else craftos_event_key_up(machine, 42);
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

int m3ds_set3DSide(lua_State *L) {
    u16 width, height;
    craftos_terminal_render_rot90(machine->term, gfxGetFramebuffer(GFX_TOP, screenSide, &width, &height), width * 3, 24, 1, 1);
    if (lua_isstring(L, 1)) {
        const char * side = lua_tostring(L, 1);
        if (strcmp(side, "left") == 0) {
            enable3D = true;
            screenSide = GFX_LEFT;
            gfxSet3D(true);
            return 0;
        } else if (strcmp(side, "right") == 0) {
            enable3D = true;
            screenSide = GFX_RIGHT;
            gfxSet3D(true);
            return 0;
        }
    }
    enable3D = false;
    screenSide = GFX_LEFT;
    gfxSet3D(false);
    return 0;
}

int m3ds_get3DSlider(lua_State *L) {
    lua_pushnumber(L, osGet3DSliderState());
    return 1;
}

static const luaL_Reg m3ds_lib[] = {
    {"set3DSide", m3ds_set3DSide},
    {"get3DSlider", m3ds_get3DSlider},
    {NULL, NULL}
};

#define SPEAKER_NBUFS 2
static ndspWaveBuf speaker_bufs[SPEAKER_NBUFS];
static int queueSpeakerEvent = 0;

int speaker_playNote(lua_State *L) {
    return luaL_error(L, "Not implemented");
}

int speaker_playSound(lua_State *L) {
    return luaL_error(L, "Not implemented");
}

int speaker_playAudio(lua_State *L) {
    lua_remove(L, 1);

    ndspWaveBuf * buf = NULL;
    for (int i = 0; i < SPEAKER_NBUFS; i++) {
        if (speaker_bufs[i].status == NDSP_WBUF_FREE) {
            buf = &speaker_bufs[i];
            break;
        } else if (speaker_bufs[i].status == NDSP_WBUF_DONE) {
            linearFree(speaker_bufs[i].data_pcm8);
            buf = &speaker_bufs[i];
            break;
        }
    }
    if (buf == NULL) {
        lua_pushboolean(L, false);
        return 1;
    }

    luaL_checktype(L, 1, LUA_TTABLE);
    int nsamples = lua_rawlen(L, 1);
    if (nsamples == 0 || nsamples > 131072) {
        luaL_error(L, "Too many samples");
    }
    int8_t* samples = (int8_t*)linearAlloc(nsamples);
    for (int i = 0; i < nsamples; i++) {
        lua_rawgeti(L, 1, i+1);
        int n;
        if (!lua_isnumber(L, -1) || (n = lua_tointeger(L, -1)) < -128 || n > 127) {
            linearFree(samples);
            return luaL_error(L, "bad argument #1 (sample #%d out of range)", i + 1);
        }
        samples[i] = n;
        lua_pop(L, 1);
    }
    DSP_FlushDataCache(samples, nsamples);

    buf->data_pcm8 = samples;
    buf->nsamples = nsamples;
    buf->adpcm_data = NULL;
    buf->offset = 0;
    buf->looping = false;
    buf->status = NDSP_WBUF_FREE;
    buf->sequence_id = 0;
    buf->next = NULL;
    ndspChnWaveBufAdd(0, buf);
    queueSpeakerEvent = 0;
    lua_pushboolean(L, true);
    return 1;
}

int speaker_stop(lua_State *L) {
    ndspChnWaveBufClear(0);
    for (int i = 0; i < SPEAKER_NBUFS; i++) {
        linearFree(speaker_bufs[i].data_pcm8);
        speaker_bufs[i].status = NDSP_WBUF_FREE;
    }
    return 0;
}

void speaker_update(void* _) {
    bool hadAudio = false, allFree = true;
    for (int i = 0; i < SPEAKER_NBUFS; i++) {
        if (speaker_bufs[i].status == NDSP_WBUF_DONE) {
            linearFree(speaker_bufs[i].data_pcm8);
            speaker_bufs[i].status = NDSP_WBUF_FREE;
            hadAudio = true;
        }
        if (speaker_bufs[i].status != NDSP_WBUF_FREE) allFree = false;
    }
    if (allFree && hadAudio) {
        if (queueSpeakerEvent == 0) queueSpeakerEvent = 1;
        else if (queueSpeakerEvent == 2) queueSpeakerEvent = 0;
    }
}

static const luaL_Reg speaker_lib[] = {
    {"playNote", speaker_playNote},
    {"playSound", speaker_playSound},
    {"playAudio", speaker_playAudio},
    {"stop", speaker_stop},
    {NULL, NULL}
};

static const char * speaker_types[] = {"speaker", NULL};

int main() {
    bool new3ds;
    APT_CheckNew3DS(&new3ds);
    osSetSpeedupEnable(new3ds);
    gfxInitDefault();
    hidInit();
    //consoleInit(GFX_BOTTOM, NULL);
    ndspInit();
    //ndspSetCallback(speaker_update, NULL);
    ndspSetOutputMode(NDSP_OUTPUT_MONO);
    //ndspChnInitParams(0);
    ndspChnSetInterp(0, NDSP_INTERP_NONE);
    ndspChnSetRate(0, 48000.0f);
    ndspChnSetFormat(0, NDSP_FORMAT_MONO_PCM8);
    float mix[12];
    memset(mix, 0, sizeof(mix));
    mix[0] = 1.0;
    mix[1] = 1.0;
    ndspChnSetMix(0, mix);
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
        "sdmc:",
        "shell.autocomplete=false"
    };
    machine = craftos_machine_create(&config);
    craftos_machine_add_api(machine, "m3ds", m3ds_lib);
    craftos_machine_peripheral_attach(machine, "left", speaker_types, speaker_lib, NULL);
    craftos_machine_mount_real(machine, "rom:", "rom", true);
    HBKB_CC keyboard;
    touchPosition touch = {0, 0};
    keyboard.HBKB_CallKeyboard(touch); // draw initial
    while (aptMainLoop()) {
        speaker_update(NULL);
        if (queueSpeakerEvent == 1) {
            craftos_machine_queue_event(machine, "speaker_audio_empty", "z", "left");
            queueSpeakerEvent = 2;
        }
        craftos_status_t status = craftos_machine_run(machine);
        if (status == CRAFTOS_MACHINE_STATUS_SHUTDOWN) break;
        else if (status == CRAFTOS_MACHINE_STATUS_ERROR) {
            uint16_t width, height;
            craftos_terminal_render_rot90(machine->term, gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &width, &height), width * 3, 24, 1, 1);
            gfxFlushBuffers();
            gfxSwapBuffers();
            while (!(hidKeysDown() & KEY_START) && aptMainLoop()) {
                gspWaitForVBlank();
                hidScanInput();
            }
            break;
        }
        uint16_t width, height;
        machine->term->changed = 1;
        craftos_terminal_render_rot90(machine->term, gfxGetFramebuffer(GFX_TOP, screenSide, &width, &height), width * 3, 24, 1, 1);
        hidScanInput();
        int kDown = hidKeysDown();
        int kUp = hidKeysUp();
        if (kDown & KEY_START) break;
        if (kDown & KEY_SELECT) craftos_machine_queue_event(machine, "terminate", "");
        if (kDown & KEY_UP) craftos_event_key(machine, 200, false);
        else if (kUp & KEY_UP) craftos_event_key_up(machine, 200);
        if (kDown & KEY_DOWN) craftos_event_key(machine, 208, false);
        else if (kUp & KEY_DOWN) craftos_event_key_up(machine, 208);
        if (kDown & KEY_LEFT) craftos_event_key(machine, 203, false);
        else if (kUp & KEY_LEFT) craftos_event_key_up(machine, 203);
        if (kDown & KEY_RIGHT) craftos_event_key(machine, 205, false);
        else if (kUp & KEY_RIGHT) craftos_event_key_up(machine, 205);
        hidTouchRead(&touch);
        keyboard.HBKB_CallKeyboard(touch);
        gfxFlushBuffers();
        gfxScreenSwapBuffers(GFX_TOP, enable3D);
        gfxScreenSwapBuffers(GFX_BOTTOM, false);
        gspWaitForVBlank();
    }
    craftos_machine_destroy(machine);
    speaker_stop(NULL);
    ndspExit();
    hidExit();
    gfxExit();
    return 0;
}
