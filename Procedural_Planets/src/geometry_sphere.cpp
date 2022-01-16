#include "geometry_sphere.h"

GeometrySphere::GeometrySphere(float radius, int sub) : radius{radius}, subdivision{sub}
{
	vertexCountPerRow = (unsigned int)pow(2, sub) + 1;
	vertexCountPerFace = vertexCountPerRow * vertexCountPerRow;

	buildSphere();
	buildInterleavedVertices();
}

void GeometrySphere::buildSphere()
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
            addNormal(x, y, z);
            addTexCoord(s, t);

            // add indices
            if (i < (vertexCountPerRow - 1) && j < (vertexCountPerRow - 1))
            {
                addIndices(k1, k2, k1 + 1);
                addIndices(k1 + 1, k2, k2 + 1);
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
        addTexCoord(texCoords[j], texCoords[j + 1]);
        addNormal(-normals[i], normals[i + 1], -normals[i + 2]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build +Y face by swapping x=>y, y=>-z, z=>-x
    startIndex = vertices.size() / 3;
    for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i + 2], vertices[i], -vertices[i + 1]);
        addTexCoord(texCoords[j], texCoords[j + 1]);
        addNormal(-normals[i + 2], normals[i], -normals[i + 1]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build -Y face by swapping x=>-y, y=>z, z=>-x
    startIndex = vertices.size() / 3;
    for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i + 2], -vertices[i], vertices[i + 1]);
        addTexCoord(texCoords[j], texCoords[j + 1]);
        addNormal(-normals[i + 2], -normals[i], normals[i + 1]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build +Z face by swapping x=>z, z=>-x
    startIndex = vertices.size() / 3;
    for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(-vertices[i + 2], vertices[i + 1], vertices[i]);
        addTexCoord(texCoords[j], texCoords[j + 1]);
        addNormal(-normals[i + 2], normals[i + 1], normals[i]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build -Z face by swapping x=>-z, z=>x
    startIndex = vertices.size() / 3;
    for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
    {
        addVertex(vertices[i + 2], vertices[i + 1], -vertices[i]);
        addTexCoord(texCoords[j], texCoords[j + 1]);
        addNormal(normals[i + 2], normals[i + 1], -normals[i]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }
}

std::vector<float> GeometrySphere::getUnitPositiveX(unsigned int pointsPerRow)
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

float GeometrySphere::computeScaleForLength(const float v[3], float length)
{
    return length / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void GeometrySphere::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void GeometrySphere::addVertices(const float v1[3], const float v2[3], const float v3[3], const float v4[3])
{
    vertices.insert(vertices.end(), v1, v1 + 3);
    vertices.insert(vertices.end(), v2, v2 + 3);
    vertices.insert(vertices.end(), v3, v3 + 3);
    vertices.insert(vertices.end(), v4, v4 + 3);
}

void GeometrySphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

void GeometrySphere::addNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

void GeometrySphere::addNormals(const float n1[3], const float n2[3], const float n3[3], const float n4[3])
{
    normals.insert(normals.end(), n1, n1 + 3);
    normals.insert(normals.end(), n2, n2 + 3);
    normals.insert(normals.end(), n3, n3 + 3);
    normals.insert(normals.end(), n4, n4 + 3);
}

void GeometrySphere::addTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}

void GeometrySphere::addTexCoords(const float t1[2], const float t2[2], const float t3[2], const float t4[2])
{
    texCoords.insert(texCoords.end(), t1, t1 + 2);
    texCoords.insert(texCoords.end(), t2, t2 + 2);
    texCoords.insert(texCoords.end(), t3, t3 + 2);
    texCoords.insert(texCoords.end(), t4, t4 + 2);
}

void GeometrySphere::buildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i + 1]);
        interleavedVertices.push_back(vertices[i + 2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i + 1]);
        interleavedVertices.push_back(normals[i + 2]);

        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j + 1]);
    }
}
