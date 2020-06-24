namespace Library
{
	inline gsl::not_null<Game*> GameComponent::GetGame() const
	{
		return mGame;
	}

	inline void GameComponent::SetGame(Game& game)
	{
		mGame = &game;
	}

	inline bool GameComponent::Enabled() const
	{
		return mEnabled;
	}

	inline void GameComponent::SetEnabled(bool enabled)
	{
		mEnabled = enabled;
	}
}