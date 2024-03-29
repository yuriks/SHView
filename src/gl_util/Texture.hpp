#pragma once

#include <GL3/gl3w.h>

#include <boost/noncopyable.hpp>

namespace hw {
namespace gl {

class Texture : boost::noncopyable
{
public:
	Texture();
	~Texture();
	Texture(Texture&& o);

	void bind(GLenum target) const;

	operator GLuint() const;

	// Provided for convenience, not automatically updated. (TODO: Probably remove.)
	int width, height;

private:
	GLuint id;
};

inline Texture::Texture()
	: width(-1), height(-1)
{
	glGenTextures(1, &id);
}

inline Texture::~Texture()
{
	if (id != 0)
		glDeleteTextures(1, &id);
}

inline Texture::Texture(Texture&& o)
	: id(o.id), width(o.width), height(o.height)
{
	o.id = 0;
}

inline void Texture::bind(GLenum target) const
{
	glBindTexture(target, id);
}

inline Texture::operator GLuint() const
{
	return id;
}

} // namespace gl
} // namespace hw
