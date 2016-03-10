#pragma once

class Entity;
class Component;

class EntitySystem
{
public:
	EntitySystem(const int newMaxNumberOfEntsCanHold);
	~EntitySystem();

	void Update();

	Entity* AddEntity();
	Entity* EnableEntity(int index);//Enables an entity
	void AddComponentToEntity(int index, Component* addedComponent);//Adds component at index
																	//Adds Component at last active index
	void AddComponentToEntity(Component* addedComponent);
	Entity* GetEntity(int index);

	bool IsEntityIndexValid(int index);
	bool IsEntityActive(int index);
private:
	Entity* ents;
	int maxNumberOfEntsCanHold;
	int numEnts;
	int numEnabledEnts;
};

