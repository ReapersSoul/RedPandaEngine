#include "Graphics.h"
#include "Math.h"

std::vector<glm::vec3> Graphics::MeshTools::Mesh::ApplyTransform()
{
    return std::vector<glm::vec3>();
}

void Graphics::MeshTools::Mesh::Draw()
{
	glBegin(GL_QUADS);
    for (int i = 0; i < Faces.size(); i++)
    {
        for (int j = 0; j < Faces[i].size(); j++)
        {
            glm::vec2 edge = Edges[Faces[i][j]];
			glm::vec3 v1 = Verts[edge.x];
			glm::vec3 v2 = Verts[edge.y];
			glVertex3d(v1.x, v1.y,v1.z);
			glVertex3d(v2.x, v2.y, v2.z);
        }
    }
    glEnd();
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
    Edges.push_back(glm::vec2(3, 0));
    Faces.push_back({ 0,1,2,3 });
}

Graphics::MeshTools::Shapes::Quad::~Quad()
{
}

Graphics::MeshTools::Shapes::Cube::Cube()
{
    //cube verts
	Verts.push_back(glm::vec3(0, 0, 0));
	Verts.push_back(glm::vec3(0, 1, 0));
	Verts.push_back(glm::vec3(1, 1, 0));
	Verts.push_back(glm::vec3(1, 0, 0));
	Verts.push_back(glm::vec3(0, 0, 1));
	Verts.push_back(glm::vec3(0, 1, 1));
	Verts.push_back(glm::vec3(1, 1, 1));
	Verts.push_back(glm::vec3(1, 0, 1));
	//cube edges
	Edges.push_back(glm::vec2(0, 1));
	Edges.push_back(glm::vec2(1, 2));
	Edges.push_back(glm::vec2(2, 3));
	Edges.push_back(glm::vec2(3, 0));
	Edges.push_back(glm::vec2(4, 5));
	Edges.push_back(glm::vec2(5, 6));
	Edges.push_back(glm::vec2(6, 7));
	Edges.push_back(glm::vec2(7, 4));
	Edges.push_back(glm::vec2(0, 4));
	Edges.push_back(glm::vec2(1, 5));
	Edges.push_back(glm::vec2(2, 6));
	Edges.push_back(glm::vec2(3, 7));
	//cube faces
	Faces.push_back({ 0,1,2,3 });
	Faces.push_back({ 4,5,6,7 });
	Faces.push_back({ 0,1,5,4 });
	Faces.push_back({ 1,2,6,5 });
	Faces.push_back({ 2,3,7,6 });
	Faces.push_back({ 3,0,4,7 });
	
}

Graphics::MeshTools::Shapes::Cube::~Cube()
{
}
