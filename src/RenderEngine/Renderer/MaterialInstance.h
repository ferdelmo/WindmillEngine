#pragma once

#include <map>
#include <vector>
#include <string>

#include "Uniform.h"
#include "Buffer.h"
#include "Texture.h"

class Material;

struct ParameterInfo {
	virtual ~ParameterInfo() {

	}

	virtual void Fill(Buffer* buffer) {}

	virtual void Fill(Texture* tex){}
	
	virtual UniformTypes GetType() { return UniformTypes::UNIFORM;  }
};

template<typename T>
struct GenericParameter : public ParameterInfo {
	T obj;

	GenericParameter(T obj) : obj(obj) {

	}

	virtual void Fill(Buffer* buffer) override {
		std::vector<T> aux = { obj };
		
		buffer->Fill(aux);
	}

	virtual UniformTypes GetType() override { return UniformTypes::UNIFORM; }
};

struct TextureParameter : public ParameterInfo {
	std::string pathTex;

	TextureParameter(std::string pathTex) : pathTex(pathTex) {

	}

	virtual void Fill(Texture* tex) {
		tex->ChangeSourceImage(pathTex);
	}

	virtual UniformTypes GetType() override { return UniformTypes::TEXTURE; }

};


/*
	Class to represent a material instance, 
	to reuse same pipeline a shader with different parameters

	@author Fernando del Molino
*/
class MaterialInstance
{
private:

	Material* _parent;

	std::string _materialName;

	std::map<std::string, ParameterInfo*> _uniforms;

public:

	/*
		The material is supposed to be passed with the materialUtils functions
	*/
	MaterialInstance(std::string materialName);

	~MaterialInstance();

	// to acces pipeline, shaders and other stuff
	Material* GetMaterial();
	
	/*
		Funtion to change material instance atributes,

		THIS IS VERY UNSECURE, NEED TO CHECK TYPES AND BUFFER SIZES
	*/
	template<typename T>
	bool SetValue(std::string variable, T value) {
		if (_uniforms.find(variable) != _uniforms.end()) {
			// the key exists
			
			((GenericParameter<T>*)_uniforms[variable])->obj = value;

			return true;
		}
		else {
			GenericParameter<T>* aux = new GenericParameter<T>(value);
			_uniforms[variable] = aux;

			return false;
		}
	}

	bool SetTexture(std::string variable, std::string path) {
		if (_uniforms.find(variable) != _uniforms.end()) {
			// the key exists

			((TextureParameter*)_uniforms[variable])->pathTex = path;

			return true;
		}
		else {
			TextureParameter* aux = new TextureParameter(path);
			_uniforms[variable] = aux;

			return false;
		}
	}

	std::map<std::string, ParameterInfo*>& GetValues();
};

