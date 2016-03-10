#include "Entity.h"



Entity::Entity()
{
	numberOfComponents = 0;
}

Entity::~Entity()
{
	for (int c = 0; c < numberOfComponents; ++c) {
		delete components[c];
	}
}

void Entity::AddComponent(Component* newComponent)
{
	components[numberOfComponents] = newComponent;
	components[numberOfComponents]->SetEntiy(this);
	numberOfComponents++;
}

void Entity::Update()
{
	for (int c = 0; c < numberOfComponents; ++c) {
		components[c]->Update();
	}
}
