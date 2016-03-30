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

	//Returns the first component of type T
	template <class T>
	T* GetComponent() {
		for (int c = 0; c < numberOfComponents; c++) {
			T* possibleComponent = dynamic_cast<T*>(components[c]);
			if (possibleComponent != nullptr) {
				return possibleComponent;
			}
		}
		return nullptr;
	}
private:
	Transform transform;
	Component* components[MAX_NUM_COMPONENTS];
	int numberOfComponents;

};

