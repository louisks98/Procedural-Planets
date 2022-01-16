#include "texture.h"
#include <glad/glad.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(std::string path)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    loadFile(path);
}

void Texture::use()
{
    glActiveTexture(GL_TEXTURE0 + (ID - 1));
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::loadFile(std::string path)
{
    int width;
    int height;
    int nbColorChannels;

    //stbi_set_flip_vertically_on_load(true);

    data = stbi_load(path.data(), &width, &height, &nbColorChannels, 0);
    if (data)
    {
        int index = path.find_first_of('.');
        auto ext = path.substr(index + 1, std::string::npos);
        if (ext == "png")
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}
