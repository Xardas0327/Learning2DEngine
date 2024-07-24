#include "ResourceManager.h"

#include <sstream>
#include <fstream>
#include <exception>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "../DebugTool/Log.h"

using namespace Learning2DEngine::Render;

namespace Learning2DEngine
{
	namespace System
	{
        ResourceManager::ResourceManager()
            : shaders(), textures()
        {
        }

        Shader ResourceManager::LoadShaderFromFile(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath)
        {
            Shader shader;

            std::string vertexSource;
            std::string fragmentSource;
            std::string geometrySource;

            //It can show all file errors.
            bool isFine = true;
            isFine = LoadShaderFile(vertexFilePath, vertexSource) && isFine;
            isFine = LoadShaderFile(fragmentFilePath, fragmentSource) && isFine;
            if (geometryFilePath != nullptr)
            {
                isFine = LoadShaderFile(geometryFilePath, geometrySource) && isFine;
            }

            if (isFine)
            {
                shader.Create(vertexSource.c_str(), fragmentSource.c_str(), geometryFilePath != nullptr ? geometrySource.c_str() : nullptr);
            }

            return shader;
        }

        bool ResourceManager::LoadShaderFile(const char* filePath, std::string& outSource)
        {
            std::ifstream shaderFile;
            shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            std::stringstream stream;
            stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                shaderFile.open(filePath);
                stream << shaderFile.rdbuf();
                shaderFile.close();
            }
            catch (std::exception e)
            {
                if (shaderFile.is_open())
                    shaderFile.close();

                LOG_ERROR(std::string("SHADER: Failed to read shader file.\n File: ") + filePath + "\n Message: " + e.what());
                return false;
            }

            outSource = stream.str();
            return true;
        }

        Shader ResourceManager::LoadShaderFromFile(
            const std::string& name, const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath)
        {
            shaders[name] = LoadShaderFromFile(vertexFilePath, fragmentFilePath, geometryFilePath);
            return shaders[name];
        }

        Shader ResourceManager::LoadShader(const char* vertexText, const char* fragmentText, const char* geometryText)
        {
            Shader shader;
            shader.Create(vertexText, fragmentText, geometryText != nullptr ? geometryText : nullptr);

            return shader;
        }

        Shader ResourceManager::LoadShader(
            const std::string& name, const char* vertexText, const char* fragmentText, const char* geometryText)
        {
            shaders[name] = LoadShader(vertexText, fragmentText, geometryText);
            return shaders[name];
        }

        Shader ResourceManager::GetShader(const std::string& name)
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

        Texture2D ResourceManager::LoadTextureFromFile(const char* filePath, const Texture2DSettings& settings)
        {
            Texture2D texture(settings);

            int width, height, nrChannels;
            unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
            texture.Create(width, height, data);
            stbi_image_free(data);
            return texture;
        }

        Texture2D ResourceManager::LoadTextureFromFile(const std::string& name, const char* filePath, const Texture2DSettings& settings)
        {
            textures[name] = LoadTextureFromFile(filePath, settings);
            return textures[name];
        }

        Texture2D ResourceManager::GetTexture(const std::string& name)
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
            for (auto& shader : shaders)
            {
                shader.second.Destroy();
            }
            shaders.clear();

            if (textures.size() > 0)
            {
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
}