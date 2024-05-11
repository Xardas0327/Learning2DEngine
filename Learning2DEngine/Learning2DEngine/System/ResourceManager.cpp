#include "ResourceManager.h"

#include <sstream>
#include <fstream>
#include <exception>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


namespace Learning2DEngine
{
	namespace System
	{
        ResourceManager::ResourceManager()
            : shaders(), textures()
        {
        }

        Render::Shader ResourceManager::LoadShaderFromFile(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
        {
            std::string vertexSource;
            std::string fragmentSource;
            std::string geometrySource;
            try
            {
                vertexSource = LoadShaderFile(vertexFile);
                fragmentSource = LoadShaderFile(fragmentFile);

                if (geometryFile != nullptr)
                {
                    geometrySource = LoadShaderFile(geometryFile);
                }
            }
            catch (std::exception e)
            {
                std::string errorMessage(e.what());
                throw std::exception(("ERROR::SHADER: Failed to read shader files Message: " + errorMessage).c_str());
            }

            Render::Shader shader;
            shader.Create(vertexSource.c_str(), fragmentSource.c_str(), geometryFile != nullptr ? geometrySource.c_str() : nullptr);
            return shader;
        }

        std::string ResourceManager::LoadShaderFile(const char* file)
        {
            std::ifstream shaderFile(file);
            std::stringstream stream;
            stream << shaderFile.rdbuf();
            shaderFile.close();
            return stream.str();
        }

        Render::Texture2D ResourceManager::LoadTextureFromFile(const char* file, bool alpha)
        {
            Render::Texture2D texture;
            if (alpha)
            {
                texture.internalFormat = GL_RGBA;
                texture.imageFormat = GL_RGBA;
            }

            int width, height, nrChannels;
            unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
            texture.Create(width, height, data);
            stbi_image_free(data);
            return texture;
        }

        Render::Shader ResourceManager::LoadShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile, std::string name)
        {
            shaders[name] = LoadShaderFromFile(vertexFile, fragmentFile, geometryFile);
            return shaders[name];
        }

        Render::Shader ResourceManager::GetShader(std::string name)
        {
            return shaders[name];
        }

        bool ResourceManager::IsShaderExist(std::string name)
        {
            return shaders.count(name);
        }

        void ResourceManager::DestroyShader(std::string name)
        {
            shaders[name].Destroy();
            shaders.erase(name);
        }

        Render::Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
        {
            textures[name] = LoadTextureFromFile(file, alpha);
            return textures[name];
        }

        Render::Texture2D ResourceManager::GetTexture(std::string name)
        {
            return textures[name];
        }

        bool ResourceManager::IsTextureExist(std::string name)
        {
            return textures.count(name);
        }

        void ResourceManager::DestroyTexture2D(std::string name)
        {
            textures[name].Destroy();
            textures.erase(name);
        }

        void ResourceManager::Clear()
        {	
            for (auto shader : shaders)
            {
                shader.second.Destroy();
            }
            shaders.clear();


            for (auto texture : textures)
            {
                texture.second.Destroy();
            }
            textures.clear();
        }
	}
}