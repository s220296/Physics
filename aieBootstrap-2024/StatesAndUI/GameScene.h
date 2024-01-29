#pragma once

#include "vector"

class State;

class GameScene
{
public:
	GameScene(const char* sceneName) { m_sceneName = sceneName; }
	~GameScene() { delete m_sceneName; }

	const char* GetSceneName() { return m_sceneName; }

	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;

private:
	const char* m_sceneName;
};

