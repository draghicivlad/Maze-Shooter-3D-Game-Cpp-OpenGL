#pragma once

#include "components/simple_scene.h"
#include "utils/glm_utils.h"
#include <vector>

using namespace std;


namespace m1
{
    Mesh* CreateCube(glm::vec3 color)
    {
        Mesh* myMesh = new Mesh("generated cube");
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        vertices.push_back(VertexFormat(glm::vec3(-1, -1, -1), color, glm::vec3(0.2, 0.8, 0.6)));
        vertices.push_back(VertexFormat(glm::vec3(-1, -1, 1), color, glm::vec3(0.2, 0.8, 0.6)));
        vertices.push_back(VertexFormat(glm::vec3(1, -1, 1), color, glm::vec3(0.2, 0.8, 0.6)));
        vertices.push_back(VertexFormat(glm::vec3(1, -1, -1), color, glm::vec3(0.2, 0.8, 0.6)));
        
        vertices.push_back(VertexFormat(glm::vec3(-1, 1, -1), color, glm::vec3(0.2, 0.8, 0.6)));
        vertices.push_back(VertexFormat(glm::vec3(-1, 1, 1), color, glm::vec3(0.2, 0.8, 0.6)));
        vertices.push_back(VertexFormat(glm::vec3(1, 1, 1), color, glm::vec3(0.2, 0.8, 0.6)));
        vertices.push_back(VertexFormat(glm::vec3(1, 1, -1), color, glm::vec3(0.2, 0.8, 0.6)));

        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(1);

        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(2);

        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(4);

        indices.push_back(4);
        indices.push_back(6);
        indices.push_back(7);
        
        ///

        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(6);

        indices.push_back(6);
        indices.push_back(3);
        indices.push_back(7);

        indices.push_back(5);
        indices.push_back(0);
        indices.push_back(1);

        indices.push_back(5);
        indices.push_back(4);
        indices.push_back(0);

        ///

        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(5);

        indices.push_back(5);
        indices.push_back(2);
        indices.push_back(6);

        indices.push_back(4);
        indices.push_back(7);
        indices.push_back(3);

        indices.push_back(4);
        indices.push_back(3);
        indices.push_back(0);

        myMesh->InitFromData(vertices, indices);

        return myMesh;
    }
    
    Mesh* CreateSquare(glm::vec3 color)
    {
        Mesh* myMesh = new Mesh("generated square");
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        vertices.push_back(VertexFormat(glm::vec3(-1, -1, 0), glm::vec3(0.2, 0.8, 0.6), color));
        vertices.push_back(VertexFormat(glm::vec3(1, -1, 0), glm::vec3(0.2, 0.8, 0.6), color));
        vertices.push_back(VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.2, 0.8, 0.6), color));
        vertices.push_back(VertexFormat(glm::vec3(-1, 1, 0), glm::vec3(0.2, 0.8, 0.6), color));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(3);

        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);

        myMesh->InitFromData(vertices, indices);

        return myMesh;
    }

    Mesh* CreateWireFrameMesh(glm::vec3 color)
    {
        Mesh* myMesh = new Mesh("generated WireFrame");
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        vertices.push_back(VertexFormat(glm::vec3(-1, -1, 0), glm::vec3(0.2, 0.8, 0.6), color));
        vertices.push_back(VertexFormat(glm::vec3(1, -1, 0), glm::vec3(0.2, 0.8, 0.6), color));
        vertices.push_back(VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0.2, 0.8, 0.6), color));
        vertices.push_back(VertexFormat(glm::vec3(-1, 1, 0), glm::vec3(0.2, 0.8, 0.6), color));

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        myMesh->InitFromData(vertices, indices);
        myMesh->SetDrawMode(GL_LINE_STRIP);

        return myMesh;
    }

}

