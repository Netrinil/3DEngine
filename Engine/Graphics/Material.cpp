#include "Material.h"
#include "Engine.h"

namespace ds
{
	bool Material::Load(const std::string& filename, void* data)
	{
		// cast the data void* to an Engine*
		auto engine = static_cast<Engine*>(data);

		// load the json file
		rapidjson::Document document;
		bool success = ds::json::Load(filename, document);
		if (!success)
		{
			SDL_Log("Could not load material file (%s).", filename.c_str());
			return false;
		}

		// color values
		//JSON_READ(document, ambient);
		JSON_READ(document, diffuse);
		JSON_READ(document, specular);
		JSON_READ(document, shininess);
		//<read the json diffuse, specular and shininess>

			// program
		std::string shader_name;
		JSON_READ(document, shader_name);
		//<read the shader name>
		shader = engine->Get<ds::ResourceSystem>()->Get<ds::Program>(shader_name, engine);

		// textures
		std::vector<std::string> texture_names;
		JSON_READ(document, texture_names);

		GLuint units[] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5 };
		size_t i = 0;

		for (auto& name : texture_names)
		{
			auto texture = engine->Get<ds::ResourceSystem>()->Get<Texture>(name, (void*)units[i++]); //<use the resource system and get Texture>(name);
			if (texture.get()) // check for valid texture
			{
				AddTexture(texture);
			}
		}

		return true;
	}

	void Material::Set()
	{
		// set the shader (bind)
		shader->Use();
		// update shader material properties
		shader->SetUniform("material.diffuse", diffuse);
		shader->SetUniform("material.specular", specular);
		shader->SetUniform("material.shininess", shininess);
		//<set material diffuse, specular, and shininess uniforms>

		// set the textures (bind)
		// maybe try using std::for_each
		for (auto& texture : textures)
		{
			texture->Bind();
		}
	}
}