/*
 * hw_helper.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: jbr373
 */

#include "hw_controls.h"
#include "keys.h"

#include <3ds.h>
//#include <string>

HW_Controls::HW_Controls() {}

HW_Controls::~HW_Controls() {}

u8 HW_Controls::CheckPressedKey(s16 &T_X, s16 &T_Y, u8 &KeyboardState)
{
	if (KeyboardState == STATE_SPECIAL)
		return CheckPressedKeySpecial(T_X, T_Y);
	else if (KeyboardState == STATE_ABC) // Actually useless
		return CheckPressedKeyABC(T_X, T_Y);
	else
		return CheckPressedKeyABC(T_X, T_Y);
}

u8 HW_Controls::CheckPressedKeyABC(s16 &T_X, s16 &T_Y)
{
	// 0 - 9
	if (T_Y < 41)
	{
		if (T_X < 33)
			return HBKB_KEY_1;
		else if (T_X > 32 && T_X < 65)
			return HBKB_KEY_2;
		else if (T_X > 64 && T_X < 97)
			return HBKB_KEY_3;
		else if (T_X > 96 && T_X < 129)
			return HBKB_KEY_4;
		else if (T_X > 128 && T_X < 161)
			return HBKB_KEY_5;
		else if (T_X > 160 && T_X < 193)
			return HBKB_KEY_6;
		else if (T_X > 192 && T_X < 225)
			return HBKB_KEY_7;
		else if (T_X > 224 && T_X < 257)
			return HBKB_KEY_8;
		else if (T_X > 256 && T_X < 289)
			return HBKB_KEY_9;
		else if (T_X > 288)
			return HBKB_KEY_0;
		else
			return HBKB_KEY_NONE;
	}
	else if (T_Y > 40 && T_Y < 81) // QWERTYUIOP
	{
		if (T_X < 33)
			return HBKB_KEY_Q;
		else if (T_X > 32 && T_X < 65)
			return HBKB_KEY_W;
		else if (T_X > 64 && T_X < 97)
			return HBKB_KEY_E;
		else if (T_X > 96 && T_X < 129)
			return HBKB_KEY_R;
		else if (T_X > 128 && T_X < 161)
			return HBKB_KEY_T;
		else if (T_X > 160 && T_X < 193)
			return HBKB_KEY_Y;
		else if (T_X > 192 && T_X < 225)
			return HBKB_KEY_U;
		else if (T_X > 224 && T_X < 257)
			return HBKB_KEY_I;
		else if (T_X > 256 && T_X < 289)
			return HBKB_KEY_O;
		else if (T_X > 288)
			return HBKB_KEY_P;
		else
			return HBKB_KEY_NONE;
	}
	else if (T_Y > 80 && T_Y < 121) // ASDFGHJKL
	{
		if (T_X < 33)
			return HBKB_KEY_A;
		else if (T_X > 32 && T_X < 65)
			return HBKB_KEY_S;
		else if (T_X > 64 && T_X < 97)
			return HBKB_KEY_D;
		else if (T_X > 96 && T_X < 129)
			return HBKB_KEY_F;
		else if (T_X > 128 && T_X < 161)
			return HBKB_KEY_G;
		else if (T_X > 160 && T_X < 193)
			return HBKB_KEY_H;
		else if (T_X > 192 && T_X < 225)
			return HBKB_KEY_J;
		else if (T_X > 224 && T_X < 257)
			return HBKB_KEY_K;
		else if (T_X > 256 && T_X < 289)
			return HBKB_KEY_L;
		else if (T_X > 288)
			return HBKB_KEY_BACK; // Part of the Back Key is in tis Line
		else
			return HBKB_KEY_NONE;
	}
	else if (T_Y > 120 && T_Y < 161) // ZXCVBNM
	{
		if (T_X < 65)
			return HBKB_KEY_CHANGEKEYS; // Change to Special Characters
		else if (T_X > 64 && T_X < 97)
			return HBKB_KEY_Z;
		else if (T_X > 96 && T_X < 129)
			return HBKB_KEY_X;
		else if (T_X > 128 && T_X < 161)
			return HBKB_KEY_C;
		else if (T_X > 160 && T_X < 193)
			return HBKB_KEY_V;
		else if (T_X > 192 && T_X < 225)
			return HBKB_KEY_B;
		else if (T_X > 224 && T_X < 257)
			return HBKB_KEY_N;
		else if (T_X > 256 && T_X < 289)
			return HBKB_KEY_M;
		else if (T_X > 288)
			return HBKB_KEY_BACK; // Second Part of Key Back
		else
			return HBKB_KEY_NONE;
	}
	else if (T_Y > 160 && T_Y < 201) // Caps, Spacebar, Shift
	{
		if (T_X < 65)
			return HBKB_KEY_CAPS;
		else if (T_X > 64 && T_X < 257)
			return HBKB_KEY_SPACE;
		else if (T_X > 256)
			return HBKB_KEY_SHIFT;
		else
			return HBKB_KEY_NONE;
	}
	else if (T_Y > 200) // OK / Cancel
	{
		if (T_X < 161)
			return HBKB_KEY_CANCEL;
		else
			return HBKB_KEY_ENTER;
	}
	else
		return HBKB_KEY_NONE;
}

