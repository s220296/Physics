#pragma once

#include "vector"

class GameScene;

class GameStateManager
{
public:
	GameStateManager(GameScene* initialState);
	~GameStateManager();

	void AddGameState(GameScene* state);
	void DeleteGameState(GameScene* state);

	void ChangeGameState(const char* stateName);
	void Update(float dt);
	void Draw();

protected:
	GameScene* m_currentGameState;
	std::vector<GameScene*> m_gameStates;
};

