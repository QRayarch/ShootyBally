#include "Component.h"
#include "Entity.h"

const Component Component::EMPTY = Component();

Component::Component()
{
	parentEntity = nullptr;
	hasBeenInitialized = false;
}

Component::~Component()
{    
	
}

void Component::Update()
{
}

void Component::ParentSet()
{
}

void Component::SetEntity(Entity* newParentEntity)
{
	parentEntity = newParentEntity;
	ParentSet();
}

Transform& Component::GetTransform() {
	return parentEntity->GetTransform();
}
