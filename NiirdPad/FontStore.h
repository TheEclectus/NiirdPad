#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "SDL_FontCache.h"

/// <summary>
/// Loads and stores FC_Fonts.
/// </summary>
class FontStore
{
public:
	enum class Role : uint8_t
	{
		NodeComment,
		NodeVisibilityScript,
		NodeFunction,
		NodeText,

		COUNT
	};

private:
	SDL_Renderer *_AssociatedRenderer;
	FC_Font *_Fonts[static_cast<uint8_t>(Role::COUNT)];

public:
	FontStore(SDL_Renderer *AssociatedRenderer);
	~FontStore();

	FC_Font *GetFont(const Role FontRole) const;
	void ResetFonts(const SDL_EventType &Type);
};