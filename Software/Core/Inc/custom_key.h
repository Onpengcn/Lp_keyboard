#ifndef __CUSTOM_KEY_H
#define __CUSTOM_KEY_H

#define KEYMAP_NUM 3
#define IO_NUMBER 88

typedef  short int16_t;
typedef  unsigned char uint8_t;

enum KeyCode_t
    {
        /*------------------------- HID report data -------------------------*/
        LEFT_CTRL = -8, LEFT_SHIFT = -7, LEFT_ALT = -6, LEFT_GUI = -5,
        RIGHT_CTRL = -4, RIGHT_SHIFT = -3, RIGHT_ALT = -2, RIGHT_GUI = -1,

        RESERVED = 0, ERROR_ROLL_OVER, POST_FAIL, ERROR_UNDEFINED,
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        NUM_1/*1!*/, NUM_2/*2@*/, NUM_3/*3#*/, NUM_4/*4$*/, NUM_5/*5%*/,
        NUM_6/*6^*/, NUM_7/*7&*/, NUM_8/*8**/, NUM_9/*9(*/, NUM_0/*0)*/,
        ENTER, ESC, BACKSPACE, TAB, SPACE,
        MINUS/*-_*/, EQUAL/*=+*/, LEFT_U_BRACE/*[{*/, RIGHT_U_BRACE/*]}*/,
        BACKSLASH/*\|*/, NONE_US/**/, SEMI_COLON/*;:*/, QUOTE/*'"*/,
        GRAVE_ACCENT/*`~*/, COMMA/*,<*/, PERIOD/*.>*/, SLASH/*/?*/,
        CAP_LOCK, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        PRINT, SCROLL_LOCK, PAUSE, INSERT, HOME, PAGE_UP, DELETE, END, PAGE_DOWN,
        RIGHT_ARROW, LEFT_ARROW, DOWN_ARROW, UP_ARROW, PAD_NUM_LOCK,
        PAD_SLASH, PAD_ASTERISK, PAD_MINUS, PAD_PLUS, PAD_ENTER,
        PAD_NUM_1, PAD_NUM_2, PAD_NUM_3, PAD_NUM_4, PAD_NUM_5,
        PAD_NUM_6, PAD_NUM_7, PAD_NUM_8, PAD_NUM_9, PAD_NUM_0,
        PAD_PERIOD = 99, // followed with 20 bits 0, total 16 Bytes report data
        /*------------------------- HID report data -------------------------*/

//      /*-------------------------- Virtual keys ---------------------------*/
//      EXECUTE = 116,
//      HELP, MENU, SELECT, STOP, AGAIN, UNDO, CUT, COPY, PASTE, FIND, MUTE,
//      VOLUME_UP, VOLUME_DOWN,
//      /*-------------------------- Virtual keys ---------------------------*/

        /*--------------------------- Smart keys ----------------------------*/
        FN = 1000,
        TOUCH_BAR_1, TOUCH_BAR_2, TOUCH_BAR_3, TOUCH_BAR_4, TOUCH_BAR_5, TOUCH_BAR_6
        /*--------------------------- Smart keys ----------------------------*/
    };
int16_t leapmotor_keymap[KEYMAP_NUM][IO_NUMBER] = {
        // The first layer, used for aligning 74HC165 IO pins to PCB key layout
        {67,61, 60, 58, 59, 52, 55, 51, 50, 49, 48,47,46,3,
            80, 81,64,57,62,63,53,54,45,44,40,31,26,18,2,
            19,70, 71, 66, 65, 56, 36, 37, 38, 39, 43, 42, 41,28,1,
            15, 74, 73, 72, 68, 69, 29, 30, 35, 34, 33, 32,24,0,
            14, 76, 77, 78, 79, 16, 20, 21, 22,23, 27,25,17, 4,
            13,12, 8,75,9, 10,7,11, 6,5,
            85},

        // Other layer, used for mapping default key layout to custom layout
        {ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PAUSE,
            GRAVE_ACCENT, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0, MINUS, EQUAL, BACKSPACE, INSERT,
            TAB, Q,W,E,R,T,Y,U,I,O,P,LEFT_U_BRACE, RIGHT_U_BRACE, BACKSLASH, DELETE,
            CAP_LOCK, A,S,D,F,G,H,J,K,L,SEMI_COLON, QUOTE, ENTER, PAGE_UP,
            LEFT_SHIFT, Z,X,C,V,B,N,M,COMMA, PERIOD, SLASH, RIGHT_SHIFT, UP_ARROW, PAGE_DOWN,
            LEFT_CTRL, LEFT_GUI, LEFT_ALT, SPACE, RIGHT_ALT, RIGHT_GUI, FN, LEFT_ARROW, DOWN_ARROW, RIGHT_ARROW,
            LEFT_ARROW},

            {ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PAUSE,
            GRAVE_ACCENT, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_0, MINUS, EQUAL, BACKSPACE, INSERT,
            TAB, Q,W,E,R,T,Y,U,I,O,P,LEFT_U_BRACE, RIGHT_U_BRACE, BACKSLASH, DELETE,
            CAP_LOCK, A,S,D,F,G,H,J,K,L,SEMI_COLON, QUOTE, ENTER, PAGE_UP,
            LEFT_SHIFT, Z,X,C,V,B,N,M,COMMA, PERIOD, SLASH, RIGHT_SHIFT, A,PAGE_DOWN,
            LEFT_CTRL, LEFT_GUI, LEFT_ALT, SPACE, RIGHT_ALT, RIGHT_GUI, FN, LEFT_ARROW, C,RIGHT_ARROW,
            LEFT_ARROW}
    };

void get_key_report_val(uint8_t *buf, enum KeyCode_t code)
{
	int16_t index, bitIndex;
	index = (int16_t) (code / 8 + 1); // +1 for modifier
	bitIndex = (int16_t) (code % 8);
	buf[index] |= (1 << (bitIndex)); // +1 for Report-ID
}

#endif
