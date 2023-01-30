#include "Graphics.h"
#include "Math.h"

std::vector<glm::vec3> Graphics::MeshTools::Mesh::ApplyTransform()
{
    return std::vector<glm::vec3>();
}

void Graphics::MeshTools::Mesh::Draw()
{
	
    for (int i = 0; i < Faces.size(); i++)
    {
		if (Faces[i].size() == 4) {
			glBegin(GL_QUADS);
		}
		else if(Faces[i].size()==3) {
			glBegin(GL_TRIANGLES);
		}
		//draw face
		for (int j = 0; j < Faces[i].size(); j++)
		{
			//draw vert
			glVertex3f(Verts[Faces[i][j]].x, Verts[Faces[i][j]].y, Verts[Faces[i][j]].z);
		}
		glEnd();
    }
}

Graphics::MeshTools::Shapes::Quad::Quad()
{
    Verts.push_back(glm::vec3(0, 0, 0));
    Verts.push_back(glm::vec3(0, 1, 0));
    Verts.push_back(glm::vec3(1, 1, 0));
    Verts.push_back(glm::vec3(1, 0, 0));
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
	//cube faces
	Faces.push_back({ 0,1,2,3 });
	Faces.push_back({ 4,5,6,7 });
	Faces.push_back({ 0,1,5,4 });
	Faces.push_back({ 3,2,6,7 });
	Faces.push_back({ 0,3,7,4 });
	Faces.push_back({ 1,2,6,5 });
	
}

Graphics::MeshTools::Shapes::Cube::~Cube()
{
}

Graphics::MeshTools::Shapes::Triangle::Triangle()
{
	//verts
	Verts.push_back(glm::vec3(0, 0, 0));
	Verts.push_back(glm::vec3(0, 1, 0));
	Verts.push_back(glm::vec3(1, 0, 0));
	//faces
	Faces.push_back({ 0,1,2 });
}

Graphics::MeshTools::Shapes::Triangle::~Triangle()
{
}

Graphics::MeshTools::Shapes::Pyramid::Pyramid()
{
	//verts
	Verts.push_back(glm::vec3(0, 0, 0));
	Verts.push_back(glm::vec3(0, 1, 0));
	Verts.push_back(glm::vec3(1, 1, 0));
	Verts.push_back(glm::vec3(1, 0, 0));
	Verts.push_back(glm::vec3(0.5, 0.5, 1));
	//faces
	Faces.push_back({ 0,1,2,3 });
	Faces.push_back({ 0,1,4 });
	Faces.push_back({ 1,2,4 });
	Faces.push_back({ 2,3,4 });
	Faces.push_back({ 3,0,4 });
}

Graphics::MeshTools::Shapes::Pyramid::~Pyramid()
{
}

Graphics::MeshTools::Shapes::Sphere::Sphere()
{
	//generate verts on sphere
	for (int i = 0; i < 360; i += 10)
	{
		for (int j = 0; j < 180; j += 10)
		{
			Verts.push_back(glm::vec3(cos(i) * sin(j), sin(i) * sin(j), cos(j)));
		}
	}
	//generate faces
	
}

Graphics::MeshTools::Shapes::Sphere::~Sphere()
{
}
