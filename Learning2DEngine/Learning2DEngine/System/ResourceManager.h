#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "Singleton.h"
#include "../Render/Shader.h"
#include "../Render/Texture2D.h"
#include "../Render/Texture2DSettings.h"

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
            /// <summary>
            /// Return false, if the std::ifstream or the std::stringstream thrown exception by std::ifstream::failbit or std::ifstream::badbit.
            /// Otherwise, it returns true.
            /// </summary>
            /// <param name="file"></param>
            /// <param name="outSource"></param>
            /// <returns></returns>
            bool LoadShaderFile(const char* file, std::string& outSource);
        public:
            /// <summary>
            /// It won't be saved, that's why you can't get it with GetShader and you have to destroy it manually
            /// </summary>
            /// <param name="file"></param>
            /// <param name="alpha"></param>
            /// <returns></returns>
            Render::Shader LoadShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr);
            Render::Shader LoadShader(const char* vertexFile, const char* fragmentFile, const char* geometryFile, const std::string& name);
            Render::Shader GetShader(const std::string& name);
            bool IsShaderExist(const std::string& name);
            void DestroyShader(const std::string& name);
            /// <summary>
            /// It won't be saved, that's why you can't get it with GetTexture and you have to destroy it manually
            /// </summary>
            /// <param name="file"></param>
            /// <param name="alpha"></param>
            /// <returns></returns>
            Render::Texture2D LoadTexture(const char* file, const Render::Texture2DSettings& settings);
            Render::Texture2D LoadTexture(const char* file, const Render::Texture2DSettings& settings, const std::string& name);
            Render::Texture2D GetTexture(const std::string& name);
            bool IsTextureExist(const std::string& name);
            void DestroyTexture2D(const std::string& name);
            void Clear();
        };
	}
}