﻿#pragma once
#include <string>

#include "GameObject.h"
#include "Scene.h"
#include "glm/vec2.hpp"
#include "Util/Logger.h"
#include "Util/SingletonTemplate.h"
#include "Util/Events/EngineEvents.h"

class SceneManager : public SingletonTemplate<SceneManager>
{
public:
	void destroyScene(const Scene* scene);
	bool changeScene(const std::string& scene);
	GameObject* createGameObject(std::string name, glm::vec2 position);
	void destroyGameObject(GameObject* gameObject) const;

	bool init();
	void update() const;
	void lateUpdate();
	void render() const;
	void deleteAllPendingObjects() const;

	void onSceneChanged(const OnSceneChangeRequested* event);
	bool isShuttingDown() const { return shuttingDown; }
	bool isLoadingScene() const { return loadingScene; }

	Scene* getScene() const { return currentScene; }
private:
	std::list<GameObject*> pendingObjects;
	void addGameObject(GameObject* gameObject);
	void addPendingObjects();
	
	Scene* currentScene;
	bool shuttingDown = false;
	bool loadingScene = false;
	friend class ImGuiHandler;
};
