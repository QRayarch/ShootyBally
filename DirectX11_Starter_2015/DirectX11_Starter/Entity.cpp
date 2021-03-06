#include "Entity.h"



Entity::Entity()
{
	numberOfComponents = 0;
}

Entity::~Entity()
{
	for (int c = 0; c < numberOfComponents; ++c) {
		if (components[c] != nullptr) {
			delete components[c];
			components[c] = nullptr;
		}
	}
}

void Entity::AddComponent(Component* newComponent)
{
	if (numberOfComponents >= MAX_NUM_COMPONENTS) return;
	components[numberOfComponents] = newComponent;
	components[numberOfComponents]->SetEntity(this);
	numberOfComponents++;
}

void Entity::Update(float dt)
{
	for (int c = 0; c < numberOfComponents; ++c) {
		components[c]->Update(dt);
	}
}
