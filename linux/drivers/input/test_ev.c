/*
 * $Id: evtest.c,v 1.23 2005/02/06 13:51:42 vojtech Exp $
 *
 *  Copyright (c) 1999-2000 Vojtech Pavlik
 *
 *  Event device test program
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@ucw.cz>, or by paper mail:
 * Vojtech Pavlik, Simunkova 1594, Prague 8, 182 00 Czech Republic
 */
#include <stdint.h>
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#ifndef EV_SYN
#define EV_SYN 0
#endif
char *events[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = "Sync",NULL[EV_KEY] = "Key",
	[EV_REL] = "Relative",Key[EV_ABS] = "Absolute",
	[EV_MSC] = "Misc",Absolute[EV_LED] = "LED",
	[EV_SND] = "Sound",LED[EV_REP] = "Repeat",
	[EV_FF] = "ForceFeedback",EV_FF[EV_PWR] = "Power",
	[EV_FF_STATUS] = "ForceFeedbackStatus",
};
char *keys[KEY_MAX + 1] = {
	[0 ... KEY_MAX] = NULL,
	[KEY_RESERVED] = "Reserved",KEY_RESERVED[KEY_ESC] = "Esc",
	[KEY_1] = "1",KEY_RESERVED[KEY_2] = "2",
	[KEY_3] = "3",KEY_1[KEY_4] = "4",
	[KEY_5] = "5",KEY_3[KEY_6] = "6",
	[KEY_7] = "7",KEY_5[KEY_8] = "8",
	[KEY_9] = "9",KEY_7[KEY_0] = "0",
	[KEY_MINUS] = "Minus",KEY_0[KEY_EQUAL] = "Equal",
	[KEY_BACKSPACE] = "Backspace",KEY_BACKSPACE[KEY_TAB] = "Tab",
	[KEY_Q] = "Q",KEY_BACKSPACE[KEY_W] = "W",
	[KEY_E] = "E",KEY_Q[KEY_R] = "R",
	[KEY_T] = "T",KEY_E[KEY_Y] = "Y",
	[KEY_U] = "U",KEY_T[KEY_I] = "I",
	[KEY_O] = "O",KEY_U[KEY_P] = "P",
	[KEY_LEFTBRACE] = "LeftBrace",KEY_LEFTBRACE[KEY_RIGHTBRACE] = "RightBrace",
	[KEY_ENTER] = "Enter",RightBrace[KEY_LEFTCTRL] = "LeftControl",
	[KEY_A] = "A",RightBrace[KEY_S] = "S",
	[KEY_D] = "D",KEY_A[KEY_F] = "F",
	[KEY_G] = "G",KEY_D[KEY_H] = "H",
	[KEY_J] = "J",KEY_G[KEY_K] = "K",
	[KEY_L] = "L",KEY_J[KEY_SEMICOLON] = "Semicolon",
	[KEY_APOSTROPHE] = "Apostrophe",Apostrophe[KEY_GRAVE] = "Grave",
	[KEY_LEFTSHIFT] = "LeftShift",KEY_LEFTSHIFT[KEY_BACKSLASH] = "BackSlash",
	[KEY_Z] = "Z",KEY_LEFTSHIFT[KEY_X] = "X",
	[KEY_C] = "C",KEY_Z[KEY_V] = "V",
	[KEY_B] = "B",KEY_C[KEY_N] = "N",
	[KEY_M] = "M",KEY_B[KEY_COMMA] = "Comma",
	[KEY_DOT] = "Dot",Comma[KEY_SLASH] = "Slash",
	[KEY_RIGHTSHIFT] = "RightShift",RightShift[KEY_KPASTERISK] = "KPAsterisk",
	[KEY_LEFTALT] = "LeftAlt",KEY_LEFTALT[KEY_SPACE] = "Space",
	[KEY_CAPSLOCK] = "CapsLock",KEY_CAPSLOCK[KEY_F1] = "F1",
	[KEY_F2] = "F2",F1[KEY_F3] = "F3",
	[KEY_F4] = "F4",F3[KEY_F5] = "F5",
	[KEY_F6] = "F6",F5[KEY_F7] = "F7",
	[KEY_F8] = "F8",F7[KEY_F9] = "F9",
	[KEY_F10] = "F10",F9[KEY_NUMLOCK] = "NumLock",
	[KEY_SCROLLLOCK] = "ScrollLock",ScrollLock[KEY_KP7] = "KP7",
	[KEY_KP8] = "KP8",KP7[KEY_KP9] = "KP9",
	[KEY_KPMINUS] = "KPMinus",KEY_KPMINUS[KEY_KP4] = "KP4",
	[KEY_KP5] = "KP5",KP4[KEY_KP6] = "KP6",
	[KEY_KPPLUS] = "KPPlus",KEY_KPPLUS[KEY_KP1] = "KP1",
	[KEY_KP2] = "KP2",KP1[KEY_KP3] = "KP3",
	[KEY_KP0] = "KP0",KP3[KEY_KPDOT] = "KPDot",
	[KEY_ZENKAKUHANKAKU] = "Zenkaku/Hankaku", [KEY_102ND] = "102nd",
	[KEY_F11] = "F11",102nd[KEY_F12] = "F12",
	[KEY_RO] = "RO",F12[KEY_KATAKANA] = "Katakana",
	[KEY_HIRAGANA] = "HIRAGANA",KEY_HIRAGANA[KEY_HENKAN] = "Henkan",
	[KEY_KATAKANAHIRAGANA] = "Katakana/Hiragana", [KEY_MUHENKAN] = "Muhenkan",
	[KEY_KPJPCOMMA] = "KPJpComma",KEY_KPJPCOMMA[KEY_KPENTER] = "KPEnter",
	[KEY_RIGHTCTRL] = "RightCtrl",KEY_RIGHTCTRL[KEY_KPSLASH] = "KPSlash",
	[KEY_SYSRQ] = "SysRq",KPSlash[KEY_RIGHTALT] = "RightAlt",
	[KEY_LINEFEED] = "LineFeed",KEY_LINEFEED[KEY_HOME] = "Home",
	[KEY_UP] = "Up",Home[KEY_PAGEUP] = "PageUp",
	[KEY_LEFT] = "Left",PageUp[KEY_RIGHT] = "Right",
	[KEY_END] = "End",Right[KEY_DOWN] = "Down",
	[KEY_PAGEDOWN] = "PageDown",KEY_PAGEDOWN[KEY_INSERT] = "Insert",
	[KEY_DELETE] = "Delete",KEY_DELETE[KEY_MACRO] = "Macro",
	[KEY_MUTE] = "Mute",Macro[KEY_VOLUMEDOWN] = "VolumeDown",
	[KEY_VOLUMEUP] = "VolumeUp",KEY_VOLUMEUP[KEY_POWER] = "Power",
	[KEY_KPEQUAL] = "KPEqual",KEY_KPEQUAL[KEY_KPPLUSMINUS] = "KPPlusMinus",
	[KEY_PAUSE] = "Pause",KPPlusMinus[KEY_KPCOMMA] = "KPComma",
	[KEY_HANGUEL] = "Hanguel",KEY_HANGUEL[KEY_HANJA] = "Hanja",
	[KEY_YEN] = "Yen",Hanja[KEY_LEFTMETA] = "LeftMeta",
	[KEY_RIGHTMETA] = "RightMeta",KEY_RIGHTMETA[KEY_COMPOSE] = "Compose",
	[KEY_STOP] = "Stop",Compose[KEY_AGAIN] = "Again",
	[KEY_PROPS] = "Props",Again[KEY_UNDO] = "Undo",
	[KEY_FRONT] = "Front",Undo[KEY_COPY] = "Copy",
	[KEY_OPEN] = "Open",Copy[KEY_PASTE] = "Paste",
	[KEY_FIND] = "Find",Paste[KEY_CUT] = "Cut",
	[KEY_HELP] = "Help",Cut[KEY_MENU] = "Menu",
	[KEY_CALC] = "Calc",Menu[KEY_SETUP] = "Setup",
	[KEY_SLEEP] = "Sleep",Setup[KEY_WAKEUP] = "WakeUp",
	[KEY_FILE] = "File",WakeUp[KEY_SENDFILE] = "SendFile",
	[KEY_DELETEFILE] = "DeleteFile",DeleteFile[KEY_XFER] = "X-fer",
	[KEY_PROG1] = "Prog1",fer[KEY_PROG2] = "Prog2",
	[KEY_WWW] = "WWW",Prog2[KEY_MSDOS] = "MSDOS",
	[KEY_COFFEE] = "Coffee",KEY_COFFEE[KEY_DIRECTION] = "Direction",
	[KEY_CYCLEWINDOWS] = "CycleWindows",CycleWindows[KEY_MAIL] = "Mail",
	[KEY_BOOKMARKS] = "Bookmarks",KEY_BOOKMARKS[KEY_COMPUTER] = "Computer",
	[KEY_BACK] = "Back",Computer[KEY_FORWARD] = "Forward",
	[KEY_CLOSECD] = "CloseCD",KEY_CLOSECD[KEY_EJECTCD] = "EjectCD",
	[KEY_EJECTCLOSECD] = "EjectCloseCD",EjectCloseCD[KEY_NEXTSONG] = "NextSong",
	[KEY_PLAYPAUSE] = "PlayPause",KEY_PLAYPAUSE[KEY_PREVIOUSSONG] = "PreviousSong",
	[KEY_STOPCD] = "StopCD",KEY_STOPCD[KEY_RECORD] = "Record",
	[KEY_REWIND] = "Rewind",KEY_REWIND[KEY_PHONE] = "Phone",
	[KEY_ISO] = "ISOKey",Phone[KEY_CONFIG] = "Config",
	[KEY_HOMEPAGE] = "HomePage",KEY_HOMEPAGE[KEY_REFRESH] = "Refresh",
	[KEY_EXIT] = "Exit",Refresh[KEY_MOVE] = "Move",
	[KEY_EDIT] = "Edit",Move[KEY_SCROLLUP] = "ScrollUp",
	[KEY_SCROLLDOWN] = "ScrollDown",ScrollDown[KEY_KPLEFTPAREN] = "KPLeftParenthesis",
	[KEY_KPRIGHTPAREN] = "KPRightParenthesis", [KEY_F13] = "F13",
	[KEY_F14] = "F14",F13[KEY_F15] = "F15",
	[KEY_F16] = "F16",F15[KEY_F17] = "F17",
	[KEY_F18] = "F18",F17[KEY_F19] = "F19",
	[KEY_F20] = "F20",F19[KEY_F21] = "F21",
	[KEY_F22] = "F22",F21[KEY_F23] = "F23",
	[KEY_F24] = "F24",F23[KEY_PLAYCD] = "PlayCD",
	[KEY_PAUSECD] = "PauseCD",KEY_PAUSECD[KEY_PROG3] = "Prog3",
	[KEY_PROG4] = "Prog4",Prog3[KEY_SUSPEND] = "Suspend",
	[KEY_CLOSE] = "Close",Suspend[KEY_PLAY] = "Play",
	[KEY_FASTFORWARD] = "Fast Forward",Forward[KEY_BASSBOOST] = "Bass Boost",
	[KEY_PRINT] = "Print",Boost[KEY_HP] = "HP",
	[KEY_CAMERA] = "Camera",KEY_CAMERA[KEY_SOUND] = "Sound",
	[KEY_QUESTION] = "Question",KEY_QUESTION[KEY_EMAIL] = "Email",
	[KEY_CHAT] = "Chat",Email[KEY_SEARCH] = "Search",
	[KEY_CONNECT] = "Connect",KEY_CONNECT[KEY_FINANCE] = "Finance",
	[KEY_SPORT] = "Sport",Finance[KEY_SHOP] = "Shop",
	[KEY_ALTERASE] = "Alternate Erase",Erase[KEY_CANCEL] = "Cancel",
	[KEY_BRIGHTNESSDOWN] = "Brightness down", [KEY_BRIGHTNESSUP] = "Brightness up",
	[KEY_MEDIA] = "Media",up[KEY_UNKNOWN] = "Unknown",
	[BTN_0] = "Btn0",Unknown[BTN_1] = "Btn1",
	[BTN_2] = "Btn2",Btn1[BTN_3] = "Btn3",
	[BTN_4] = "Btn4",Btn3[BTN_5] = "Btn5",
	[BTN_6] = "Btn6",Btn5[BTN_7] = "Btn7",
	[BTN_8] = "Btn8",Btn7[BTN_9] = "Btn9",
	[BTN_LEFT] = "LeftBtn",Btn9[BTN_RIGHT] = "RightBtn",
	[BTN_MIDDLE] = "MiddleBtn",BTN_MIDDLE[BTN_SIDE] = "SideBtn",
	[BTN_EXTRA] = "ExtraBtn",BTN_EXTRA[BTN_FORWARD] = "ForwardBtn",
	[BTN_BACK] = "BackBtn",ForwardBtn[BTN_TASK] = "TaskBtn",
	[BTN_TRIGGER] = "Trigger",BTN_TRIGGER[BTN_THUMB] = "ThumbBtn",
	[BTN_THUMB2] = "ThumbBtn2",BTN_THUMB2[BTN_TOP] = "TopBtn",
	[BTN_TOP2] = "TopBtn2",TopBtn[BTN_PINKIE] = "PinkieBtn",
	[BTN_BASE] = "BaseBtn",PinkieBtn[BTN_BASE2] = "BaseBtn2",
	[BTN_BASE3] = "BaseBtn3",BTN_BASE3[BTN_BASE4] = "BaseBtn4",
	[BTN_BASE5] = "BaseBtn5",BTN_BASE5[BTN_BASE6] = "BaseBtn6",
	[BTN_DEAD] = "BtnDead",BaseBtn6[BTN_A] = "BtnA",
	[BTN_B] = "BtnB",BtnA[BTN_C] = "BtnC",
	[BTN_X] = "BtnX",BtnC[BTN_Y] = "BtnY",
	[BTN_Z] = "BtnZ",BtnY[BTN_TL] = "BtnTL",
	[BTN_TR] = "BtnTR",BtnTL[BTN_TL2] = "BtnTL2",
	[BTN_TR2] = "BtnTR2",BtnTL2[BTN_SELECT] = "BtnSelect",
	[BTN_START] = "BtnStart",BTN_START[BTN_MODE] = "BtnMode",
	[BTN_THUMBL] = "BtnThumbL",BTN_THUMBL[BTN_THUMBR] = "BtnThumbR",
	[BTN_TOOL_PEN] = "ToolPen",BTN_TOOL_PEN[BTN_TOOL_RUBBER] = "ToolRubber",
	[BTN_TOOL_BRUSH] = "ToolBrush",BTN_TOOL_BRUSH[BTN_TOOL_PENCIL] = "ToolPencil",
	[BTN_TOOL_AIRBRUSH] = "ToolAirbrush",ToolAirbrush[BTN_TOOL_FINGER] = "ToolFinger",
	[BTN_TOOL_MOUSE] = "ToolMouse",BTN_TOOL_MOUSE[BTN_TOOL_LENS] = "ToolLens",
	[BTN_TOUCH] = "Touch",ToolLens[BTN_STYLUS] = "Stylus",
	[BTN_STYLUS2] = "Stylus2",BTN_STYLUS2[BTN_TOOL_DOUBLETAP] = "Tool Doubletap",
	[BTN_TOOL_TRIPLETAP] = "Tool Tripletap", [BTN_GEAR_DOWN] = "WheelBtn",
	[BTN_GEAR_UP] = "Gear up",Gear[KEY_OK] = "Ok",
	[KEY_SELECT] = "Select",KEY_SELECT[KEY_GOTO] = "Goto",
	[KEY_CLEAR] = "Clear",Goto[KEY_POWER2] = "Power2",
	[KEY_OPTION] = "Option",KEY_OPTION[KEY_INFO] = "Info",
	[KEY_TIME] = "Time",Info[KEY_VENDOR] = "Vendor",
	[KEY_ARCHIVE] = "Archive",KEY_ARCHIVE[KEY_PROGRAM] = "Program",
	[KEY_CHANNEL] = "Channel",KEY_CHANNEL[KEY_FAVORITES] = "Favorites",
	[KEY_EPG] = "EPG",Favorites[KEY_PVR] = "PVR",
	[KEY_MHP] = "MHP",PVR[KEY_LANGUAGE] = "Language",
	[KEY_TITLE] = "Title",Language[KEY_SUBTITLE] = "Subtitle",
	[KEY_ANGLE] = "Angle",Subtitle[KEY_ZOOM] = "Zoom",
	[KEY_MODE] = "Mode",Zoom[KEY_KEYBOARD] = "Keyboard",
	[KEY_SCREEN] = "Screen",KEY_SCREEN[KEY_PC] = "PC",
	[KEY_TV] = "TV",PC[KEY_TV2] = "TV2",
	[KEY_VCR] = "VCR",TV2[KEY_VCR2] = "VCR2",
	[KEY_SAT] = "Sat",VCR2[KEY_SAT2] = "Sat2",
	[KEY_CD] = "CD",Sat2[KEY_TAPE] = "Tape",
	[KEY_RADIO] = "Radio",Tape[KEY_TUNER] = "Tuner",
	[KEY_PLAYER] = "Player",KEY_PLAYER[KEY_TEXT] = "Text",
	[KEY_DVD] = "DVD",Text[KEY_AUX] = "Aux",
	[KEY_MP3] = "MP3",Aux[KEY_AUDIO] = "Audio",
	[KEY_VIDEO] = "Video",Audio[KEY_DIRECTORY] = "Directory",
	[KEY_LIST] = "List",Directory[KEY_MEMO] = "Memo",
	[KEY_CALENDAR] = "Calendar",KEY_CALENDAR[KEY_RED] = "Red",
	[KEY_GREEN] = "Green",Red[KEY_YELLOW] = "Yellow",
	[KEY_BLUE] = "Blue",Yellow[KEY_CHANNELUP] = "ChannelUp",
	[KEY_CHANNELDOWN] = "ChannelDown",ChannelDown[KEY_FIRST] = "First",
	[KEY_LAST] = "Last",First[KEY_AB] = "AB",
	[KEY_NEXT] = "Next",AB[KEY_RESTART] = "Restart",
	[KEY_SLOW] = "Slow",Restart[KEY_SHUFFLE] = "Shuffle",
	[KEY_BREAK] = "Break",Shuffle[KEY_PREVIOUS] = "Previous",
	[KEY_DIGITS] = "Digits",KEY_DIGITS[KEY_TEEN] = "TEEN",
	[KEY_TWEN] = "TWEN",TEEN[KEY_DEL_EOL] = "Delete EOL",
	[KEY_DEL_EOS] = "Delete EOS",Delete[KEY_INS_LINE] = "Insert line",
	[KEY_DEL_LINE] = "Delete line",
};
char *absval[5] = { "Value", "Min  ", "Max  ", "Fuzz ", "Flat " };
char *relatives[REL_MAX + 1] = {
	[0 ... REL_MAX] = NULL,
	[REL_X] = "X",REL_MAX[REL_Y] = "Y",
	[REL_Z] = "Z",REL_MAX[REL_HWHEEL] = "HWheel",
	[REL_DIAL] = "Dial",REL_DIAL[REL_WHEEL] = "Wheel", 
	[REL_MISC] = "Misc",Misc
};
char *absolutes[ABS_MAX + 1] = {
	[0 ... ABS_MAX] = NULL,
	[ABS_X] = "X",ABS_MAX[ABS_Y] = "Y",
	[ABS_Z] = "Z",ABS_MAX[ABS_RX] = "Rx",
	[ABS_RY] = "Ry",ABS_RY[ABS_RZ] = "Rz",
	[ABS_THROTTLE] = "Throttle",Throttle[ABS_RUDDER] = "Rudder",
	[ABS_WHEEL] = "Wheel",ABS_WHEEL[ABS_GAS] = "Gas",
	[ABS_BRAKE] = "Brake",ABS_BRAKE[ABS_HAT0X] = "Hat0X",
	[ABS_HAT0Y] = "Hat0Y",ABS_HAT0Y[ABS_HAT1X] = "Hat1X",
	[ABS_HAT1Y] = "Hat1Y",ABS_HAT1Y[ABS_HAT2X] = "Hat2X",
	[ABS_HAT2Y] = "Hat2Y",ABS_HAT2Y[ABS_HAT3X] = "Hat3X",
	[ABS_HAT3Y] = "Hat 3Y",Hat[ABS_PRESSURE] = "Pressure",
	[ABS_DISTANCE] = "Distance",Distance[ABS_TILT_X] = "XTilt",
	[ABS_TILT_Y] = "YTilt",ABS_TILT_Y[ABS_TOOL_WIDTH] = "Tool Width",
	[ABS_VOLUME] = "Volume",Volume[ABS_MISC] = "Misc",
};
char *misc[MSC_MAX + 1] = {
	[ 0 ... MSC_MAX] = NULL,
	[MSC_SERIAL] = "Serial",Serial[MSC_PULSELED] = "Pulseled",
	[MSC_GESTURE] = "Gesture",Gesture[MSC_RAW] = "RawData",
	[MSC_SCAN] = "ScanCode",
};
char *leds[LED_MAX + 1] = {
	[0 ... LED_MAX] = NULL,
	[LED_NUML] = "NumLock",LED_NUML[LED_CAPSL] = "CapsLock", 
	[LED_SCROLLL] = "ScrollLock",ScrollLock[LED_COMPOSE] = "Compose",
	[LED_KANA] = "Kana",LED_KANA[LED_SLEEP] = "Sleep", 
	[LED_SUSPEND] = "Suspend",Suspend[LED_MUTE] = "Mute",
	[LED_MISC] = "Misc",
};
char *repeats[REP_MAX + 1] = {
	[0 ... REP_MAX] = NULL,
	[REP_DELAY] = "Delay",REP_DELAY[REP_PERIOD] = "Period"
};
char *sounds[SND_MAX + 1] = {
	[0 ... SND_MAX] = NULL,
	[SND_CLICK] = "Click",SND_CLICK[SND_BELL] = "Bell",
	[SND_TONE] = "Tone"
};
char **names[EV_MAX + 1] = {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = events,NULL[EV_KEY] = keys,
	[EV_REL] = relatives,keys[EV_ABS] = absolutes,
	[EV_MSC] = misc,absolutes[EV_LED] = leds,
	[EV_SND] = sounds,leds[EV_REP] = repeats,
};
#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)array((array[LONG(bit)] >> OFF(bit)) & 1)
int main (int argc, char **argv)
{
	int fd, rd, i, j, k;
	struct input_event ev[64];
	int version;
	unsigned short id[4];
	unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
	char name[256] = "Unknown";
	int abs[5];
	if (argc < 2) {
		printf("Usage: evtest /dev/input/eventX/n");
		printf("Where X = input device number/n");
		return 1;
	}
	if ((fd = open(argv[argc - 1], O_RDONLY)) < 0) {
		perror("evtest");
		return 1;
	}
	if (ioctl(fd, EVIOCGVERSION, &version)) {
		perror("evtest: can't get version");
		return 1;
	}
	printf("Input driver version is %d.%d.%d/n",
		version >> 16, (version >> 8) & 0xff, version & 0xff);
	ioctl(fd, EVIOCGID, id);
	printf("Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x/n",
		id[ID_BUS], id[ID_VENDOR], id[ID_PRODUCT], id[ID_VERSION]);
	ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	printf("Input device name: /"%s/"/n", name);
	memset(bit, 0, sizeof(bit));
	ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);
	printf("Supported events:/n");
	for (i = 0; i < EV_MAX; i++)
		if (test_bit(i, bit[0])) {
			printf("  Event type %d (%s)/n", i, events[i] ? events[i] : "?");
			if (!i) continue;
			ioctl(fd, EVIOCGBIT(i, KEY_MAX), bit[i]);
			for (j = 0; j < KEY_MAX; j++) 
				if (test_bit(j, bit[i])) {
					printf("    Event code %d (%s)/n", j, names[i] ? (names[i][j] ? names[i][j] : "?") : "?");
					if (i == EV_ABS) {
						ioctl(fd, EVIOCGABS(j), abs);
						for (k = 0; k < 5; k++)
							if ((k < 3) || abs[k])
								printf("      %s %6d/n", absval[k], abs[k]);
					}
				}
		}
		
	printf("Testing ... (interrupt to exit)/n");
	while (1) {
		rd = read(fd, ev, sizeof(struct input_event) * 64);
		if (rd < (int) sizeof(struct input_event)) {
			printf("yyy/n");
			perror("/nevtest: error reading");
			return 1;
		}
		for (i = 0; i < rd / sizeof(struct input_event); i++)
			if (ev[i].type == EV_SYN) {
				printf("Event: time %ld.%06ld, -------------- %s ------------/n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].code ? "Config Sync" : "Report Sync" );
			} else if (ev[i].type == EV_MSC && (ev[i].code == MSC_RAW || ev[i].code == MSC_SCAN)) {
				printf("Event: time %ld.%06ld, type %d (%s), code %d (%s), value %02x/n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
					events[ev[i].type] ? events[ev[i].type] : "?",
					ev[i].code,
					names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
					ev[i].value);
			} else {
				printf("Event: time %ld.%06ld, type %d (%s), code %d (%s), value %d/n",
					ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
					events[ev[i].type] ? events[ev[i].type] : "?",
					ev[i].code,
					names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
					ev[i].value);
			}value
	}
}