u8 HW_Controls::CheckPressedKeySpecial(s16 &T_X, s16 &T_Y)
{
	// 0 - 9
	if (T_Y < 41)
	{
		if (T_X < 33)
			return HBKB_KEY_1;
		else if (T_X > 32 && T_X < 65)
			return HBKB_KEY_2;
		else if (T_X > 64 && T_X < 97)
			return HBKB_KEY_3;
		else if (T_X > 96 && T_X < 129)
			return HBKB_KEY_4;
		else if (T_X > 128 && T_X < 161)
			return HBKB_KEY_5;
		else if (T_X > 160 && T_X < 193)
			return HBKB_KEY_6;
		else if (T_X > 192 && T_X < 225)
			return HBKB_KEY_7;
		else if (T_X > 224 && T_X < 257)
			return HBKB_KEY_8;
		else if (T_X > 256 && T_X < 289)
			return HBKB_KEY_9;
		else if (T_X > 288)
			return HBKB_KEY_0;
		else
			return HBKB_KEY_NONE;
	}
	else if (T_Y > 40 && T_Y < 81) // !"§$%&/()=
	{
		if (T_X < 33)
			return HBKB_KEY_SPECIAL_A;
		else if (T_X > 32 && T_X < 65)
			return HBKB_KEY_SPECIAL_B;
		else if (T_X > 64 && T_X < 97)
			return HBKB_KEY_SPECIAL_C;
		else if (T_X > 96 && T_X < 129)
			return HBKB_KEY_SPECIAL_D;
		else if (T_X > 128 && T_X < 161)
			return HBKB_KEY_SPECIAL_E;
		else if (T_X > 160 && T_X < 193)
			return HBKB_KEY_SPECIAL_F;
		else if (T_X > 192 && T_X < 225)
			return HBKB_KEY_SPECIAL_G;
		else if (T_X > 224 && T_X < 257)
			return HBKB_KEY_SPECIAL_H;
		else if (T_X > 256 && T_X < 289)
			return HBKB_KEY_SPECIAL_I;
		else if (T_X > 288)
			return HBKB_KEY_SPECIAL_J;
		else
			return HBKB_KEY_NONE;
	}
	else if (T_Y > 80 && T_Y < 121) // ASDFGHJKL
	{
		if (T_X < 33)
			return HBKB_KEY_SPECIAL_K;
		else if (T_X > 32 && T_X < 65)
			return HBKB_KEY_SPECIAL_L;
		else if (T_X > 64 && T_X < 97)
			return HBKB_KEY_SPECIAL_M;
		else if (T_X > 96 && T_X < 129)
			return HBKB_KEY_SPECIAL_N;
		else if (T_X > 128 && T_X < 161)
			return HBKB_KEY_SPECIAL_O;
		else if (T_X > 160 && T_X < 193)
			return HBKB_KEY_SPECIAL_P;
		else if (T_X > 192 && T_X < 225)
			return HBKB_KEY_SPECIAL_Q;
		else if (T_X > 224 && T_X < 257)
			return HBKB_KEY_SPECIAL_R;
		else if (T_X > 256 && T_X < 289)
			return HBKB_KEY_SPECIAL_S;
		else if (T_X > 288)
			return HBKB_KEY_BACK; // Part of the Back Key is in tis Line
		else
			return HBKB_KEY_NONE;
	}
	else if (T_Y > 120 && T_Y < 161) // ZXCVBNM
	{
		if (T_X < 65)
			return HBKB_KEY_CHANGEKEYS; // Change to ABC Characters
		else if (T_X > 64 && T_X < 97)
			return HBKB_KEY_SPECIAL_T;
		else if (T_X > 96 && T_X < 129)
			return HBKB_KEY_SPECIAL_U;
		else if (T_X > 128 && T_X < 161)
			return HBKB_KEY_SPECIAL_V;
		else if (T_X > 160 && T_X < 193)
			return HBKB_KEY_SPECIAL_W;
		else if (T_X > 192 && T_X < 225)
			return HBKB_KEY_SPECIAL_X;
		else if (T_X > 224 && T_X < 257)
			return HBKB_KEY_SPECIAL_Y;
		else if (T_X > 256 && T_X < 289)
			return HBKB_KEY_SPECIAL_Z;
		else if (T_X > 288)
			return HBKB_KEY_BACK; // Second Part of Key Back
		else
			return HBKB_KEY_NONE;
	}
	else if (T_Y > 160 && T_Y < 201) // Caps, Spacebar, Shift
	{
		if (T_X < 65)
			return HBKB_KEY_CAPS;
		else if (T_X > 64 && T_X < 257)
			return HBKB_KEY_SPACE;
		else if (T_X > 256)
			return HBKB_KEY_SHIFT;
		else
			return HBKB_KEY_NONE;
	}
	else if (T_Y > 200) // OK / Cancel
	{
		if (T_X < 161)
			return HBKB_KEY_CANCEL;
		else
			return HBKB_KEY_ENTER;
	}
	else
		return HBKB_KEY_NONE;
}
