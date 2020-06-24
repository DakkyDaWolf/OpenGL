#include "pch.h"
#include "DrawableGameComponent.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(DrawableGameComponent)

	DrawableGameComponent::DrawableGameComponent(Game& game, shared_ptr<Camera> camera) :
		GameComponent(game), mCamera(move(camera))
	{
	}
}