#pragma once

#include <core/types.h>

class Renderer;
class Entity;
class Collider;

class Component {
	friend class Scene;
	friend class Entity;
public:
	Component();
	virtual ~Component() = default;

	Entity* entity;
	bool enabled = true;

	virtual void init();
	virtual void update() = 0;
	virtual void render(Renderer* ren) = 0;
};

class Scene {
public:
	Scene();
	virtual ~Scene();



	Renderer* ren;

	Entity* create(String name);

	Entity* get(String name);

	Vector<Entity*> get_entities() { return m_entities; };

	void update_collider_list();

	virtual void init() = 0;
	virtual void update();
	virtual void render();
	virtual void destroy() = 0;

private:
	Vector<Entity*> m_entities;
	Vector<Collider*> m_colliders;
};



class Entity final {

	friend class Scene;

public:
	Entity() : position(640,320) {};

	String name;
	bool flip;
	Vec2 position;
	Scene* world;

	template<typename T>
	T* add(T&& comp = T());

	template<typename T>
	T* get()
	{
		T* m = nullptr;

		for (auto& comp : m_components)
		{
			if (m = dynamic_cast<T*>(comp))
				return m;
		}

		throw std::runtime_error("Component not found!");
		return nullptr;
	};

private:
	Vector<Component*> m_components;
};

template<typename T>
T* Entity::add(T&& comp /*= T()*/)
{
	T* instance = new T();

	*instance = std::forward<T>(comp);

	((Component*)instance)->entity = this;

	((Component*)instance)->init();

	// hack
	this->world->update_collider_list();

	m_components.push_back(instance);
	return instance;
}