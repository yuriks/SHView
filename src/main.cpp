#include "math/Quaternion.hpp"
#include "math/misc.hpp"
#include "gl_util/VertexArrayObject.hpp"
#include "gl_util/Shader.hpp"
#include "gl_util/ShaderProgram.hpp"

#include <iostream>
#include <fstream>
#include "glfw_gl3.hpp"
#include <GL3/gl3w.h>

namespace hw {

void APIENTRY debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
	if ((type != GL_DEBUG_TYPE_PERFORMANCE_ARB && type != GL_DEBUG_TYPE_OTHER_ARB) || severity != GL_DEBUG_SEVERITY_LOW_ARB)
		std::cerr << message << std::endl;
	if ((type != GL_DEBUG_TYPE_PERFORMANCE_ARB && type != GL_DEBUG_TYPE_OTHER_ARB) || severity == GL_DEBUG_SEVERITY_HIGH_ARB)
		__asm int 3; // Breakpoint
}

bool init_window()
{
	if (glfwInit() != GL_TRUE)
	{
		char tmp;
		std::cerr << "Failed to initialize GLFW." << std::endl;
		std::cin >> tmp;
		return false;
	}

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (glfwOpenWindow(800, 600, 8, 8, 8, 0, 0, 0, GLFW_WINDOW) != GL_TRUE)
	{
		char tmp;
		std::cerr << "Failed to open window." << std::endl;
		std::cin >> tmp;
		return false;
	}

	glfwSwapInterval(1);

	if (gl3wInit() != 0) {
		char tmp;
		std::cerr << "Failed to initialize gl3w." << std::endl;
		std::cin >> tmp;
		return false;
	} else if (!gl3wIsSupported(3, 3)) {
		char tmp;
		std::cerr << "OpenGL 3.3 not supported." << std::endl;
		std::cin >> tmp;
		return false;
	}

	if (glDebugMessageCallbackARB) {
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
		glDebugMessageCallbackARB(debug_callback, 0);
	}

	glViewport(0, 0, 800, 600);

	glfwDisable(GLFW_MOUSE_CURSOR);

	return true;
}

int main(int argc, char *argv[])
{
	if (!init_window())
		return 1;

	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		//glEnable(GL_CULL_FACE);
		//glEnable(GL_DEPTH_CLAMP);
		glFrontFace(GL_CW);
		glEnable(GL_FRAMEBUFFER_SRGB);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		gl::VertexArrayObject dummy_vao;
		dummy_vao.bind();

		gl::ShaderProgram shader_prog;

		{
			gl::Shader vert_shader(GL_VERTEX_SHADER);
			gl::Shader frag_shader(GL_FRAGMENT_SHADER);

			{
				std::ifstream f("data/shaders/fullscreen_triangle.vert");
				vert_shader.setSource(f);
			}
			{
				std::ifstream f("data/shaders/sh_skybox.frag");
				frag_shader.setSource(f);
			}

			vert_shader.compile();
			frag_shader.compile();

			vert_shader.printInfoLog(std::cout);
			frag_shader.printInfoLog(std::cout);

			shader_prog.attachShader(vert_shader);
			shader_prog.attachShader(frag_shader);

			shader_prog.link();
			shader_prog.printInfoLog(std::cout);
		}

		shader_prog.use();

		GLint uloc_Rotation = shader_prog.getUniformLocation("u_Rotation");

		bool running = true;
		double elapsed_game_time = 0.;
		double elapsed_real_time = 0.;
		double last_frame_time;

		Quaternion rot_amount(vec3_up, pi);

		int last_mouse_pos[2];
		glfwGetMousePos(&last_mouse_pos[0], &last_mouse_pos[1]);

		while (running)
		{
			double frame_start = glfwGetTime();

			int i;
			for (i = 0; elapsed_game_time < elapsed_real_time && i < 5; ++i)
			{
				static const float ROT_SPEED = 0.02f;
				static const float MOUSE_ROT_SPEED = 0.01f;

				if (glfwGetKey('A')) rot_amount = Quaternion(vec3_up, ROT_SPEED) * rot_amount;
				if (glfwGetKey('D')) rot_amount = Quaternion(vec3_up, -ROT_SPEED) * rot_amount;

				if (glfwGetKey('W')) rot_amount = Quaternion(vec3_right, ROT_SPEED) * rot_amount;
				if (glfwGetKey('S')) rot_amount = Quaternion(vec3_right, -ROT_SPEED) * rot_amount;

				if (glfwGetKey('Q')) rot_amount = Quaternion(vec3_forward, ROT_SPEED) * rot_amount;
				if (glfwGetKey('E')) rot_amount = Quaternion(vec3_forward, -ROT_SPEED) * rot_amount;

				int cur_mouse_pos[2];
				glfwGetMousePos(&cur_mouse_pos[0], &cur_mouse_pos[1]);

				float x_mdelta = float(cur_mouse_pos[0] - last_mouse_pos[0]);
				float y_mdelta = float(cur_mouse_pos[1] - last_mouse_pos[1]);
				rot_amount = Quaternion(vec3_up,    -x_mdelta * MOUSE_ROT_SPEED) * rot_amount;
				rot_amount = Quaternion(vec3_right, -y_mdelta * MOUSE_ROT_SPEED) * rot_amount;

				last_mouse_pos[0] = cur_mouse_pos[0];
				last_mouse_pos[1] = cur_mouse_pos[1];

				elapsed_game_time += 1./60.;
			}

			mat3x4 rot_mat = matrixFromQuaternion(rot_amount);
			const float* m_data = rot_mat.data();

			float tmp_mat3[3*3];
			tmp_mat3[0] = m_data[0+0];
			tmp_mat3[1] = m_data[0+1];
			tmp_mat3[2] = m_data[0+2];
			tmp_mat3[3] = m_data[4+0];
			tmp_mat3[4] = m_data[4+1];
			tmp_mat3[5] = m_data[4+2];
			tmp_mat3[6] = m_data[8+0];
			tmp_mat3[7] = m_data[8+1];
			tmp_mat3[8] = m_data[8+2];

			glUniformMatrix3fv(uloc_Rotation, 1, GL_FALSE, tmp_mat3);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers();

			double tmp = elapsed_real_time + (glfwGetTime() - frame_start);
			if (elapsed_game_time > tmp)
				glfwSleep(elapsed_game_time - tmp - 0.01);

			last_frame_time = glfwGetTime() - frame_start;
			elapsed_real_time += last_frame_time;
			if (i == 5)
				elapsed_game_time = elapsed_real_time;

			running = glfwGetWindowParam(GLFW_OPENED) != 0 && glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS;
		}
	}

	glfwTerminate();

	return 0;
}

} // namespace hw

int main(int argc, char *argv[])
{
	hw::main(argc, argv);
}
