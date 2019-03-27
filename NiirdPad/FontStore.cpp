#include "FontStore.h"

FontStore::FontStore(SDL_Renderer *AssociatedRenderer)
{
	for (uint8_t i = 0u; i < static_cast<uint8_t>(Role::COUNT); i++)
	{
		_Fonts[i] = FC_CreateFont();
		FC_LoadFont(_Fonts[i], AssociatedRenderer, "segoeuil.ttf", 14, { 255, 255, 255, 255 }, 0);
	}

	//FC_LoadFont(_Fonts[static_cast<uint8_t>(Role::NodeComment)], AssociatedRenderer, "segoeuil.ttf", 14, { 255, 255, 255, 255 }, 0);
}

FontStore::~FontStore()
{
	for (uint8_t i = 0u; i < static_cast<uint8_t>(Role::COUNT); i++)
	{
		FC_FreeFont(_Fonts[i]);
	}
}

FC_Font *FontStore::GetFont(const Role FontRole)
{
	if (FontRole == Role::COUNT)
		return nullptr;

	return _Fonts[static_cast<uint8_t>(FontRole)];
}