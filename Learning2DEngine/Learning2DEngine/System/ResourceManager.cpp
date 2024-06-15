#include "ResourceManager.h"

#include <sstream>
#include <fstream>
#include <exception>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Log.h"

namespace Learning2DEngine
{
	namespace System
	{
        ResourceManager::ResourceManager()
            : shaders(), textures()
        {
        }

        Render::Shader ResourceManager::LoadShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
        {
            Render::Shader shader;

            std::string vertexSource;
            std::string fragmentSource;
            std::string geometrySource;

            //It can show all file errors.
            bool isFine = true;
            isFine = LoadShaderFile(vertexFile, vertexSource) && isFine;
            isFine = LoadShaderFile(fragmentFile, fragmentSource) && isFine;
            if (geometryFile != nullptr)
            {
                isFine = LoadShaderFile(geometryFile, geometrySource) && isFine;
            }

            if (isFine)
            {
                shader.Create(vertexSource.c_str(), fragmentSource.c_str(), geometryFile != nullptr ? geometrySource.c_str() : nullptr);
            }

            return shader;
        }

        bool ResourceManager::LoadShaderFile(const char* file, std::string& outSource)
        {
            std::ifstream shaderFile;
            shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            std::stringstream stream;
            stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                shaderFile.open(file);
                stream << shaderFile.rdbuf();
                shaderFile.close();
            }
            catch (std::exception e)
            {
                if (shaderFile.is_open())
                    shaderFile.close();

                LOG_ERROR(std::string("SHADER: Failed to read shader file.\n File: ") + file + "\n Message: " + e.what());
                return false;
            }

            outSource = stream.str();
            return true;
        }

        Render::Shader ResourceManager::LoadShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile, const std::string& name)
        {
            shaders[name] = LoadShader(vertexFile, fragmentFile, geometryFile);
            return shaders[name];
        }

        Render::Shader ResourceManager::GetShader(const std::string& name)
        {
            return shaders[name];
        }

        bool ResourceManager::IsShaderExist(const std::string& name)
        {
            return shaders.count(name);
        }

        void ResourceManager::DestroyShader(const std::string& name)
        {
            shaders[name].Destroy();
            shaders.erase(name);
        }

        Render::Texture2D ResourceManager::LoadTexture(const char* file, bool alpha)
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

        Render::Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, const std::string& name)
        {
            textures[name] = LoadTexture(file, alpha);
            return textures[name];
        }

        Render::Texture2D ResourceManager::GetTexture(const std::string& name)
        {
            return textures[name];
        }

        bool ResourceManager::IsTextureExist(const std::string& name)
        {
            return textures.count(name);
        }

        void ResourceManager::DestroyTexture2D(const std::string& name)
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

            unsigned int* textureIds = new unsigned int[textures.size()];
            unsigned int index = 0;
            for (auto i = textures.begin(); i != textures.end(); ++i)
            {
                textureIds[index] = i->second.GetId();
                ++index;
            }

            glDeleteTextures(index, textureIds);
            delete[] textureIds;
            textures.clear();
        }
	}
}