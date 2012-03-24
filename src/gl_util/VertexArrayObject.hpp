#pragma once

#include <GL3/gl3w.h>

#include <boost/noncopyable.hpp>

namespace hw {
namespace gl {

class VertexArrayObject : boost::noncopyable
{
public:
	VertexArrayObject();
	~VertexArrayObject();
	VertexArrayObject(VertexArrayObject&& o);

	void bind() const;

	operator GLuint() const;

private:
	GLuint id;
};

inline VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &id);
}

inline VertexArrayObject::~VertexArrayObject()
{
	if (id != 0)
		glDeleteVertexArrays(1, &id);
}

inline VertexArrayObject::VertexArrayObject(VertexArrayObject&& o)
	: id(o.id)
{
	o.id = 0;
}

inline void VertexArrayObject::bind() const
{
	glBindVertexArray(id);
}

inline VertexArrayObject::operator GLuint() const
{
	return id;
}

} // namespace gl
} // namespaec hw
