#pragma once
#include "DirectXGameCore.h"
#include "Component.h"
#include "Transform.h"
#include <vector>

class Entity
{
public:
	const static int MAX_NUM_COMPONENTS = 5;
	Entity();
	~Entity();

	void AddComponent(Component* newComponent);
	void Update();

	Transform& GetTransform() { return transform; }
private:
	Transform transform;
	Component* components[MAX_NUM_COMPONENTS];
	int numberOfComponents;

};

