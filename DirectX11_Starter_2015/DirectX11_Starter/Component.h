#pragma once

class Entity;
class Transform;

class Component
{
public:
	const static Component EMPTY;
	Component();
	virtual ~Component();

	virtual void Update(float dt);
	virtual void ParentSet();

	void SetEntity(Entity* newParentEntity);
	Entity* GetEntity() { return parentEntity; }
	bool GetHasBeenInitialized() const { return hasBeenInitialized; }
	Transform& GetTransform();
private:
	Entity* parentEntity;
	bool hasBeenInitialized;
};

