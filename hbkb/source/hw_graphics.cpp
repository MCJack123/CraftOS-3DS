/*
 * hw_graphics.cpp
 *
 *  Created on: Sep 11, 2015
 *      Author: jb373
 */

#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hw_graphics.h"
#include "keys.h"

// Graphics
#include "hbkb_abc_lower_bin.h" // 320x240 Keyboard ABC (Lowercase Letters)
#include "hbkb_abc_upper_bin.h" // 320x240 Keyboard ABC (Uppercase Letters)
#include "hbkb_special_bin.h" // 320x240 Keyboard Special Characters
#include "hbkb_normal_sel_bin.h" // Normal Key Selection
#include "hbkb_special_sel_bin.h" // Special Key Shadow
#include "hbkb_shift_sel_bin.h" // Shift Key Selection
#include "hbkb_caps_sel_bin.h" // Caps Key Selection
#include "hbkb_back_sel_bin.h" // Back Key Selection
#include "hbkb_space_sel_bin.h" // Space Key Selection

HW_Graphics::HW_Graphics() {}

HW_Graphics::~HW_Graphics() {}

void HW_Graphics::DrawKeyboard(u8 &KeyboadState, s16 &T_X, s16 &T_Y, bool &isShift, bool &isCaps, u8 &Key)
{
	// Draw Background
	if (KeyboadState == STATE_ABC) // ABC Characters
	{
		if (isShift || isCaps) // Draw Keyboard with Uppercase Letters
			gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)hbkb_abc_upper_bin, 240, 320, 0, 0);
		else // Lowercase Letters
			gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)hbkb_abc_lower_bin, 240, 320, 0, 0);
	}
	else if (KeyboadState == STATE_SPECIAL) // Special Characters
		gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)hbkb_special_bin, 240, 320, 0, 0);
	else
		gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)hbkb_abc_lower_bin, 240, 320, 0, 0);

	// Draw Selected Keys
	DrawKeyShadow(Key, isShift, isCaps);
}

void HW_Graphics::DrawKeyShadow(u8 &Key, bool &isShift, bool &isCaps)
{
	// X = Y and Y = X in the 3DS FrameBuffer!
	// TODO: This is so incredibly ugly handled...

	// Shift and Caps can be active without pressing by the User
	if (isShift) // 40, 64, 160, 257
		gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)hbkb_shift_sel_bin, 40, 64, 40, 256);

	if (isCaps)
		gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)hbkb_caps_sel_bin, 40, 64, 40, 0);

	// Other Keys
	if (Key == HBKB_KEY_CHANGEKEYS)
		gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)hbkb_special_sel_bin, 40, 64, 80, 0);
	else if (Key == HBKB_KEY_BACK)
		gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)hbkb_back_sel_bin, 80, 32, 80, 288);
	else if (Key == HBKB_KEY_SPACE)
		gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)hbkb_space_sel_bin, 40, 192, 40, 64);
	else
	{

	// Set X, Y Holders
	s16 GraphicX, GraphicY;

	// Assign X | Y Coords
	if (Key == HBKB_KEY_1) // First Row
		{GraphicX = 200; GraphicY = 0;}
	else if (Key == HBKB_KEY_2)
		{GraphicX = 200; GraphicY = 32;}
	else if (Key == HBKB_KEY_3)
		{GraphicX = 200; GraphicY = 64;}
	else if (Key == HBKB_KEY_4)
		{GraphicX = 200; GraphicY = 96;}
	else if (Key == HBKB_KEY_5)
		{GraphicX = 200; GraphicY = 128;}
	else if (Key == HBKB_KEY_6)
		{GraphicX = 200; GraphicY = 160;}
	else if (Key == HBKB_KEY_7)
		{GraphicX = 200; GraphicY = 192;}
	else if (Key == HBKB_KEY_8)
		{GraphicX = 200; GraphicY = 224;}
	else if (Key == HBKB_KEY_9)
		{GraphicX = 200; GraphicY = 256;}
	else if (Key == HBKB_KEY_0)
		{GraphicX = 200; GraphicY = 288;}
	else if (Key == HBKB_KEY_Q || Key == HBKB_KEY_SPECIAL_A) // Second Row
		{GraphicX = 160; GraphicY = 0;}
	else if (Key == HBKB_KEY_W || Key == HBKB_KEY_SPECIAL_B)
		{GraphicX = 160; GraphicY = 32;}
	else if (Key == HBKB_KEY_E || Key == HBKB_KEY_SPECIAL_C)
		{GraphicX = 160; GraphicY = 64;}
	else if (Key == HBKB_KEY_R || Key == HBKB_KEY_SPECIAL_D)
		{GraphicX = 160; GraphicY = 96;}
	else if (Key == HBKB_KEY_T || Key == HBKB_KEY_SPECIAL_E)
		{GraphicX = 160; GraphicY = 128;}
	else if (Key == HBKB_KEY_Y || Key == HBKB_KEY_SPECIAL_F)
		{GraphicX = 160; GraphicY = 160;}
	else if (Key == HBKB_KEY_U || Key == HBKB_KEY_SPECIAL_G)
		{GraphicX = 160; GraphicY = 192;}
	else if (Key == HBKB_KEY_I || Key == HBKB_KEY_SPECIAL_H)
		{GraphicX = 160; GraphicY = 224;}
	else if (Key == HBKB_KEY_O || Key == HBKB_KEY_SPECIAL_I)
		{GraphicX = 160; GraphicY = 256;}
	else if (Key == HBKB_KEY_P || Key == HBKB_KEY_SPECIAL_J)
		{GraphicX = 160; GraphicY = 288;}
	else if (Key == HBKB_KEY_A || Key == HBKB_KEY_SPECIAL_K) // Third Row
		{GraphicX = 120; GraphicY = 0;}
	else if (Key == HBKB_KEY_S || Key == HBKB_KEY_SPECIAL_L)
		{GraphicX = 120; GraphicY = 32;}
	else if (Key == HBKB_KEY_D || Key == HBKB_KEY_SPECIAL_M)
		{GraphicX = 120; GraphicY = 64;}
	else if (Key == HBKB_KEY_F || Key == HBKB_KEY_SPECIAL_N)
		{GraphicX = 120; GraphicY = 96;}
	else if (Key == HBKB_KEY_G || Key == HBKB_KEY_SPECIAL_O)
		{GraphicX = 120; GraphicY = 128;}
	else if (Key == HBKB_KEY_H || Key == HBKB_KEY_SPECIAL_P)
		{GraphicX = 120; GraphicY = 160;}
	else if (Key == HBKB_KEY_J || Key == HBKB_KEY_SPECIAL_Q)
		{GraphicX = 120; GraphicY = 192;}
	else if (Key == HBKB_KEY_K || Key == HBKB_KEY_SPECIAL_R)
		{GraphicX = 120; GraphicY = 224;}
	else if (Key == HBKB_KEY_L || Key == HBKB_KEY_SPECIAL_S)
		{GraphicX = 120; GraphicY = 256;}
	else if (Key == HBKB_KEY_Z || Key == HBKB_KEY_SPECIAL_T) // Forth Row
		{GraphicX = 80; GraphicY = 64;}
	else if (Key == HBKB_KEY_X || Key == HBKB_KEY_SPECIAL_U)
		{GraphicX = 80; GraphicY = 96;}
	else if (Key == HBKB_KEY_C || Key == HBKB_KEY_SPECIAL_V)
		{GraphicX = 80; GraphicY = 128;}
	else if (Key == HBKB_KEY_V || Key == HBKB_KEY_SPECIAL_W)
		{GraphicX = 80; GraphicY = 160;}
	else if (Key == HBKB_KEY_B || Key == HBKB_KEY_SPECIAL_X)
		{GraphicX = 80; GraphicY = 192;}
	else if (Key == HBKB_KEY_N || Key == HBKB_KEY_SPECIAL_Y)
		{GraphicX = 80; GraphicY = 224;}
	else if (Key == HBKB_KEY_M || Key == HBKB_KEY_SPECIAL_Z)
		{GraphicX = 80; GraphicY = 256;}
	else
	{ GraphicX = 0; GraphicY = 0; }

	// Draw
	if (Key != HBKB_KEY_NONE && Key != HBKB_KEY_SHIFT && Key != HBKB_KEY_CAPS)
		gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)hbkb_normal_sel_bin, 40, 32, GraphicX, GraphicY);

	}
}

