#include "Graphics.h"
#include "Math.h"

std::vector<glm::vec3> Graphics::MeshTools::Mesh::ApplyTransform()
{
    return std::vector<glm::vec3>();
}

void Graphics::MeshTools::Mesh::Draw()
{
}

Graphics::MeshTools::Shapes::Quad::Quad()
{
    Verts.push_back(glm::vec3(0, 0, 0));
    Verts.push_back(glm::vec3(0, 1, 0));
    Verts.push_back(glm::vec3(1, 1, 0));
    Verts.push_back(glm::vec3(1, 0, 0));
    Edges.push_back(glm::vec2(0, 1));
    Edges.push_back(glm::vec2(1, 2));
    Edges.push_back(glm::vec2(2, 3));
    Edges.push_back(glm::vec2(3, 4));
    Faces.push_back({ 0,1,2,3 });
}

Graphics::MeshTools::Shapes::Quad::~Quad()
{
}
