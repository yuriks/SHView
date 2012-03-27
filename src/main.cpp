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

static const float sh_coeffs[9*3] = {
	/*
	// Grace Cathedral
	// l = 0
	0.79f, 0.44f, 0.54f,

	// l = 1
	0.39f, 0.35f, 0.60f,
	-.34f, -.18f, -.27f,
	-.29f, -.06f, 0.01f,

	// l = 2
	-.11f, -.05f, -.12f,
	-.26f, -.22f, -.47f,
	-.16f, -.09f, -.15f,
	0.56f, 0.21f, 0.14f,
	0.21f, -.05f, -.30f
	*/

	/*
	// Eucalyptus Grove
	// l = 0
	0.38f, 0.43f, 0.45f,

	// l = 1
	0.29f, 0.36f, 0.41f,
	0.04f, 0.03f, 0.01f,
	-.10f, -.10f, -.09f,

	// l = 2
	-.06f, -.06f, -.04f,
	0.01f, -.01f, -.05f,
	-.09f, -.13f, -.15f,
	-.06f, -.05f, -.04f,
	0.02f, -.00f, -.05f,
	*/

	/*
	// Mountain
	// l = 0
	0.497747f, 0.533142f, 0.351614f,

	// l = 1
	0.184322f, 0.237415f, 0.201101f,
	0.197857f, 0.192596f, 0.158596f,
	-0.00598033f, 0.011225f, 0.00537241f,

	// l = 2
	0.00195113f, -0.00926185f, 0.00735418f,
	0.0383797f, 0.0551979f, 0.0643008f,
	0.0777444f, 0.0695796f, 0.0674626f,
	-0.0985914f, -0.0822909f, -0.0903143f,
	-0.0788599f, -0.0628599f, -0.0874032f,
	*/

	/*
	// Beach
	// l = 0
	1.94902f, 2.01515f, 1.95485f,

	// l = 1
	0.639259f, 0.638559f, 0.87111f,
	-0.186191f, -0.188515f, -0.188985f,
	-0.320778f, -0.282544f, -0.234709f,

	// l = 2
	-0.137627f, -0.108518f, -0.0696648f,
	-0.0688254f, -0.0460346f, -0.0278676f,
	-0.0174168f, -0.0219164f, -0.0360965f,
	-0.0264728f, -0.0237854f, -0.0236417f,
	0.204196f, 0.209379f, 0.216124f,
	*/


	// Sand
	// l = 0
	0.731866f, 0.563547f, 0.547556f,

	// l = 1
	-0.162351f, 0.127578f, 0.410455f,
	0.00366212f, -0.0305503f, -0.0522595f,
	-0.18162f, -0.195591f, -0.212264f,

	// l = 2
	-0.117137f, -0.148956f, -0.179564f,
	0.0577083f, 0.00285475f, -0.0309231f,
	0.0800637f, 0.0346251f, -0.00649192f,
	0.0661692f, 0.0579224f, 0.0519312f,
	0.208809f, 0.176148f, 0.129312f,


	/*
	// Hotel
	// l = 0
	0.926943f, 0.611083f, 0.490182f,

	// l = 1
	0.546959f, 0.34241f, 0.202446f,
	0.328894f, 0.287813f, 0.250144f,
	-0.0126105f, -0.0152039f, -0.0479908f,

	// l = 2
	-0.0644728f, -0.0382017f, -0.055518f,
	0.130336f, 0.116984f, 0.0688767f,
	0.249529f, 0.186009f, 0.152008f,
	0.0513811f, 0.0662891f, 0.0733768f,
	0.0567261f, 0.0724645f, 0.0954549f,
	*/

	/*
	// l = 0
	0.f, 0.f, 0.f,

	// l = 1
	0.f, 0.f, 0.f,
	0.f, 0.f, 0.f,
	0.f, 0.f, 0.f,
	
	// l = 2
	0.f, 0.f, 0.f,
	0.f, 0.f, 0.f,
	0.f, 0.f, 0.f,
	0.f, 0.f, 0.f,
	0.f, 0.f, 0.f,
	*/
};

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

		glUniform3fv(shader_prog.getUniformLocation("u_SHcoef_0p0"), 1, &sh_coeffs[0*3]);

		glUniform3fv(shader_prog.getUniformLocation("u_SHcoef_1n1"), 1, &sh_coeffs[1*3]);
		glUniform3fv(shader_prog.getUniformLocation("u_SHcoef_1p0"), 1, &sh_coeffs[2*3]);
		glUniform3fv(shader_prog.getUniformLocation("u_SHcoef_1p1"), 1, &sh_coeffs[3*3]);

		glUniform3fv(shader_prog.getUniformLocation("u_SHcoef_2n2"), 1, &sh_coeffs[4*3]);
		glUniform3fv(shader_prog.getUniformLocation("u_SHcoef_2n1"), 1, &sh_coeffs[5*3]);
		glUniform3fv(shader_prog.getUniformLocation("u_SHcoef_2p0"), 1, &sh_coeffs[6*3]);
		glUniform3fv(shader_prog.getUniformLocation("u_SHcoef_2p1"), 1, &sh_coeffs[7*3]);
		glUniform3fv(shader_prog.getUniformLocation("u_SHcoef_2p2"), 1, &sh_coeffs[8*3]);

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
