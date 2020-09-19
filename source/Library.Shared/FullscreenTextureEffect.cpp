#include "pch.h"
#include "FullscreenTextureEffect.h"
#include "VertexDeclarations.h"

using namespace glm;
using namespace std;

namespace Library
{
    RTTI_DEFINITIONS(FullscreenTextureEffect)

        void FullscreenTextureEffect::Initialize(GLuint vertexArrayObject)
    {
        ShaderProgram::Initialize(vertexArrayObject);

        glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position));
        glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));

        glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TextureCoordinate), 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates));
        glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TextureCoordinate));
    }
}