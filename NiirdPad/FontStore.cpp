#include "FontStore.h"

FontStore::FontStore(SDL_Renderer *AssociatedRenderer) :
	_AssociatedRenderer(AssociatedRenderer)
{
	for (auto &Font : _Fonts)
		Font = nullptr;

	auto FontNodeComment = FC_CreateFont();
	//FC_LoadFont(FontNodeComment, AssociatedRenderer, "segoeuil.ttf", 12, { 255, 255, 255, 255 }, 0);
	FC_LoadFont(FontNodeComment, AssociatedRenderer, "sserife.fon", 12, { 255, 255, 255, 255 }, 0);
	_Fonts[static_cast<uint8_t>(Role::NodeComment)] = FontNodeComment;

	//_Fonts[static_cast<uint8_t>(Role::NodeComment)] = FC_CreateFont();
	//FC_LoadFont(_Fonts[static_cast<uint8_t>(Role::NodeComment)], AssociatedRenderer, "segoeuil.ttf", 14, { 255, 255, 255, 255 }, 0);
}

FontStore::~FontStore()
{
	for (uint8_t i = 0u; i < static_cast<uint8_t>(Role::COUNT); i++)
	{
		FC_FreeFont(_Fonts[i]);
	}
}

FC_Font *FontStore::GetFont(const Role FontRole) const
{
	if (FontRole == Role::COUNT)
		return nullptr;

	return _Fonts[static_cast<uint8_t>(FontRole)];
}

void FontStore::ResetFonts(const SDL_EventType &Type)
{
	for (auto Font : _Fonts)
	{
		if(Font != nullptr)
			FC_ResetFontFromRendererReset(Font, _AssociatedRenderer, Type);
	}
}