#include "pch.h"
#include "DepthPass.h"
#include "Mesh.h"
#include "ColorHelper.h"

using namespace glm;
using namespace std;

namespace Library
{

    RTTI_DEFINITIONS(DepthPass)

    DepthPass::DepthPass() :
        SHADER_VARIABLE_INITIALIZATION(WorldViewProjection)
    {
    }

    SHADER_VARIABLE_DEFINITION(DepthPass, WorldViewProjection)

    void DepthPass::Initialize(GLuint vertexArrayObject)
    {
        ShaderProgram::Initialize(vertexArrayObject);

        SHADER_VARIABLE_INSTANTIATE(WorldViewProjection)

        glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::Position), 4, GL_FLOAT, GL_FALSE, sizeof(VertexPosition), (void*)offsetof(VertexPositionColor, Position));
        glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::Position));
    }
}