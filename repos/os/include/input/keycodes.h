/*
 * \brief  Key code definitions
 * \author Norman Feske
 * \date   2006-08-16
 *
 * The definitions are based on the code
 * of the Linux /dev/input/event interface.
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__INPUT__KEYCODES_H_
#define _INCLUDE__INPUT__KEYCODES_H_

/*
 * C++ provides no reflection of the names of enum value. So we use the
 * preprocessor to generate the bodies of both the enum type 'Keycode' and
 * the reflection function 'key_name' from one definition.
 */
#define GENODE_INPUT_KEYCODES \
	GENODE_TAG_VALUE(KEY_RESERVED,    0) \
	GENODE_TAG_VALUE(KEY_ESC,         1) \
	GENODE_TAG_VALUE(KEY_1,           2) \
	GENODE_TAG_VALUE(KEY_2,           3) \
	GENODE_TAG_VALUE(KEY_3,           4) \
	GENODE_TAG_VALUE(KEY_4,           5) \
	GENODE_TAG_VALUE(KEY_5,           6) \
	GENODE_TAG_VALUE(KEY_6,           7) \
	GENODE_TAG_VALUE(KEY_7,           8) \
	GENODE_TAG_VALUE(KEY_8,           9) \
	GENODE_TAG_VALUE(KEY_9,          10) \
	GENODE_TAG_VALUE(KEY_0,          11) \
	GENODE_TAG_VALUE(KEY_MINUS,      12) \
	GENODE_TAG_VALUE(KEY_EQUAL,      13) \
	GENODE_TAG_VALUE(KEY_BACKSPACE,  14) \
	GENODE_TAG_VALUE(KEY_TAB,        15) \
	GENODE_TAG_VALUE(KEY_Q,          16) \
	GENODE_TAG_VALUE(KEY_W,          17) \
	GENODE_TAG_VALUE(KEY_E,          18) \
	GENODE_TAG_VALUE(KEY_R,          19) \
	GENODE_TAG_VALUE(KEY_T,          20) \
	GENODE_TAG_VALUE(KEY_Y,          21) \
	GENODE_TAG_VALUE(KEY_U,          22) \
	GENODE_TAG_VALUE(KEY_I,          23) \
	GENODE_TAG_VALUE(KEY_O,          24) \
	GENODE_TAG_VALUE(KEY_P,          25) \
	GENODE_TAG_VALUE(KEY_LEFTBRACE,  26) \
	GENODE_TAG_VALUE(KEY_RIGHTBRACE, 27) \
	GENODE_TAG_VALUE(KEY_ENTER,      28) \
	GENODE_TAG_VALUE(KEY_LEFTCTRL,   29) \
	GENODE_TAG_VALUE(KEY_A,          30) \
	GENODE_TAG_VALUE(KEY_S,          31) \
	GENODE_TAG_VALUE(KEY_D,          32) \
	GENODE_TAG_VALUE(KEY_F,          33) \
	GENODE_TAG_VALUE(KEY_G,          34) \
	GENODE_TAG_VALUE(KEY_H,          35) \
	GENODE_TAG_VALUE(KEY_J,          36) \
	GENODE_TAG_VALUE(KEY_K,          37) \
	GENODE_TAG_VALUE(KEY_L,          38) \
	GENODE_TAG_VALUE(KEY_SEMICOLON,  39) \
	GENODE_TAG_VALUE(KEY_APOSTROPHE, 40) \
	GENODE_TAG_VALUE(KEY_GRAVE,      41) \
	GENODE_TAG_VALUE(KEY_LEFTSHIFT,  42) \
	GENODE_TAG_VALUE(KEY_BACKSLASH,  43) \
	GENODE_TAG_VALUE(KEY_Z,          44) \
	GENODE_TAG_VALUE(KEY_X,          45) \
	GENODE_TAG_VALUE(KEY_C,          46) \
	GENODE_TAG_VALUE(KEY_V,          47) \
	GENODE_TAG_VALUE(KEY_B,          48) \
	GENODE_TAG_VALUE(KEY_N,          49) \
	GENODE_TAG_VALUE(KEY_M,          50) \
	GENODE_TAG_VALUE(KEY_COMMA,      51) \
	GENODE_TAG_VALUE(KEY_DOT,        52) \
	GENODE_TAG_VALUE(KEY_SLASH,      53) \
	GENODE_TAG_VALUE(KEY_RIGHTSHIFT, 54) \
	GENODE_TAG_VALUE(KEY_KPASTERISK, 55) \
	GENODE_TAG_VALUE(KEY_LEFTALT,    56) \
	GENODE_TAG_VALUE(KEY_SPACE,      57) \
	GENODE_TAG_VALUE(KEY_CAPSLOCK,   58) \
	GENODE_TAG_VALUE(KEY_F1,         59) \
	GENODE_TAG_VALUE(KEY_F2,         60) \
	GENODE_TAG_VALUE(KEY_F3,         61) \
	GENODE_TAG_VALUE(KEY_F4,         62) \
	GENODE_TAG_VALUE(KEY_F5,         63) \
	GENODE_TAG_VALUE(KEY_F6,         64) \
	GENODE_TAG_VALUE(KEY_F7,         65) \
	GENODE_TAG_VALUE(KEY_F8,         66) \
	GENODE_TAG_VALUE(KEY_F9,         67) \
	GENODE_TAG_VALUE(KEY_F10,        68) \
	GENODE_TAG_VALUE(KEY_NUMLOCK,    69) \
	GENODE_TAG_VALUE(KEY_SCROLLLOCK, 70) \
	GENODE_TAG_VALUE(KEY_KP7,        71) \
	GENODE_TAG_VALUE(KEY_KP8,        72) \
	GENODE_TAG_VALUE(KEY_KP9,        73) \
	GENODE_TAG_VALUE(KEY_KPMINUS,    74) \
	GENODE_TAG_VALUE(KEY_KP4,        75) \
	GENODE_TAG_VALUE(KEY_KP5,        76) \
	GENODE_TAG_VALUE(KEY_KP6,        77) \
	GENODE_TAG_VALUE(KEY_KPPLUS,     78) \
	GENODE_TAG_VALUE(KEY_KP1,        79) \
	GENODE_TAG_VALUE(KEY_KP2,        80) \
	GENODE_TAG_VALUE(KEY_KP3,        81) \
	GENODE_TAG_VALUE(KEY_KP0,        82) \
	GENODE_TAG_VALUE(KEY_KPDOT,      83) \
	\
	GENODE_TAG_VALUE(KEY_ZENKAKUHANKAKU,   85) \
	GENODE_TAG_VALUE(KEY_102ND,            86) \
	GENODE_TAG_VALUE(KEY_F11,              87) \
	GENODE_TAG_VALUE(KEY_F12,              88) \
	GENODE_TAG_VALUE(KEY_RO,               89) \
	GENODE_TAG_VALUE(KEY_KATAKANA,         90) \
	GENODE_TAG_VALUE(KEY_HIRAGANA,         91) \
	GENODE_TAG_VALUE(KEY_HENKAN,           92) \
	GENODE_TAG_VALUE(KEY_KATAKANAHIRAGANA, 93) \
	GENODE_TAG_VALUE(KEY_MUHENKAN,         94) \
	GENODE_TAG_VALUE(KEY_KPJPCOMMA,        95) \
	GENODE_TAG_VALUE(KEY_KPENTER,          96) \
	GENODE_TAG_VALUE(KEY_RIGHTCTRL,        97) \
	GENODE_TAG_VALUE(KEY_KPSLASH,          98) \
	GENODE_TAG_VALUE(KEY_SYSRQ,            99) \
	GENODE_TAG_VALUE(KEY_RIGHTALT,        100) \
	GENODE_TAG_VALUE(KEY_LINEFEED,        101) \
	GENODE_TAG_VALUE(KEY_HOME,            102) \
	GENODE_TAG_VALUE(KEY_UP,              103) \
	GENODE_TAG_VALUE(KEY_PAGEUP,          104) \
	GENODE_TAG_VALUE(KEY_LEFT,            105) \
	GENODE_TAG_VALUE(KEY_RIGHT,           106) \
	GENODE_TAG_VALUE(KEY_END,             107) \
	GENODE_TAG_VALUE(KEY_DOWN,            108) \
	GENODE_TAG_VALUE(KEY_PAGEDOWN,        109) \
	GENODE_TAG_VALUE(KEY_INSERT,          110) \
	GENODE_TAG_VALUE(KEY_DELETE,          111) \
	GENODE_TAG_VALUE(KEY_MACRO,           112) \
	GENODE_TAG_VALUE(KEY_MUTE,            113) \
	GENODE_TAG_VALUE(KEY_VOLUMEDOWN,      114) \
	GENODE_TAG_VALUE(KEY_VOLUMEUP,        115) \
	GENODE_TAG_VALUE(KEY_POWER,           116) \
	GENODE_TAG_VALUE(KEY_KPEQUAL,         117) \
	GENODE_TAG_VALUE(KEY_KPPLUSMINUS,     118) \
	GENODE_TAG_VALUE(KEY_PAUSE,           119) \
	\
	GENODE_TAG_VALUE(KEY_KPCOMMA,   121) \
	GENODE_TAG_VALUE(KEY_HANGUEL,   122) \
	GENODE_TAG_VALUE(KEY_HANJA,     123) \
	GENODE_TAG_VALUE(KEY_YEN,       124) \
	GENODE_TAG_VALUE(KEY_LEFTMETA,  125) \
	GENODE_TAG_VALUE(KEY_RIGHTMETA, 126) \
	GENODE_TAG_VALUE(KEY_COMPOSE,   127) \
	\
	GENODE_TAG_VALUE(KEY_STOP,         128) \
	GENODE_TAG_VALUE(KEY_AGAIN,        129) \
	GENODE_TAG_VALUE(KEY_PROPS,        130) \
	GENODE_TAG_VALUE(KEY_UNDO,         131) \
	GENODE_TAG_VALUE(KEY_FRONT,        132) \
	GENODE_TAG_VALUE(KEY_COPY,         133) \
	GENODE_TAG_VALUE(KEY_OPEN,         134) \
	GENODE_TAG_VALUE(KEY_PASTE,        135) \
	GENODE_TAG_VALUE(KEY_FIND,         136) \
	GENODE_TAG_VALUE(KEY_CUT,          137) \
	GENODE_TAG_VALUE(KEY_HELP,         138) \
	GENODE_TAG_VALUE(KEY_MENU,         139) \
	GENODE_TAG_VALUE(KEY_CALC,         140) \
	GENODE_TAG_VALUE(KEY_SETUP,        141) \
	GENODE_TAG_VALUE(KEY_SLEEP,        142) \
	GENODE_TAG_VALUE(KEY_WAKEUP,       143) \
	GENODE_TAG_VALUE(KEY_FILE,         144) \
	GENODE_TAG_VALUE(KEY_SENDFILE,     145) \
	GENODE_TAG_VALUE(KEY_DELETEFILE,   146) \
	GENODE_TAG_VALUE(KEY_XFER,         147) \
	GENODE_TAG_VALUE(KEY_PROG1,        148) \
	GENODE_TAG_VALUE(KEY_PROG2,        149) \
	GENODE_TAG_VALUE(KEY_WWW,          150) \
	GENODE_TAG_VALUE(KEY_MSDOS,        151) \
	GENODE_TAG_VALUE(KEY_COFFEE,       152) \
	GENODE_TAG_VALUE(KEY_DIRECTION,    153) \
	GENODE_TAG_VALUE(KEY_CYCLEWINDOWS, 154) \
	GENODE_TAG_VALUE(KEY_MAIL,         155) \
	GENODE_TAG_VALUE(KEY_BOOKMARKS,    156) \
	GENODE_TAG_VALUE(KEY_COMPUTER,     157) \
	GENODE_TAG_VALUE(KEY_BACK,         158) \
	GENODE_TAG_VALUE(KEY_FORWARD,      159) \
	GENODE_TAG_VALUE(KEY_CLOSECD,      160) \
	GENODE_TAG_VALUE(KEY_EJECTCD,      161) \
	GENODE_TAG_VALUE(KEY_EJECTCLOSECD, 162) \
	GENODE_TAG_VALUE(KEY_NEXTSONG,     163) \
	GENODE_TAG_VALUE(KEY_PLAYPAUSE,    164) \
	GENODE_TAG_VALUE(KEY_PREVIOUSSONG, 165) \
	GENODE_TAG_VALUE(KEY_STOPCD,       166) \
	GENODE_TAG_VALUE(KEY_RECORD,       167) \
	GENODE_TAG_VALUE(KEY_REWIND,       168) \
	GENODE_TAG_VALUE(KEY_PHONE,        169) \
	GENODE_TAG_VALUE(KEY_ISO,          170) \
	GENODE_TAG_VALUE(KEY_CONFIG,       171) \
	GENODE_TAG_VALUE(KEY_HOMEPAGE,     172) \
	GENODE_TAG_VALUE(KEY_REFRESH,      173) \
	GENODE_TAG_VALUE(KEY_EXIT,         174) \
	GENODE_TAG_VALUE(KEY_MOVE,         175) \
	GENODE_TAG_VALUE(KEY_EDIT,         176) \
	GENODE_TAG_VALUE(KEY_SCROLLUP,     177) \
	GENODE_TAG_VALUE(KEY_SCROLLDOWN,   178) \
	GENODE_TAG_VALUE(KEY_KPLEFTPAREN,  179) \
	GENODE_TAG_VALUE(KEY_KPRIGHTPAREN, 180) \
	GENODE_TAG_VALUE(KEY_NEW,          181) \
	GENODE_TAG_VALUE(KEY_REDO,         182) \
	\
	GENODE_TAG_VALUE(KEY_F13, 183) \
	GENODE_TAG_VALUE(KEY_F14, 184) \
	GENODE_TAG_VALUE(KEY_F15, 185) \
	GENODE_TAG_VALUE(KEY_F16, 186) \
	GENODE_TAG_VALUE(KEY_F17, 187) \
	GENODE_TAG_VALUE(KEY_F18, 188) \
	GENODE_TAG_VALUE(KEY_F19, 189) \
	GENODE_TAG_VALUE(KEY_F20, 190) \
	GENODE_TAG_VALUE(KEY_F21, 191) \
	GENODE_TAG_VALUE(KEY_F22, 192) \
	GENODE_TAG_VALUE(KEY_F23, 193) \
	GENODE_TAG_VALUE(KEY_F24, 194) \
	\
	GENODE_TAG_VALUE(KEY_PLAYCD,          200) \
	GENODE_TAG_VALUE(KEY_PAUSECD,         201) \
	GENODE_TAG_VALUE(KEY_PROG3,           202) \
	GENODE_TAG_VALUE(KEY_PROG4,           203) \
	GENODE_TAG_VALUE(KEY_DASHBOARD,       204) \
	GENODE_TAG_VALUE(KEY_SUSPEND,         205) \
	GENODE_TAG_VALUE(KEY_CLOSE,           206) \
	GENODE_TAG_VALUE(KEY_PLAY,            207) \
	GENODE_TAG_VALUE(KEY_FASTFORWARD,     208) \
	GENODE_TAG_VALUE(KEY_BASSBOOST,       209) \
	GENODE_TAG_VALUE(KEY_PRINT,           210) \
	GENODE_TAG_VALUE(KEY_HP,              211) \
	GENODE_TAG_VALUE(KEY_CAMERA,          212) \
	GENODE_TAG_VALUE(KEY_SOUND,           213) \
	GENODE_TAG_VALUE(KEY_QUESTION,        214) \
	GENODE_TAG_VALUE(KEY_EMAIL,           215) \
	GENODE_TAG_VALUE(KEY_CHAT,            216) \
	GENODE_TAG_VALUE(KEY_SEARCH,          217) \
	GENODE_TAG_VALUE(KEY_CONNECT,         218) \
	GENODE_TAG_VALUE(KEY_FINANCE,         219) \
	GENODE_TAG_VALUE(KEY_SPORT,           220) \
	GENODE_TAG_VALUE(KEY_SHOP,            221) \
	GENODE_TAG_VALUE(KEY_ALTERASE,        222) \
	GENODE_TAG_VALUE(KEY_CANCEL,          223) \
	GENODE_TAG_VALUE(KEY_BRIGHTNESSDOWN,  224) \
	GENODE_TAG_VALUE(KEY_BRIGHTNESSUP,    225) \
	GENODE_TAG_VALUE(KEY_MEDIA,           226) \
	GENODE_TAG_VALUE(KEY_SWITCHVIDEOMODE, 227) \
	GENODE_TAG_VALUE(KEY_KBDILLUMTOGGLE,  228) \
	GENODE_TAG_VALUE(KEY_KBDILLUMDOWN,    229) \
	GENODE_TAG_VALUE(KEY_KBDILLUMUP,      230) \
	GENODE_TAG_VALUE(KEY_SEND,            231) \
	GENODE_TAG_VALUE(KEY_REPLY,           232) \
	GENODE_TAG_VALUE(KEY_FORWARDMAIL,     233) \
	GENODE_TAG_VALUE(KEY_SAVE,            234) \
	GENODE_TAG_VALUE(KEY_DOCUMENTS,       235) \
	GENODE_TAG_VALUE(KEY_BATTERY,         236) \
	GENODE_TAG_VALUE(KEY_BLUETOOTH,       237) \
	GENODE_TAG_VALUE(KEY_WLAN,            238) \
	\
	GENODE_TAG_VALUE(BTN_0,    0x100) \
	GENODE_TAG_VALUE(BTN_1,    0x101) \
	GENODE_TAG_VALUE(BTN_2,    0x102) \
	GENODE_TAG_VALUE(BTN_3,    0x103) \
	GENODE_TAG_VALUE(BTN_4,    0x104) \
	GENODE_TAG_VALUE(BTN_5,    0x105) \
	GENODE_TAG_VALUE(BTN_6,    0x106) \
	GENODE_TAG_VALUE(BTN_7,    0x107) \
	GENODE_TAG_VALUE(BTN_8,    0x108) \
	GENODE_TAG_VALUE(BTN_9,    0x109) \
	\
	GENODE_TAG_VALUE(BTN_LEFT,    0x110) \
	GENODE_TAG_VALUE(BTN_RIGHT,   0x111) \
	GENODE_TAG_VALUE(BTN_MIDDLE,  0x112) \
	GENODE_TAG_VALUE(BTN_SIDE,    0x113) \
	GENODE_TAG_VALUE(BTN_EXTRA,   0x114) \
	GENODE_TAG_VALUE(BTN_FORWARD, 0x115) \
	GENODE_TAG_VALUE(BTN_BACK,    0x116) \
	GENODE_TAG_VALUE(BTN_TASK,    0x117) \
	\
	GENODE_TAG_VALUE(BTN_TRIGGER,  0x120) \
	GENODE_TAG_VALUE(BTN_THUMB,    0x121) \
	GENODE_TAG_VALUE(BTN_THUMB2,   0x122) \
	GENODE_TAG_VALUE(BTN_TOP,      0x123) \
	GENODE_TAG_VALUE(BTN_TOP2,     0x124) \
	GENODE_TAG_VALUE(BTN_PINKIE,   0x125) \
	GENODE_TAG_VALUE(BTN_BASE,     0x126) \
	GENODE_TAG_VALUE(BTN_BASE2,    0x127) \
	GENODE_TAG_VALUE(BTN_BASE3,    0x128) \
	GENODE_TAG_VALUE(BTN_BASE4,    0x129) \
	GENODE_TAG_VALUE(BTN_BASE5,    0x12a) \
	GENODE_TAG_VALUE(BTN_BASE6,    0x12b) \
	GENODE_TAG_VALUE(BTN_DEAD,     0x12f) \
	\
	GENODE_TAG_VALUE(BTN_A,       0x130) \
	GENODE_TAG_VALUE(BTN_B,       0x131) \
	GENODE_TAG_VALUE(BTN_C,       0x132) \
	GENODE_TAG_VALUE(BTN_X,       0x133) \
	GENODE_TAG_VALUE(BTN_Y,       0x134) \
	GENODE_TAG_VALUE(BTN_Z,       0x135) \
	GENODE_TAG_VALUE(BTN_TL,      0x136) \
	GENODE_TAG_VALUE(BTN_TR,      0x137) \
	GENODE_TAG_VALUE(BTN_TL2,     0x138) \
	GENODE_TAG_VALUE(BTN_TR2,     0x139) \
	GENODE_TAG_VALUE(BTN_SELECT,  0x13a) \
	GENODE_TAG_VALUE(BTN_START,   0x13b) \
	GENODE_TAG_VALUE(BTN_MODE,    0x13c) \
	GENODE_TAG_VALUE(BTN_THUMBL,  0x13d) \
	GENODE_TAG_VALUE(BTN_THUMBR,  0x13e) \
	\
	GENODE_TAG_VALUE(BTN_TOOL_PEN,       0x140) \
	GENODE_TAG_VALUE(BTN_TOOL_RUBBER,    0x141) \
	GENODE_TAG_VALUE(BTN_TOOL_BRUSH,     0x142) \
	GENODE_TAG_VALUE(BTN_TOOL_PENCIL,    0x143) \
	GENODE_TAG_VALUE(BTN_TOOL_AIRBRUSH,  0x144) \
	GENODE_TAG_VALUE(BTN_TOOL_FINGER,    0x145) \
	GENODE_TAG_VALUE(BTN_TOOL_MOUSE,     0x146) \
	GENODE_TAG_VALUE(BTN_TOOL_LENS,      0x147) \
	GENODE_TAG_VALUE(BTN_TOUCH,          0x14a) \
	GENODE_TAG_VALUE(BTN_STYLUS,         0x14b) \
	GENODE_TAG_VALUE(BTN_STYLUS2,        0x14c) \
	GENODE_TAG_VALUE(BTN_TOOL_DOUBLETAP, 0x14d) \
	GENODE_TAG_VALUE(BTN_TOOL_TRIPLETAP, 0x14e) \
	\
	GENODE_TAG_VALUE(BTN_GEAR_DOWN,      0x150) \
	GENODE_TAG_VALUE(BTN_GEAR_UP,        0x151) \
	\
	GENODE_TAG_VALUE(KEY_OK,             0x160) \
	GENODE_TAG_VALUE(KEY_SELECT,         0x161) \
	GENODE_TAG_VALUE(KEY_GOTO,           0x162) \
	GENODE_TAG_VALUE(KEY_CLEAR,          0x163) \
	GENODE_TAG_VALUE(KEY_POWER2,         0x164) \
	GENODE_TAG_VALUE(KEY_OPTION,         0x165) \
	GENODE_TAG_VALUE(KEY_INFO,           0x166) \
	GENODE_TAG_VALUE(KEY_TIME,           0x167) \
	GENODE_TAG_VALUE(KEY_VENDOR,         0x168) \
	GENODE_TAG_VALUE(KEY_ARCHIVE,        0x169) \
	GENODE_TAG_VALUE(KEY_PROGRAM,        0x16a) \
	GENODE_TAG_VALUE(KEY_CHANNEL,        0x16b) \
	GENODE_TAG_VALUE(KEY_FAVORITES,      0x16c) \
	GENODE_TAG_VALUE(KEY_EPG,            0x16d) \
	GENODE_TAG_VALUE(KEY_PVR,            0x16e) \
	GENODE_TAG_VALUE(KEY_MHP,            0x16f) \
	GENODE_TAG_VALUE(KEY_LANGUAGE,       0x170) \
	GENODE_TAG_VALUE(KEY_TITLE,          0x171) \
	GENODE_TAG_VALUE(KEY_SUBTITLE,       0x172) \
	GENODE_TAG_VALUE(KEY_ANGLE,          0x173) \
	GENODE_TAG_VALUE(KEY_ZOOM,           0x174) \
	GENODE_TAG_VALUE(KEY_MODE,           0x175) \
	GENODE_TAG_VALUE(KEY_KEYBOARD,       0x176) \
	GENODE_TAG_VALUE(KEY_SCREEN,         0x177) \
	GENODE_TAG_VALUE(KEY_PC,             0x178) \
	GENODE_TAG_VALUE(KEY_TV,             0x179) \
	GENODE_TAG_VALUE(KEY_TV2,            0x17a) \
	GENODE_TAG_VALUE(KEY_VCR,            0x17b) \
	GENODE_TAG_VALUE(KEY_VCR2,           0x17c) \
	GENODE_TAG_VALUE(KEY_SAT,            0x17d) \
	GENODE_TAG_VALUE(KEY_SAT2,           0x17e) \
	GENODE_TAG_VALUE(KEY_CD,             0x17f) \
	GENODE_TAG_VALUE(KEY_TAPE,           0x180) \
	GENODE_TAG_VALUE(KEY_RADIO,          0x181) \
	GENODE_TAG_VALUE(KEY_TUNER,          0x182) \
	GENODE_TAG_VALUE(KEY_PLAYER,         0x183) \
	GENODE_TAG_VALUE(KEY_TEXT,           0x184) \
	GENODE_TAG_VALUE(KEY_DVD,            0x185) \
	GENODE_TAG_VALUE(KEY_AUX,            0x186) \
	GENODE_TAG_VALUE(KEY_MP3,            0x187) \
	GENODE_TAG_VALUE(KEY_AUDIO,          0x188) \
	GENODE_TAG_VALUE(KEY_VIDEO,          0x189) \
	GENODE_TAG_VALUE(KEY_DIRECTORY,      0x18a) \
	GENODE_TAG_VALUE(KEY_LIST,           0x18b) \
	GENODE_TAG_VALUE(KEY_MEMO,           0x18c) \
	GENODE_TAG_VALUE(KEY_CALENDAR,       0x18d) \
	GENODE_TAG_VALUE(KEY_RED,            0x18e) \
	GENODE_TAG_VALUE(KEY_GREEN,          0x18f) \
	GENODE_TAG_VALUE(KEY_YELLOW,         0x190) \
	GENODE_TAG_VALUE(KEY_BLUE,           0x191) \
	GENODE_TAG_VALUE(KEY_CHANNELUP,      0x192) \
	GENODE_TAG_VALUE(KEY_CHANNELDOWN,    0x193) \
	GENODE_TAG_VALUE(KEY_FIRST,          0x194) \
	GENODE_TAG_VALUE(KEY_LAST,           0x195) \
	GENODE_TAG_VALUE(KEY_AB,             0x196) \
	GENODE_TAG_VALUE(KEY_NEXT,           0x197) \
	GENODE_TAG_VALUE(KEY_RESTART,        0x198) \
	GENODE_TAG_VALUE(KEY_SLOW,           0x199) \
	GENODE_TAG_VALUE(KEY_SHUFFLE,        0x19a) \
	GENODE_TAG_VALUE(KEY_BREAK,          0x19b) \
	GENODE_TAG_VALUE(KEY_PREVIOUS,       0x19c) \
	GENODE_TAG_VALUE(KEY_DIGITS,         0x19d) \
	GENODE_TAG_VALUE(KEY_TEEN,           0x19e) \
	GENODE_TAG_VALUE(KEY_TWEN,           0x19f) \
	GENODE_TAG_VALUE(KEY_VIDEOPHONE,     0x1a0) \
	GENODE_TAG_VALUE(KEY_GAMES,          0x1a1) \
	GENODE_TAG_VALUE(KEY_ZOOMIN,         0x1a2) \
	GENODE_TAG_VALUE(KEY_ZOOMOUT,        0x1a3) \
	GENODE_TAG_VALUE(KEY_ZOOMRESET,      0x1a4) \
	GENODE_TAG_VALUE(KEY_WORDPROCESSOR,  0x1a5) \
	GENODE_TAG_VALUE(KEY_EDITOR,         0x1a6) \
	GENODE_TAG_VALUE(KEY_SPREADSHEET,    0x1a7) \
	GENODE_TAG_VALUE(KEY_GRAPHICSEDITOR, 0x1a8) \
	GENODE_TAG_VALUE(KEY_PRESENTATION,   0x1a9) \
	GENODE_TAG_VALUE(KEY_DATABASE,       0x1aa) \
	GENODE_TAG_VALUE(KEY_NEWS,           0x1ab) \
	GENODE_TAG_VALUE(KEY_VOICEMAIL,      0x1ac) \
	GENODE_TAG_VALUE(KEY_ADDRESSBOOK,    0x1ad) \
	GENODE_TAG_VALUE(KEY_MESSENGER,      0x1ae) \
	\
	GENODE_TAG_VALUE(KEY_DEL_EOL,        0x1c0) \
	GENODE_TAG_VALUE(KEY_DEL_EOS,        0x1c1) \
	GENODE_TAG_VALUE(KEY_INS_LINE,       0x1c2) \
	GENODE_TAG_VALUE(KEY_DEL_LINE,       0x1c3) \
	\
	GENODE_TAG_VALUE(KEY_FN,             0x1d0) \
	GENODE_TAG_VALUE(KEY_FN_ESC,         0x1d1) \
	GENODE_TAG_VALUE(KEY_FN_F1,          0x1d2) \
	GENODE_TAG_VALUE(KEY_FN_F2,          0x1d3) \
	GENODE_TAG_VALUE(KEY_FN_F3,          0x1d4) \
	GENODE_TAG_VALUE(KEY_FN_F4,          0x1d5) \
	GENODE_TAG_VALUE(KEY_FN_F5,          0x1d6) \
	GENODE_TAG_VALUE(KEY_FN_F6,          0x1d7) \
	GENODE_TAG_VALUE(KEY_FN_F7,          0x1d8) \
	GENODE_TAG_VALUE(KEY_FN_F8,          0x1d9) \
	GENODE_TAG_VALUE(KEY_FN_F9,          0x1da) \
	GENODE_TAG_VALUE(KEY_FN_F10,         0x1db) \
	GENODE_TAG_VALUE(KEY_FN_F11,         0x1dc) \
	GENODE_TAG_VALUE(KEY_FN_F12,         0x1dd) \
	GENODE_TAG_VALUE(KEY_FN_1,           0x1de) \
	GENODE_TAG_VALUE(KEY_FN_2,           0x1df) \
	GENODE_TAG_VALUE(KEY_FN_D,           0x1e0) \
	GENODE_TAG_VALUE(KEY_FN_E,           0x1e1) \
	GENODE_TAG_VALUE(KEY_FN_F,           0x1e2) \
	GENODE_TAG_VALUE(KEY_FN_S,           0x1e3) \
	GENODE_TAG_VALUE(KEY_FN_B,           0x1e4) \
	\
	GENODE_TAG_VALUE(KEY_BRL_DOT1,       0x1f1) \
	GENODE_TAG_VALUE(KEY_BRL_DOT2,       0x1f2) \
	GENODE_TAG_VALUE(KEY_BRL_DOT3,       0x1f3) \
	GENODE_TAG_VALUE(KEY_BRL_DOT4,       0x1f4) \
	GENODE_TAG_VALUE(KEY_BRL_DOT5,       0x1f5) \
	GENODE_TAG_VALUE(KEY_BRL_DOT6,       0x1f6) \
	GENODE_TAG_VALUE(KEY_BRL_DOT7,       0x1f7) \
	GENODE_TAG_VALUE(KEY_BRL_DOT8,       0x1f8) \
	\
	GENODE_TAG_VALUE(KEY_UNKNOWN,        0x1fe) \
	GENODE_TAG_VALUE(KEY_MAX,            0x1ff)


namespace Input {

	/**
	 * Definition of enum values for key codes
	 */
	enum Keycode {
#define GENODE_TAG_VALUE(tag, value) tag = value,
		GENODE_INPUT_KEYCODES
#undef GENODE_TAG_VALUE

		/*
		 * Aliases
		 */
		BTN_WHEEL    = BTN_GEAR_DOWN,
		BTN_DIGI     = BTN_TOOL_PEN,
		BTN_GAMEPAD  = BTN_A,
		BTN_JOYSTICK = BTN_TRIGGER,
		BTN_MOUSE    = BTN_LEFT,
		BTN_MISC     = BTN_0,
	};


	/**
	 * Request name of specified key code
	 */
	static inline char const *key_name(Keycode key) {
		switch (key) {
#define GENODE_TAG_VALUE(tag, value) case tag: return #tag;
			GENODE_INPUT_KEYCODES
#undef GENODE_TAG_VALUE
		}
		return "KEY_UNKNOWN";
	}
}


/*
 * Undefine the macro because it is not part of the Genode API
 */
#undef GENODE_INPUT_KEYCODES


#endif /* _INCLUDE__INPUT__KEYCODES_H_ */
