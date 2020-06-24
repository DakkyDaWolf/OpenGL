#include "pch.h"
#include "GameComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(GameComponent)

	GameComponent::GameComponent(Game& game) :
		mGame(&game), mEnabled(true)
	{
	}

	void GameComponent::Initialize()
	{
	}

	void GameComponent::Update(const GameTime& /*gameTime*/)
	{
	}
}