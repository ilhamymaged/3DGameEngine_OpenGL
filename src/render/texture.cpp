#include <render/texture.hpp>
#include <stb_image/stb_image.h>

std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::textures;

std::shared_ptr<Texture> Texture::Load(const std::string& path)
{
    auto it = textures.find(path);

    if (it != textures.end())
        return it->second;   // already loaded

    auto texture = std::shared_ptr<Texture>(new Texture(path));

    textures[path] = texture;

    return texture;
}

Texture::Texture(const std::string& path)
{
    glGenTextures(1, &m_ID); 
    glBindTexture(GL_TEXTURE_2D, m_ID); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); 
    }
    else
    {
        std::cout << "Failed to load texture: " << path << "." << std::endl;
    }
    stbi_image_free(data); 
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

