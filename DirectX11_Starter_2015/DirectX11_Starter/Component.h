#pragma once

class Entity;
class Transform;

class Component
{
public:
	const static Component EMPTY;
	Component();
	~Component();

	virtual void Update();

	void SetEntity(Entity* newParentEntity);
	Entity* GetEntity() { return parentEntity; }
	bool GetHasBeenInitialized() const { return hasBeenInitialized; }
	Transform& GetTransform();
private:
	Entity* parentEntity;
	bool hasBeenInitialized;
};

