#include "ecs.h"
#include <components/collider.h>
#include <lib/imgui/imgui.h>

Entity* Scene::create(String name)
{
	auto ent = new Entity();
	ent->world = this;
	ent->name = name;

	for (auto ent : m_entities)
		if (ent->name == name)
			throw std::invalid_argument("Entity with this name already exists!");

	m_entities.push_back(ent);
	
	return ent;
}

Entity* Scene::get(String name)
{
	for (auto ent : m_entities)
		if (ent->name == name)
			return ent;

	throw std::invalid_argument("Entity not found!");
}


static Vector<Collider*> _colliders;

bool check_aabb(Collider* col, Collider* col2)
{
	return (col->position.x < col2->position.x + col2->size.x &&
		col->position.x + col->size.x > col2->position.x &&
		col->position.y < col2->position.y + col2->size.x &&
		col->position.y + col->size.y > col2->position.y);

} 

void Scene::update_collider_list()
{
	for (auto ent : m_entities)
	{
		for (auto comp : ent->m_components)
		{
			if (comp->enabled && dynamic_cast<Collider*>(comp) != nullptr && std::find(_colliders.begin(), _colliders.end(), dynamic_cast<Collider*>(comp)) == _colliders.end())
				_colliders.push_back(dynamic_cast<Collider*>(comp));
		}
	}
}

void Scene::init()
{

}

void Scene::update()
{
	if (_colliders.size() > 1)
	{
		for (int i = 0; i < _colliders.size() - 1; i++)
		{
			if (check_aabb(_colliders[i], _colliders[i + 1]))
			{
				if (_colliders[i]->on_collision_enter != nullptr)
					_colliders[i]->on_collision_enter(_colliders[i + 1]->entity);

				if (_colliders[i + 1]->on_collision_enter != nullptr)
					_colliders[i + 1]->on_collision_enter(_colliders[i]->entity);
			}
		}
	}

	for (auto ent : m_entities)
	{
		for (auto comp : ent->m_components)
		{
			if (comp->enabled)
				comp->update();
		}
	}
}

void Scene::render()
{
	for (auto ent : m_entities)
	{
		for (auto comp : ent->m_components)
		{
			if (comp->enabled)
				comp->render(ren);
		}
	}

	if (ImGui::Begin("Collider DEBUG"))
	{
		ImGui::Text("Active colliders");
		for (int i = 0; i < _colliders.size(); i++)
		{
			auto collider = _colliders[i];
			if (ImGui::CollapsingHeader(collider->entity->name.c_str()))
			{
				ImGui::LabelText("Position", "X: %f Y: %f", collider->position.x, collider->position.y);
				ImGui::LabelText("Size", "X: %f Y: %f ", collider->size.x, collider->size.y);
			}
		}
		ImGui::Text("Collision queries");


		ImGui::End();
	}
}

Scene::Scene()
{
	// collision maybe


}

Scene::~Scene()
{
	for (Entity* ent : m_entities)
	{
		for (Component* comp : ent->m_components)
		{
			delete comp;
		}
		delete ent;
	}
}

Component::Component()
{

}

void Component::init()
{

}
