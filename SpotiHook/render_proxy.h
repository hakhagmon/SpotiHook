#pragma once
#include "overlay.h"

void line(int x, int y, int x2, int y2, zgui::color c) noexcept
{
	DrawLine2(x, y, x2, y2, 20, true, c);
}

void rect(int x, int y, int x2, int y2, zgui::color c) noexcept
{
	BorderedBox(x, y, x2, y2, 1, c);
}

void filled_rect(int x, int y, int x2, int y2, zgui::color c) noexcept
{
	FilledBox(x, y, x2, y2, c);
}

void text(int x, int y, zgui::color color, int font, bool center, const char* text) noexcept
{
	String(x, y, color, pFont, true, text);
	/*if (center)
	{
		int32_t w, h;
		client.surface->GetTextSize(font, (const wchar_t*)text, w, h);
		client.surface->SetTextPos(x - (w / 2), y);
	}
	else
		client.surface->SetTextPos(x, y);
	client.surface->DrawText(text); */
}

void get_text_size(unsigned long font, const char* text, int& width, int& height) noexcept
{
	RECT rcRect = { 0,0,0,0 };
	if (pFont)
	{
		// calculate required rect
		pFont->DrawText(NULL, text, strlen(text), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}
	width = rcRect.right - rcRect.left;
	height = rcRect.bottom - rcRect.top;
}

float get_frametime() noexcept
{
	//return client.globals->frametime; // fixme ghetto
	return 0.1;
}