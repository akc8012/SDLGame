#pragma once
#include "IUpdatable.h"
#include "Scene.h"
#include "ClickLabel.h"

class ClickLabelManager : public IUpdatable, public IEventListener
{
private:
	const static string LabelSuffix;

	Scene* scene = nullptr;
	Systems* systems = nullptr;

	ClickLabel* activeClickLabel = nullptr;
	vector<ClickLabel*> clickLabels;

	void RemoveAllClickLabels();

	void CreateClickLabelForGameObject(IGameObject* gameObject);
	bool ShouldCreateClickLabel(const IGameObject* gameObject) const;

	ClickLabel* GetCursorIntersectingClickLabel() const;

	void RemoveClickLabel(const string& gameObjectName);

public:
	ClickLabelManager(Scene* scene, Systems* systems);
	~ClickLabelManager();

	static bool GameObjectNameIsClickLabel(const string& gameObjectName);

	void InitializeClickLabels();

	void Update() override;
	void ReceiveEvent(const string& key, const json& event) override;
	
	IGameObject* GetActiveGameObject() const;
};
