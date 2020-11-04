#pragma once
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include "libs/nvImage/nvImage.h"


class texture {
public:
	static GLuint loadPNG(const char* name)
	{
		// Texture loading object
		nv::Image img;

		static GLuint myTextureID;

		// Return true on success
		if (img.loadImageFromFile(name))
		{
			glGenTextures(1, &myTextureID);
			glBindTexture(GL_TEXTURE_2D, myTextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		}

		else
			MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

		return myTextureID;
	}
};