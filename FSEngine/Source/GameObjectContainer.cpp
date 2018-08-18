#include "../Header/GameObject.h"

GameObject::GameObjectContainer::GameObjectContainer()
{
	gameObjectMapper = new GameObjectMapper();
}

GameObject* GameObject::GameObjectContainer::AddGameObject(const string& name, GameObject* gameObject)
{
	gameObjectMapper->MapGameObject(name, (int)gameObjects.size());
	gameObjects.push_back(gameObject);

	return gameObject;
}

GameObject* GameObject::GameObjectContainer::GetGameObject(const string& name) const
{
	GameObject* gameObject = TryGetGameObject(name);
	if (gameObject == nullptr)
		throw "Could not get GameObject with name: " + name;

	return gameObject;
}

GameObject* GameObject::GameObjectContainer::TryGetGameObject(const string& name) const
{
	int index = gameObjectMapper->TryGetGameObjectIndex(name);
	return TryGetGameObjectAtIndex(index);
}

GameObject* GameObject::GameObjectContainer::TryGetGameObjectAtIndex(int index) const
{
	try
	{
		return gameObjects.at(index);
	}
	catch (std::out_of_range)
	{
		return nullptr;
	}
}

vector<GameObject*> GameObject::GameObjectContainer::GetGameObjects() const
{
	return gameObjects;
}

GameObject::GameObjectContainer::~GameObjectContainer()
{
	delete gameObjectMapper;

	for (auto& gameObject : gameObjects)
		delete gameObject;
}
