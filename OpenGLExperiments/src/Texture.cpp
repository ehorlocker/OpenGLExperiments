#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_FilePath(path), m_LocalBuffer(nullptr),
	m_Height(0), m_Width(0), m_BPP(0)
{
	// flip vertically since origin in stbi =/= origin in OpenGL
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	// generate and bind so we can set parameters, generate the texture, etc
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// set minification filter to linear (for when we scale down textures)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	// set the magnification filter to linear (for when we scale up textures)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	// horizontal wrapping
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	// vertical wrapping
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	/**
	* glTexImage2D(target, level, internalFormat, width, height, border,
				format, type, const GLvoid * data);
	* This creates a 2D texture image with some arguments. There
	* are a few that are important here. internalFormat "specifies the
	* number of color components in the texture" vs format which "specifies
	* the format of pixel data" which is an important distinction. It's 
	* important to note that the last three arguments establish how the 
	* texture is stored in memory. GL_RGBA8 is RGBA with 8 bits for each.
	* https://docs.gl/gl4/glTexImage2D
	*/
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGBA8, m_Width,
		m_Height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE,
		m_LocalBuffer));

	// unbind the texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer) {
		// free the image
		stbi_image_free(m_LocalBuffer);
	}
}

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_RendererID))
}

void Texture::Bind(unsigned int slot) const {
	/**
	* When we make a texture active, we need to assign it a slot.
	* This is the slot that will be used by the fragment shader to
	* draw it later on. The video establishes that computers generally
	* have 32 slots, phones have 8, etc.
	*/
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}