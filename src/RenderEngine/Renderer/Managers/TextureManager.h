#pragma once

#include <map>
#include <string>

#include "../Texture.h"

/*
	Texture manager. Used to create texture and reused. Store the path to the texture as a key, 
	and the texture itself

	@author Fernando del Molino
*/
class TextureManager
{
public:
	// maybe store number of references to clean the texture at runtime when not used
	struct TextureInfo {
		Texture texture;
		int references = 0;
	};
private:
	static TextureManager* _instance;

	// maybe sabe 
	std::map<std::string, TextureInfo> _textures;

	TextureManager();

public:
	~TextureManager();

	static TextureManager& GetInstance();

	static void CleanUp();

	// return a material instance
	Texture* LoadTexture(std::string path);

	// return a material instance
	void UnloadTexture(std::string path);

};

