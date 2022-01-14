#include "sphere.h"
#include <glad/glad.h>

Sphere::Sphere(float radius, int division) : radius{ radius }, subdivision{division}
{
	vertexCountPerRow = (unsigned int)pow(2, division) + 1;
	vertexCountPerFace = vertexCountPerRow * vertexCountPerRow;

	buildSphere();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void Sphere::draw()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, getVertexSize(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    //glBindVertexArray(0);
    glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, 0);
}

void Sphere::buildSphere()
{
    std::vector<float> unitVertices = getUnitPositiveX(vertexCountPerRow);

    // clear memory of prev arrays
    //clearArrays();

    float x, y, z, s, t;
    int k = 0, k1, k2;

    // build +X face
    for (unsigned int i = 0; i < vertexCountPerRow; ++i)
    {
        k1 = i * vertexCountPerRow;     // index for curr row
        k2 = k1 + vertexCountPerRow;    // index for next row
        t = (float)i / (vertexCountPerRow - 1);

        for (unsigned int j = 0; j < vertexCountPerRow; ++j, k += 3, ++k1, ++k2)
        {
            x = unitVertices[k];
            y = unitVertices[k + 1];
            z = unitVertices[k + 2];
            s = (float)j / (vertexCountPerRow - 1);
            addVertex(x * radius, y * radius, z * radius);
            //addNormal(x, y, z);
            //addTexCoord(s, t);

            // add indices
            if (i < (vertexCountPerRow - 1) && j < (vertexCountPerRow - 1))
            {
                addIndices(k1, k2, k1 + 1);
                addIndices(k1 + 1, k2, k2 + 1);
                // lines: left and top
                //lineIndices.push_back(k1);  // left
                //lineIndices.push_back(k2);
                //lineIndices.push_back(k1);  // top
                //lineIndices.push_back(k1 + 1);
            }
        }
    }

    // array size and index for building next face
    unsigned int startIndex;                    // starting index for next face
    int vertexSize = (int)vertices.size();      // vertex array size of +X face
    int indexSize = (int)indices.size();        // index array size of +X face
    //int lineIndexSize = (int)lineIndices.size(); // line index size of +X face

    // build -X face by negating x and z
    startIndex = vertices.size() / 3;
    for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i], vertices[i + 1], -vertices[i + 2]);
        //addTexCoord(texCoords[j], texCoords[j + 1]);
        //addNormal(-normals[i], normals[i + 1], -normals[i + 2]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    //for (int i = 0; i < lineIndexSize; i += 4)
    //{
    //    // left and bottom lines
    //    lineIndices.push_back(startIndex + lineIndices[i]);     // left
    //    lineIndices.push_back(startIndex + lineIndices[i + 1]);
    //    lineIndices.push_back(startIndex + lineIndices[i + 1]);  // bottom
    //    lineIndices.push_back(startIndex + lineIndices[i + 1] + 1);
    //}

    // build +Y face by swapping x=>y, y=>-z, z=>-x
    startIndex = vertices.size() / 3;
    for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i + 2], vertices[i], -vertices[i + 1]);
        //addTexCoord(texCoords[j], texCoords[j + 1]);
        //addNormal(-normals[i + 2], normals[i], -normals[i + 1]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    //for (int i = 0; i < lineIndexSize; ++i)
    //{
    //    // top and left lines (same as +X)
    //    lineIndices.push_back(startIndex + lineIndices[i]);
    //}

    // build -Y face by swapping x=>-y, y=>z, z=>-x
    startIndex = vertices.size() / 3;
    for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i + 2], -vertices[i], vertices[i + 1]);
        //addTexCoord(texCoords[j], texCoords[j + 1]);
        //addNormal(-normals[i + 2], -normals[i], normals[i + 1]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    //for (int i = 0; i < lineIndexSize; i += 4)
    //{
    //    // top and right lines
    //    lineIndices.push_back(startIndex + lineIndices[i]); // top
    //    lineIndices.push_back(startIndex + lineIndices[i + 3]);
    //    lineIndices.push_back(startIndex + lineIndices[i] + 1); // right
    //    lineIndices.push_back(startIndex + lineIndices[i + 1] + 1);
    //}

    // build +Z face by swapping x=>z, z=>-x
    startIndex = vertices.size() / 3;
    for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i + 2], vertices[i + 1], vertices[i]);
        //addTexCoord(texCoords[j], texCoords[j + 1]);
        //addNormal(-normals[i + 2], normals[i + 1], normals[i]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    //for (int i = 0; i < lineIndexSize; ++i)
    //{
    //    // top and left lines (same as +X)
    //    lineIndices.push_back(startIndex + lineIndices[i]);
    //}

    // build -Z face by swapping x=>-z, z=>x
    startIndex = vertices.size() / 3;
    for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(vertices[i + 2], vertices[i + 1], -vertices[i]);
        //addTexCoord(texCoords[j], texCoords[j + 1]);
        //addNormal(normals[i + 2], normals[i + 1], -normals[i]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
    //for (int i = 0; i < lineIndexSize; i += 4)
    //{
    //    // left and bottom lines
    //    lineIndices.push_back(startIndex + lineIndices[i]);     // left
    //    lineIndices.push_back(startIndex + lineIndices[i + 1]);
    //    lineIndices.push_back(startIndex + lineIndices[i + 1]);   // bottom
    //    lineIndices.push_back(startIndex + lineIndices[i + 1] + 1);
    //}
}

std::vector<float> Sphere::getUnitPositiveX(unsigned int pointsPerRow)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along y-axis
    float a2;           // latitudinal angle
    float scale;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis
    for (unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis
        for (unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            scale = computeScaleForLength(v, 1);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);

            // DEBUG
            //std::cout << "vertex: (" << v[0] << ", " << v[1] << ", " << v[2] << "), "
            //          << sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) << std::endl;
        }
    }

    return vertices;
}

float Sphere::computeScaleForLength(const float v[3], float length)
{
    return length / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void Sphere::addVertex(float x, float y, float z)
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
}

void Sphere::addVertices(const float v1[3], const float v2[3], const float v3[3], const float v4[3])
{
	vertices.insert(vertices.end(), v1, v1 + 3);
	vertices.insert(vertices.end(), v2, v2 + 3);
	vertices.insert(vertices.end(), v3, v3 + 3);
	vertices.insert(vertices.end(), v4, v4 + 3);
}

void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}
