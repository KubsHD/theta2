#pragma once

#include <core/types.h>
#include <glm/ext/matrix_transform.hpp>

class Camera {
	glm::mat4 m_viewMatrix;
public:
	Camera() : position(0,0) {
		m_viewMatrix = glm::mat4(1.0f);
	};

	inline glm::mat4 get_matrix() {
		return glm::translate(m_viewMatrix, Vec3(-position, 0.0));
	}

	Vec2 position;
	float zoom = 1.0f;
};

class Window;

class Texture
{
public:
	Texture(String path);
	~Texture();

	unsigned int id;
	Vec2 size;
};


class Subtexture
{
public:
	Subtexture(Texture* sheetTex, Vec2 pos, Vec2 size);
	~Subtexture();
	Texture* tex;
	Vec2 texSize;
	unsigned int vboId;
	unsigned int vaoId;
};

class Target
{
public:
	Target(int w, int h);
	~Target();

	void bind();

	unsigned int id;
	unsigned int texId;

	Vec2 target_size;
};

class Shader
{
public:
	Shader(const char* vtx, const char* fsx);
	int id;
};

class Renderer {
	Camera* m_currentCamera;
	Target* m_currentTarget;

public:
	static Target* Backbuffer;

	void init(Window* win);
	void clear();

	void set_target(Target* tg);
	void set_mvp(const glm::mat4& mvp);
	void set_uniform_vec2(String uniformName, Vec2 v);

	inline void set_camera(Camera* cam) { m_currentCamera = cam; };

	void draw_target(Target* tg);
	void draw_tex(Texture* tex, Vec2 pos);
	void draw_subtex(Subtexture* subTex, Vec2 pos);
	void draw_box(Vec2 pos, Vec2 size, Vec3 color);
	void draw_quad();
};