#include "Component.h"
#include "Entity.h"

const Component Component::EMPTY = Component();

Component::Component()
{
	parrentEntity = nullptr;
	hasBeenInialized = false;
}

Component::~Component(){    
	
}

void Component::Update()
{
}

void Component::SetEntiy(Entity* newParrentEntity)
{
	parrentEntity = newParrentEntity;
}

Transform& Component::GetTransform() {
	return parrentEntity->GetTransform();
}
