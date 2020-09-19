#pragma once
#include "DrawableGameComponent.h"
#include <vector>
#include <memory>
#include "GL/gl3w.h"
#include "Texture.h"
#include "FullscreenTextureEffect.h"

namespace Library
{

	class Presentation final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Presentation, DrawableGameComponent)

	public:
		Presentation(Game& game);
		Presentation(const Presentation&) = default;
		Presentation& operator=(const Presentation&) = default;
		Presentation(Presentation&&) = default;
		Presentation& operator=(Presentation&&) = default;
		virtual ~Presentation() = default;

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

		void AddSlide(const std::string& filename);
		void AddSlide(std::shared_ptr<Texture> newSlide);

		void Next();
		void Previous();

	private:
		std::vector<std::shared_ptr<Texture>> mSlides;

		GLuint mTextureSampler{ 0 };
		GLuint mRectVAO{ 0 };
		GLuint mRectVertexBuffer{ 0 };
		GLuint mIndexCount{ 0 };
		GLuint mIndexBuffer{ 0 };

		FullscreenTextureEffect mShaderProgram;

		size_t mSelectedSlide{ 0 };
	};

}