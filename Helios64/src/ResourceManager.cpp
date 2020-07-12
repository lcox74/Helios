#include "ResourceManager.h"

#include <iostream>

const int default_font_size = 16;
TTF_Font* default_regular_font = nullptr;
TTF_Font* default_light_font = nullptr;
TTF_Font* default_semi_light_font = nullptr;
TTF_Font* default_semi_bold_font = nullptr;
TTF_Font* default_bold_font = nullptr;
TTF_Font* default_glyph_font = nullptr;

std::vector<SDL_Renderer*> renderer_list;

TTF_Font* Helios::Resources::GetFont(int size, int font_type)
{
	if (size == default_font_size) {
		switch (font_type) {
		case FONT_TYPE::LIGHT:
			if (default_light_font == nullptr) default_light_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeuil.ttf", default_font_size);
			return default_light_font;
		case FONT_TYPE::SEMI_LIGHT:
			if (default_semi_light_font == nullptr) default_semi_light_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeuisl.ttf", default_font_size);
			return default_semi_light_font;
		case FONT_TYPE::SEMI_BOLD:
			if (default_semi_bold_font == nullptr) default_semi_bold_font = TTF_OpenFont("C:\\Windows\\Fonts\\seguisb.ttf", default_font_size);
			return default_semi_bold_font;
		case FONT_TYPE::BOLD:
			if (default_bold_font == nullptr) default_bold_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeuib.ttf", default_font_size);
			return default_bold_font;
		default:
			if (default_regular_font == nullptr) default_regular_font = TTF_OpenFont("C:\\Windows\\Fonts\\segoeui.ttf", default_font_size);
			return default_regular_font;
		}
	}

	switch (font_type) {
	case FONT_TYPE::LIGHT:		return TTF_OpenFont("C:\\Windows\\Fonts\\segoeuil.ttf", size); break;
	case FONT_TYPE::SEMI_LIGHT: return TTF_OpenFont("C:\\Windows\\Fonts\\segoeuisl.ttf", size); break;
	case FONT_TYPE::SEMI_BOLD:	return TTF_OpenFont("C:\\Windows\\Fonts\\seguisb.ttf", size); break;
	case FONT_TYPE::BOLD:		return TTF_OpenFont("C:\\Windows\\Fonts\\segoeuib.ttf", size); break;
	default:					return TTF_OpenFont("C:\\Windows\\Fonts\\segoeui.ttf", size);
	}
}

TTF_Font* Helios::Resources::GetGlyph(int size)
{
	if (size == default_font_size) {
		if (default_glyph_font == nullptr) default_glyph_font = TTF_OpenFont("C:\\Windows\\Fonts\\segmdl2.ttf", default_font_size);
		return default_glyph_font;
	}
	return TTF_OpenFont("C:\\Windows\\Fonts\\segmdl2.ttf", size);
}

void Helios::Resources::RenderText(int monitor, const char* text, int x, int y, int size, int justification, int font_type, SDL_Color colour)
{
	SDL_Renderer* renderer = GetRenderer(monitor);;
	if (renderer == nullptr) return;

	TTF_Font* text_font = GetFont(size, font_type);

	SDL_Surface* text_surface = TTF_RenderText_Blended(text_font, text, colour);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	int text_width = 0, text_height = 0;
	SDL_QueryTexture(text_texture, NULL, NULL, &text_width, &text_height);

	int altered_justification = x;
	if (justification == TEXT_ALIGN::CENTRE) altered_justification -= (int)(text_width / 2.0f);
	else if (justification == TEXT_ALIGN::RIGHT) altered_justification -= text_width;

	SDL_Rect text_rect{ altered_justification, y, text_width, text_height };
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	// Remove Font if not default
	if (size != default_font_size) {
		TTF_CloseFont(text_font);
		text_font = nullptr;
	}

	SDL_FreeSurface(text_surface);		text_surface = nullptr;
	SDL_DestroyTexture(text_texture);	text_texture = nullptr;
}

void Helios::Resources::RenderGlyph(int monitor, unsigned short glyph, int x, int y, int size, SDL_Color colour)
{
	SDL_Renderer* renderer = GetRenderer(monitor);
	if (renderer == nullptr) return;

	TTF_Font* glyph_font = GetGlyph(size);

	SDL_Surface* glyph_surface = TTF_RenderGlyph_Blended(glyph_font, glyph, colour);
	SDL_Texture* glyph_texture = SDL_CreateTextureFromSurface(renderer, glyph_surface);

	int glyph_width = 0, glyph_height = 0;
	SDL_QueryTexture(glyph_texture, NULL, NULL, &glyph_width, &glyph_height);

	SDL_Rect glyph_rect{ x, y, glyph_width, glyph_height };
	SDL_RenderCopy(renderer, glyph_texture, NULL, &glyph_rect);

	// Remove Font if not default
	if (size != default_font_size) {
		TTF_CloseFont(glyph_font);
		glyph_font = nullptr;
	}

	SDL_FreeSurface(glyph_surface);		glyph_surface = nullptr;
	SDL_DestroyTexture(glyph_texture);	glyph_texture = nullptr;
}

void Helios::Resources::SetRenderer(int monitor, SDL_Renderer* renderer)
{
	if (monitor < 0) return;

	if (monitor >= renderer_list.size()) renderer_list.push_back(renderer);
	else renderer_list.at(monitor) = renderer;
}

SDL_Renderer* Helios::Resources::GetRenderer(int monitor)
{
	if (monitor < 0) return nullptr;

	if (monitor > renderer_list.size()) return nullptr;
	return renderer_list.at(monitor);
}

void Helios::Resources::CleanResources()
{
	TTF_CloseFont(default_regular_font);		default_regular_font = nullptr;
	TTF_CloseFont(default_light_font);			default_light_font = nullptr;
	TTF_CloseFont(default_semi_light_font);		default_semi_light_font = nullptr;
	TTF_CloseFont(default_semi_bold_font);		default_semi_bold_font = nullptr;
	TTF_CloseFont(default_bold_font);			default_bold_font = nullptr;
	TTF_CloseFont(default_glyph_font);			default_glyph_font = nullptr;
}
