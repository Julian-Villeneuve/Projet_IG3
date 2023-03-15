#ifndef BONE_H
#define BONE_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <mesh.h>

#include <string>
#include <vector>
using namespace std;

class Bone {
    public:
    glm::mat4 transformation;
    vector<Vertex> affectedVertices; // list of vertices influenced by this bone

    Bone(glm::mat4 transformation) {
        this->transformation = transformation;
    }

    // add a vertex influenced by this bone
    void addAffectedVertex(Vertex v) {
        affectedVertices.push_back(v);
    }

    // compute the weights for each vertex influenced by this bone
    void computeVertexWeights() {
        for (Vertex &v : affectedVertices) {
            // compute the distance between the vertex and the bone
            float distance = glm::distance(glm::vec3(transformation[3]), v.Position);

            // set the weight to 1 if the vertex is too far from the bone
            if (distance > 1.0f) {
                v.m_Weights[0] = 1.0f;
                v.m_BoneIDs[0] = -1;
                v.m_Weights[1] = 0.0f;
                v.m_BoneIDs[1] = 0;
                v.m_Weights[2] = 0.0f;
                v.m_BoneIDs[2] = 0;
                v.m_Weights[3] = 0.0f;
                v.m_BoneIDs[3] = 0;
            }
            // set the weight to 0 if the vertex is too close to the bone
            else if (distance < 0.001f) {
                v.m_Weights[0] = 1.0f;
                v.m_BoneIDs[0] = 0;
                v.m_Weights[1] = 0.0f;
                v.m_BoneIDs[1] = 0;
                v.m_Weights[2] = 0.0f;
                v.m_BoneIDs[2] = 0;
                v.m_Weights[3] = 0.0f;
                v.m_BoneIDs[3] = 0;
            }
            // compute the weight using a linear interpolation between 1 and 0
            else {
                float weight = 1.0f - (distance - 0.001f) / (1.0f - 0.001f);
                v.m_Weights[0] = weight;
                v.m_BoneIDs[0] = 0;
                v.m_Weights[1] = 0.0f;
                v.m_BoneIDs[1] = 0;
                v.m_Weights[2] = 0.0f;
                v.m_BoneIDs[2] = 0;
                v.m_Weights[3] = 0.0f;
                v.m_BoneIDs[3] = 0;
            }
        }
    }

    // animate the bone by a given angle
    void animate(float angle) {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        transformation = transformation * rotationMatrix;
    }
};
#endif
