#pragma once

class Entity;
class Component;

class EntitySystem
{
public:
	EntitySystem(const int newMaxNumberOfEntsCanHold);
	~EntitySystem();

	void Update(float dt);

	Entity* AddEntity();
	Entity* EnableEntity(int index);//Enables an entity
	Entity* DisableEntity(int index);
	Entity* DisableEntity(Entity* entity);
	void AddComponentToEntity(int index, Component* addedComponent);//Adds component at index
																	//Adds Component at last active index
	void AddComponentToEntity(Component* addedComponent);
	Entity* GetEntity(int index);

	bool IsEntityIndexValid(int index);
	bool IsEntityActive(int index);
	bool CanAddEntity() { return numEnts + 1 < maxNumberOfEntsCanHold; }
	int GetNumberOfEnts() { return numEnts; }
private:
	Entity* ents;
	int maxNumberOfEntsCanHold;
	int numEnts;
	int numEnabledEnts;
};

