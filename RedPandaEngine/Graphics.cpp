#include "Graphics.h"
#include "Math.h"

std::vector<Math::Vec3<float>> Graphics::MeshTools::Mesh::ApplyTransform()
{
    return std::vector<Math::Vec3<float>>();
}

void Graphics::MeshTools::Mesh::Draw()
{
}

Graphics::MeshTools::Shapes::Quad::Quad()
{
    Verts.push_back(Math::Vec3<float>(0, 0, 0));
    Verts.push_back(Math::Vec3<float>(0, 1, 0));
    Verts.push_back(Math::Vec3<float>(1, 1, 0));
    Verts.push_back(Math::Vec3<float>(1, 0, 0));
    Edges.push_back(Math::Vec2<int>(0, 1));
    Edges.push_back(Math::Vec2<int>(1, 2));
    Edges.push_back(Math::Vec2<int>(2, 3));
    Edges.push_back(Math::Vec2<int>(3, 4));
    Faces.push_back({ 0,1,2,3 });
}

Graphics::MeshTools::Shapes::Quad::~Quad()
{
}