/*
 * Functions by Smealum
 * This Code was taken from Smealum's 3DS Homebrew Menu
 * http://www.github.com/smealum/3ds_hb_menu/
 */

void HW_Graphics::gfxDrawSprite(gfxScreen_t screen, gfx3dSide_t side, u8* spriteData, u16 width, u16 height, s16 x, s16 y)
{
	if(!spriteData)return;

	u16 fbWidth, fbHeight;
	u8* fbAdr=gfxGetFramebuffer(screen, side, &fbWidth, &fbHeight);

	if(x+width<0 || x>=fbWidth)return;
	if(y+height<0 || y>=fbHeight)return;

	u16 xOffset=0, yOffset=0;
	u16 widthDrawn=width, heightDrawn=height;

	if(x<0)xOffset=-x;
	if(y<0)yOffset=-y;
	if(x+width>=fbWidth)widthDrawn=fbWidth-x;
	if(y+height>=fbHeight)heightDrawn=fbHeight-y;
	widthDrawn-=xOffset;
	heightDrawn-=yOffset;

	int j;
	for(j=yOffset; j<yOffset+heightDrawn; j++)
	{
		memcpy(&fbAdr[((x+xOffset)+(y+j)*fbWidth)*3], &spriteData[((xOffset)+(j)*width)*3], widthDrawn*3);
	}
}

void HW_Graphics::gfxDrawSpriteAlpha(gfxScreen_t screen, gfx3dSide_t side, u8* spriteData, u16 width, u16 height, s16 x, s16 y)
{
	if(!spriteData)return;

	u16 fbWidth, fbHeight;
	u8* fbAdr=gfxGetFramebuffer(screen, side, &fbWidth, &fbHeight);

	if(x+width<0 || x>=fbWidth)return;
	if(y+height<0 || y>=fbHeight)return;

	u16 xOffset=0, yOffset=0;
	u16 widthDrawn=width, heightDrawn=height;

	if(x<0)xOffset=-x;
	if(y<0)yOffset=-y;
	if(x+width>=fbWidth)widthDrawn=fbWidth-x;
	if(y+height>=fbHeight)heightDrawn=fbHeight-y;
	widthDrawn-=xOffset;
	heightDrawn-=yOffset;

	//TODO : optimize
	fbAdr+=(y+yOffset)*fbWidth*3;
	spriteData+=yOffset*width*4;
	int j, i;
	for(j=yOffset; j<yOffset+heightDrawn; j++)
	{
		u8* fbd=&fbAdr[(x+xOffset)*3];
		u8* data=&spriteData[(xOffset)*4];
		for(i=xOffset; i<xOffset+widthDrawn; i++)
		{
			if(data[3])
			{
				fbd[0]=data[0];
				fbd[1]=data[1];
				fbd[2]=data[2];
			}
			fbd+=3;
			data+=4;
		}
		fbAdr+=fbWidth*3;
		spriteData+=width*4;
	}
}

