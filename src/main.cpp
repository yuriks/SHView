#include "math/Quaternion.hpp"
#include "math/misc.hpp"

#include <iostream>
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
		bool running = true;

		glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		glEnable(GL_CULL_FACE);
		//glEnable(GL_DEPTH_CLAMP);
		glFrontFace(GL_CW);
		glEnable(GL_FRAMEBUFFER_SRGB);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
				//rot_amount = math::Quaternion(math::up, -x_mdelta * MOUSE_ROT_SPEED) * rot_amount;
				//rot_amount = math::Quaternion(math::right, -y_mdelta * MOUSE_ROT_SPEED) * rot_amount;

				last_mouse_pos[0] = cur_mouse_pos[0];
				last_mouse_pos[1] = cur_mouse_pos[1];

				elapsed_game_time += 1./60.;
			}

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
