/*****************************************************************//**
 * \file   Renderer.h
 * \brief  Contains main rendering functions
 * 
 * \author kubs
 * \date   January 2023
 *********************************************************************/

#pragma once




#include <core/types.h>
#include <glm/ext/matrix_transform.hpp>
#include <render/Shader.h>
#include <lib/glad/glad.h>

/// <summary>
/// Scene Camera
/// </summary>
class Camera {
	glm::mat4 m_viewMatrix;
public:

	/// <summary>
	/// Create a camera
	/// </summary>
	Camera() : position(0,0) {
		m_viewMatrix = glm::mat4(1.0f);
	};

	/// <summary>
	/// Get camera's view matrix
	/// </summary>
	/// <returns></returns>
	inline glm::mat4 get_matrix() {
		return glm::translate(m_viewMatrix, Vec3(-position, 0.0));
	}

	Vec2 position;
	float zoom = 1.0f;
};

class Window;

/// <summary>
/// Texture class
/// </summary>
class Texture
{
	/// <summary>
	/// Create a texture from file
	/// </summary>
	/// <param name="path">Path to texture</param>
	Texture(String path);

	/// <summary>
	/// Create a texture from raw bytes
	/// </summary>
	/// <param name="data">Raw bytes</param>
	Texture(std::vector<char> data);

	friend struct Atlas;
	friend class Asset;
public:

	~Texture();

	unsigned int id;
	Vec2 size;
};

/// <summary>
/// Texture inside of another texture
/// </summary>
class Subtexture
{
public:

	/// <summary>
	/// Create a subtexture
	/// </summary>
	/// <param name="sheetTex">Root texture</param>
	/// <param name="pos">Position of subtexture's top-left corner in a root texture (in px)</param>
	/// <param name="size">Size of a subtexture</param>
	Subtexture(Texture* sheetTex, Vec2 pos, Vec2 size);
	~Subtexture();
	Texture* tex;
	Vec2 texSize;
	unsigned int vboId;
	unsigned int vaoId;
};

/// <summary>
/// Describes scaling type of a given texture
/// </summary>
enum TargetScalingType {
	Nearest,
	Linear
};

/// <summary>
/// A texture that things can be rendered into
/// </summary>
class Target
{
public:

	/// <summary>
	/// Create a target
	/// </summary>
	/// <param name="w">Target width</param>
	/// <param name="h">Target height</param>
	/// <param name="type">Target scaling type</param>
	Target(int w, int h, TargetScalingType type = TargetScalingType::Nearest);
	~Target();

	/// <summary>
	/// Bind the target for rendering
	/// </summary>
	void bind();

	unsigned int id;
	unsigned int texId;

	Vec2 target_size;
};

/// <summary>
/// Describes a letter in a font file
/// </summary>
struct Glyph
{
	int id;
	int x;
	int y;
	int w;
	int h;
	int xoff;
	int yoff;
	int xadv;
	// not used

	int page;
	int chnl;

	Ref<Subtexture> subTex;

	/* data */
};

/// <summary>
/// Font class
/// </summary>
class Font {
private:
	friend class Asset;
	
	/// <summary>
	/// Create a font from a .fnt file
	/// </summary>
	/// <param name="path">Path to file</param>
	Font(String path);
public:

	String name;
	Texture* atlas;
	Glyph glyphs[200];
};

/// <summary>
/// Main renderer
/// NOTE: All rendered things have origin at the top-left corner
/// </summary>
class Renderer {
	Camera* m_currentCamera;
	Target* m_currentTarget;

	Shader* m_circleShader;
	Shader* m_defaultShader;
	Shader* m_filledBoxShader;

	void set_required_uniforms(Shader* s, glm::mat4 mvp, float opacity);

public:
	static Target* Backbuffer;
	static Font* DefaultFont;

	/// <summary>
	/// Initialize renderer for a given window
	/// </summary>
	/// <param name="win">window</param>
	void init(Window* win);

	/// <summary>
	/// Clear screen with given color
	/// </summary>
	/// <param name="color">Color to clear the screen with</param>
	void clear(Vec3 color = Vec3(0.03f, 0.25f, 0.03f));

	/// <summary>
	/// Update rendering size
	/// </summary>
	/// <param name="w">New width</param>
	/// <param name="h">New height</param>
	void update_size(int w, int h);

