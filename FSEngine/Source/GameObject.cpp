#include "../Header/GameObject.h"

GameObject::GameObject()
{
	components = make_unique<ComponentContainer>();

	string parameterNames[] = { "DoUpdate", "DoDraw", "DoLateUpdate", "DoLateDraw" };
	parameterCollection = make_unique<ParameterCollection<Parameters, ParametersLength>>(parameterNames);
	SetDefaultParameters();
}

void GameObject::SetDefaultParameters()
{
	Parameters defaultTrue[] = { DoUpdate, DoDraw };
	Parameters defaultFalse[] = { DoLateUpdate, DoLateDraw };

	for (const auto& parameter : defaultTrue)
		parameterCollection->SetParameter(parameter, true);

	for (const auto& parameter : defaultFalse)
		parameterCollection->SetParameter(parameter, false);
}

void GameObject::SetReferences(Systems* systems, IGameObjectContainer* gameObjectContainer)
{
	this->systems = systems;
	this->gameObjectContainer = gameObjectContainer;
}

void GameObject::Start()
{

}

void GameObject::SceneLoaded()
{

}

void GameObject::Update()
{

}

ParameterCollection<GameObject::Parameters, GameObject::ParametersLength>* GameObject::GetParameterCollection() const
{
	return parameterCollection.get();
}

const string& GameObject::GetName() const
{
	return name;
}

void GameObject::SetName(const string& name)
{
	this->name = name;
}

ComponentContainer* GameObject::GetComponentContainer() const
{
	return components.get();
}

json GameObject::GetJson() const
{
	json gameObjectJson;
	gameObjectJson["type"] = GetGameObjectType();

	json componentsJson;
	for (const auto component : components->GetAllComponents())
	{
		if (!component->GetSerializable())
			continue;

		json componentJson = component->GetJson();
		if (componentJson != nullptr)
			componentsJson[component->GetName()] = componentJson;
	}

	gameObjectJson["Components"] = componentsJson;

	gameObjectJson["ParameterCollection"] = parameterCollection->GetJson();
	return gameObjectJson;
}

void GameObject::SetFromJson(const json& j)
{
	json componentObjects = j["Components"];

	RemoveUnloadedComponents(componentObjects);

	for (const auto componentJson : componentObjects.items())
	{
		string name = componentJson.key();
		string type = componentJson.value()["type"];

		Component* component = TryGetComponentOfType(type, name);
		if (component == nullptr)
			component = AddComponent(ComponentFactory::MakeComponent(type), name);

		component->SetFromJson(componentJson.value());
	}

	parameterCollection->SetFromJson(j["ParameterCollection"]);
}

void GameObject::RemoveUnloadedComponents(const json& componentObjects)
{
	for (const Component* component : components->GetAllComponents())
	{
		if (!component->GetSerializable())
			continue;

		auto foundComponent = componentObjects.find(component->GetName());
		if (foundComponent != componentObjects.end() && ComponentMatchesType(*foundComponent, component))
			continue;

		components->RemoveComponent(component);
	}
}

bool GameObject::ComponentMatchesType(const json& foundComponent, const Component* loadedComponent) const
{
	string foundType = foundComponent["type"].get<string>();
	string loadedType = Types::ComponentTypeToString(loadedComponent->GetComponentTypeId());

	return foundType == loadedType;
}

void GameObject::SetSerializable(bool serializable)
{
	this->serializable = serializable;
}

bool GameObject::GetSerializable() const
{
	return serializable;
}

ComponentCollection<Mesh>* GameObject::GetMeshCollection() const
{
	return components->GetCollectionOfType<Mesh>(Types::Mesh);
}

ComponentCollection<Texture>* GameObject::GetTextureCollection() const
{
	return components->GetCollectionOfType<Texture>(Types::Texture);
}

Component* GameObject::TryGetComponentOfType(const string& type, const string& name)
{
	return GetComponentContainer()->GetCollectionOfType<Component>(Types::StringToComponentType(type))->TryGet(name);
}

void GameObject::ReceiveEvent(const string& key, const json& event)
{

}

GameObject::~GameObject()
{

}
