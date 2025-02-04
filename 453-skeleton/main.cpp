#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Window.h"
#include "AssetPath.h"

void sTriangle(std::vector<float> A, std::vector<float> B, std::vector<float> C, int iteration, CPU_Geometry& cpuGeom);


// EXAMPLE CALLBACKS
class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(ShaderProgram& shader) : shader(shader) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		
		if (key == GLFW_KEY_R && action == GLFW_PRESS) { // On a positive edge press (when FIRST clicked)
			shader.recompile();
		}
	}


	// Other callbacks we implemented that you may use
	//virtual void mouseButtonCallback(int button, int action, int mods) {}
	//virtual void cursorPosCallback(double xpos, double ypos) {}
	//virtual void scrollCallback(double xoffset, double yoffset) {}
	virtual void windowSizeCallback(int width, int height) { CallbackInterface::windowSizeCallback(width, height);/*Should be called*/ }

private:
	ShaderProgram& shader;
};


class IterationCallback : public CallbackInterface {

public:
	IterationCallback(int& iteration) : iteration(iteration) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			iteration++;
			std::cout << "Iteration num: " << iteration << std::endl;
		}
	}
private:
	int& iteration;

};
// END EXAMPLES

int main() {

	Log::debug("Starting main");

	// WINDOW
	glfwInit();//MUST call this first to set up environment (There is a terminate pair after the loop)
	Window window(800, 800, "CPSC 453 Assignment 1: Fractals"); // Can set callbacks at construction if desired

	GLDebug::enable(); // ON Submission you may comments this out to avoid unnecessary prints to the console

	// SHADERS
	ShaderProgram shader(
		AssetPath::Instance()->Get("shaders/basic.vert"), 
		AssetPath::Instance()->Get("shaders/basic.frag")
	); // Render pipeline we will use (You can use more than one!)

	// CALLBACKS
	int iteration = 0;
	std::shared_ptr<IterationCallback> callback_ptr = std::make_shared<IterationCallback>(iteration); // Class To capture input events
	//std::shared_ptr<MyCallbacks2> callback2_ptr = std::make_shared<MyCallbacks2>();
	window.setCallbacks(callback_ptr); // Can also update callbacks to new ones as needed (create more than one instance)

	// GEOMETRY
	CPU_Geometry cpuGeom; // Just a collection of vectors
	GPU_Geometry gpuGeom; // Wrapper managing VAO and VBOs, in a TIGHTLY packed format
	//https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices#Attribute_sizes

	// vertices (initial triangle)
	std::vector<float> A = { -0.5f, -float(sqrt(3)) / 4 };
	std::vector<float> B = { 0.5f, -float(sqrt(3)) / 4 };
	std::vector<float> C = { 0.f, float(sqrt(3)) / 4 };

	cpuGeom.verts.push_back(glm::vec3(A.at(0), A.at(1), 0.f)); // Lower Left
	cpuGeom.verts.push_back(glm::vec3(B.at(0), B.at(1), 0.f)); // Lower Right
	cpuGeom.verts.push_back(glm::vec3(C.at(0), C.at(1), 0.f)); // Upper

	// colours (these should be in linear space)
	cpuGeom.cols.push_back(glm::vec3(0.4f, 0.4f, 1.f));
	cpuGeom.cols.push_back(glm::vec3(0.4f, 0.4f, 1.f));
	cpuGeom.cols.push_back(glm::vec3(0.4f, 0.4f, 1.f));

	gpuGeom.setVerts(cpuGeom.verts); // Upload vertex position geometry to VBO
	gpuGeom.setCols(cpuGeom.cols); // Upload vertex colour attribute to VBO

	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents(); // Propagate events to the callback class

		shader.use(); // Use "this" shader to render
		gpuGeom.bind(); // USe "this" VAO (Geometry) on render call
		cpuGeom.verts.clear();
		cpuGeom.cols.clear();
		

		sTriangle(A, B, C, iteration, cpuGeom);
		gpuGeom.setVerts(cpuGeom.verts); // Upload vertex position geometry to VBO
		gpuGeom.setCols(cpuGeom.cols); // Upload vertex colour attribute to VBO

		glEnable(GL_FRAMEBUFFER_SRGB); // Expect Colour to be encoded in sRGB standard (as opposed to RGB) 
		// https://www.viewsonic.com/library/creative-work/srgb-vs-adobe-rgb-which-one-to-use/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear render screen (all zero) and depth (all max depth)
		glDrawArrays(GL_TRIANGLES, 0, cpuGeom.verts.size()); // Render Triangle primatives, starting at index 0 (first) with a total of 3 elements (in this case 1 triangle)
		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui (if used)

		window.swapBuffers(); //Swap the buffers while displaying the previous 	
	}

	glfwTerminate(); // Clean up GLFW
	return 0;
}

void sTriangle(std::vector<float> A, std::vector<float> B, std::vector<float> C, int iteration, CPU_Geometry& cpuGeom) {
	std::vector<float> D, E, F;

	if (iteration > 0) {
		D = { (0.5f * A.at(0)) + (0.5f * C.at(0)), (0.5f * A.at(1)) + (0.5f * C.at(1)) };
		E = { (0.5f * C.at(0)) + (0.5f * B.at(0)), (0.5f * C.at(1)) + (0.5f * B.at(1)) };
		F = { (0.5f * B.at(0)) + (0.5f * A.at(0)), (0.5f * B.at(1)) + (0.5f * A.at(1)) };

		

		sTriangle(D, E, C, iteration - 1, cpuGeom);
		sTriangle(A, F, D, iteration - 1, cpuGeom);
		sTriangle(F, B, E, iteration - 1, cpuGeom);

	}
	else {
		cpuGeom.verts.push_back(glm::vec3(A.at(0), A.at(1), 0.f)); // Lower Left
		cpuGeom.verts.push_back(glm::vec3(B.at(0), B.at(1), 0.f)); // Lower Right
		cpuGeom.verts.push_back(glm::vec3(C.at(0), C.at(1), 0.f)); // Upper

		cpuGeom.cols.push_back(glm::vec3(0.f, 0.f, 1.f));
		cpuGeom.cols.push_back(glm::vec3(0.f, 0.f, 1.f));
		cpuGeom.cols.push_back(glm::vec3(0.f, 0.f, 1.f));
		

	}


}

// push to gpu in batch