#include "BitmapText.h"
#include <assert.h>
#include "Colors.h"
#include "SpriteEffect.h"
#include "Surface.h"

BitmapText::BitmapText(Graphics& gfx)
	:
	gfx(gfx)
{
	for (int i = 0; i < (int)Font::Count; i++)
	{
		fontSpriteSheets.emplace_back(Surface::FromFile(fontFiles[i]));
	}
	//fontSpriteSheets.emplace_back(Surface::FromFile(fontFiles[0]));
}

RectI BitmapText::GetChar(char ch, Font font)
{
	int i = ch - ' ';
	int fontWidth = fontSizes[(int)font].x;
	int fontHeight = fontSizes[(int)font].y;
	assert(ch >= ' ');
	assert(ch <= ('~' + 1));
	//return { { (i%32) * fontWidth , (i/32)* fontHeight},fontWidth,fontHeight };
	//                  L                     T
	return { (i / 32) * fontHeight, (i / 32) * fontHeight+fontHeight, (i % 32) * fontWidth , (i % 32) * fontWidth + fontWidth};
	// t,b,l,r
}

void BitmapText::DrawChar(int x, int y, Color textColor, char ch, Font font)
{
	// create effect functor
	//SpriteEffect::Substitution e{ Colors::White, textColor };
	//SpriteEffect::Copy e;
	SpriteEffect::Chroma e_c(Colors::Black);
	SpriteEffect::Substitution e_s(4294967295, Colors::Red);
	// draw the character
	gfx.DrawSprite( x , y , GetChar(ch,font) , gfx.GetScreenRect() , fontSpriteSheets[int(font)] , e_s);
}

void BitmapText::DrawString(int x, int y, Color textColor, std::string str, Font font)
{
	int fontWidth = fontSizes[(int)font].x;
	int i = 0;
	for (char ch : str)
	{
		DrawChar(x + fontWidth *  i++  , y , textColor, ch , font);
	}
}
