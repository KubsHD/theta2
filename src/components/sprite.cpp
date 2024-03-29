#include "sprite.h"
#include <core/asset.h>

Sprite::Sprite(String texPath)
{
	tex = Asset::load_texture(texPath);
};

void Sprite::change(String path)
{
	tex = Asset::load_texture(path);
}

void Sprite::update()
{

}

void Sprite::render(Renderer* ren)
{
	ren->draw_tex(tex, entity->position, 1.0f, flip);
	//ren->draw_box(entity->position, tex->size, Vec3(0.1f, 0.1f, 1.0f));
};
