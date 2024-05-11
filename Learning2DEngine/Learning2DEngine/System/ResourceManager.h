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
            Render::Shader LoadShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile, const std::string& name);
            Render::Shader GetShader(const std::string& name);
            bool IsShaderExist(const std::string& name);
            void DestroyShader(const std::string& name);
            Render::Texture2D LoadTexture(const char* file, bool alpha, const std::string& name);
            Render::Texture2D GetTexture(const std::string& name);
            bool IsTextureExist(const std::string& name);
            void DestroyTexture2D(const std::string& name);
            void Clear();
        };
	}
}