	/// <summary>
	/// Get the target that the renderer is currently drawing to.
	/// </summary>
	/// <returns>Current target</returns>
	Target* get_target() { return m_currentTarget; }

	/// <summary>
	/// Set the target for the renderer to draw into
	/// </summary>
	/// <param name="tg">Target pointer</param>
	void set_target(Target* tg);

	// todo: change this to smart pointer or dont forget to cleanup
	
	/// <summary>
	/// Set the camera for the renderer to use when drawing
	/// </summary>
	/// <param name="cam"></param>
	inline void set_camera(Camera* cam) { m_currentCamera = cam; };

	/// <summary>
	/// Draw given target
	/// </summary>
	/// <param name="tg"></param>
	void draw_target(Target* tg);

	/// <summary>
	/// Draw a texture
	/// </summary>
	/// <param name="tex">Texture pointer</param>
	/// <param name="pos">Position to draw at</param>
	/// <param name="opacity">Opacity to draw with</param>
	/// <param name="flip">Should the texture be flipped?</param>
	void draw_tex(Texture* tex, Vec2 pos, float opacity = 1.0f, bool flip = false);

	/// <summary>
	/// Draw a texture with custom shader and size
	/// </summary>
	/// <param name="tex">Texture pointer</param>
	/// <param name="pos">Position to draw at</param>
	/// <param name="size">Texture size</param>
	/// <param name="custom_shader">Shader pointer</param>
	/// <param name="opacity">Opacity to draw with</param>
	/// <param name="flip">Should the texture be flipped?</param>
	void draw_tex_s(Texture* tex, Vec2 pos, Vec2 size, Shader* custom_shader = nullptr, float opacity = 1.0f, bool flip = false);

	/// <summary>
	/// Draw a subtexture of a texture
	/// </summary>
	/// <param name="subTex">Subtexture pointer</param>
	/// <param name="pos">Position to draw at</param>
	/// <param name="scale">Scale to draw with</param>
	/// <param name="opacity">Opacity to draw with</param>
	/// <param name="flip">Should the texture be flipped?</param>
	void draw_subtex(Subtexture* subTex, Vec2 pos, float opacity = 1.0f, float scale = 1.0f, bool flip = false);

	/// <summary>
	/// Draw a rectangle
	/// </summary>
	/// <param name="pos">Position to draw at</param>
	/// <param name="size">Size of the box</param>
	/// <param name="color">Color of the box</param>
	/// <param name="fill">Fill the box?</param>
	void draw_box(Vec2 pos, Vec2 size, Vec3 color = Vec3(0,0,0), bool fill = false);
	
	/// <summary>
	/// Draw a circle
	/// </summary>
	/// <param name="pos">Position to draw the circle at</param>
	/// <param name="radius">Radius of the circle</param>
	/// <param name="color">Color of the circle</param>
	void draw_circle(Vec2 pos, float radius, Vec3 color = Vec3(0, 0, 0));

	void draw_quad();

	/// <summary>
	/// Draw text with given font
	/// </summary>
	/// <param name="text">Text to draw</param>
	/// <param name="font">Font to use</param>
	/// <param name="pos">Text position</param>
	/// <param name="scale">Text scale</param>
	/// <param name="opacity">Text opacity</param>
	void draw_text(String text, Font* font, Vec2 pos, float scale = 1.0f, float opacity = 1.0f);

	/// <summary>
	/// Draw filled box with a custom shader
	/// </summary>
	/// <param name="pos">Position to draw at</param>
	/// <param name="size">Size of the box</param>
	/// <param name="color">Color of the box</param>
	/// <param name="shd">Custom shader to use</param>
	void draw_box_s(Vec2 pos, Vec2 size, Vec3 color, Shader* shd);

	void ui_draw_box(Vec2 pos, Vec2 size, Vec3 color = Vec3(0, 0, 0), bool fill = false);

	// used for particles

	/// <summary>
	/// Draw VAO (Vertex Array Object) directly
	/// </summary>
	/// <param name="m_particle_vao">VAO Object</param>
	/// <param name="m_ptl_shader">Pointer to the shader to use</param>
	/// <param name="model">model matrix to use</param>
	void draw_vao(GLuint m_particle_vao, Shader* m_ptl_shader, glm::mat4 model);
};