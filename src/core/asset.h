#pragma once

#include <core/types.h>

class Renderer;
class Texture;
class Font;
class Sound;

class Asset
{
private:
	static Map<String, Texture*> cache_texture;
	static Map<String, Sound*> cache_sound;
	static Map<String, Font*> cache_font;

	Renderer* renderer_ref;
public:

	void init(Renderer* ren);

	static Texture* load_texture(String path);
	static Sound* load_sound(String path);
	static Font* load_font(String path);


	static std::string read_file(String filePath);
};