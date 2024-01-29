#include "GameStateManager.h"
#include "GameScene.h"

GameStateManager::GameStateManager(GameScene* initialState)
{
	m_currentGameState = initialState;
}

GameStateManager::~GameStateManager()
{
	for (GameScene* state : m_gameStates)
	{
		delete state;
	}

	m_gameStates.clear();
}

void GameStateManager::AddGameState(GameScene* state)
{
	if(state)
		m_gameStates.push_back(state);
}

void GameStateManager::DeleteGameState(GameScene* state)
{
	if (state && state != m_currentGameState)
		m_gameStates.erase(std::find(m_gameStates.begin(), m_gameStates.end(), state));
}

void GameStateManager::ChangeGameState(const char* stateName)
{
	for (GameScene* scene : m_gameStates)
	{
		if (scene->GetSceneName() == stateName)
		{
			m_currentGameState = scene;
		}
	}
}

void GameStateManager::Update(float dt)
{
	if (m_currentGameState)
		m_currentGameState->Update(dt);
}

void GameStateManager::Draw()
{
	if (m_currentGameState)
		m_currentGameState->Draw();
}
