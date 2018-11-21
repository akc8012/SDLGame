#pragma once
#include "GameObject.h"
#include "ClickLabel.h"

class ClickLabelManager : public GameObject
{
private:
	void CreateClickLabelForGameObject(const IGameObject* gameObject);

public:
	void Start() override;
	void SceneLoaded() override;

	string GetGameObjectType() const override;
};
