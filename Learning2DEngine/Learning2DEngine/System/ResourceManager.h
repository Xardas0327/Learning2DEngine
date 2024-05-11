#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "Singleton.h"
#include "../Render/Shader.h"
#include "../Render/Texture2D.h"

namespace Learning2DEngine
{
	namespace System
	{
        class ResourceManager : public virtual Singleton<ResourceManager>
        {
            friend class Singleton<ResourceManager>;
        private:
            std::map<std::string, Render::Shader>    shaders;
            std::map<std::string, Render::Texture2D> textures;

            ResourceManager();
            Render::Shader LoadShaderFromFile(const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr);
            std::string LoadShaderFile(const char* file);
            Render::Texture2D LoadTextureFromFile(const char* file, bool alpha);
        public:
            Render::Shader LoadShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile, std::string name);
            Render::Shader GetShader(std::string name);
            bool IsShaderExist(std::string name);
            void DestroyShader(std::string name);
            Render::Texture2D LoadTexture(const char* file, bool alpha, std::string name);
            Render::Texture2D GetTexture(std::string name);
            bool IsTextureExist(std::string name);
            void DestroyTexture2D(std::string name);
            void Clear();
        };
	}
}