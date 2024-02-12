#pragma once

#include "vector"
#include "glm/vec2.hpp"

class State;

class GameScene
{
public:
	GameScene(const char* sceneName) { m_sceneName = sceneName; }
	~GameScene() {}

	const char* GetSceneName() { return m_sceneName; }

	virtual void Enter() {}
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual void Exit() {}

private:
	const char* m_sceneName;
};