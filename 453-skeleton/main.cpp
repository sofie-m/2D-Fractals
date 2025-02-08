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

class SierpinskiTriangle {
public:
	// Triangle vertices
	std::vector<float> A;
	std::vector<float> B;
	std::vector<float> C;

	// Triangle colour
	std::vector<float> colour;

	SierpinskiTriangle(std::vector<float> x, std::vector<float> y, std::vector<float> z, std::vector<float> newColour) {

		// vertices (initial triangle)
		A = x;
		B = y;
		C = z;

		// Initial triangle colour
		colour = newColour;

	}
};

class LevyCCurve {
public:
	glm::vec3 A;
	glm::vec3 B;

	glm::vec3 colourA;
	glm::vec3 colourB;


	LevyCCurve(glm::vec3 x, glm::vec3 y, glm::vec3 colourLeft, glm::vec3 colourRight) {
		A = x;
		B = y;

		colourA = colourLeft;
		colourB = colourRight;
	}
};


void sierpinskiTriangleCreate(SierpinskiTriangle triangle, int iteration, bool setColour, CPU_Geometry& cpuGeom);
void levyCCurveCreate(LevyCCurve curve, int iteration, int totalIterations, CPU_Geometry& cpuGeom);


// EXAMPLE CALLBACKS
class MyCallbacks2 : public CallbackInterface {

public:
	MyCallbacks2(ShaderProgram& shader) : shader(shader) {}

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

// Increase/decrease iteration with right and left arrow keys respectively
class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(int& iteration, int& sceneNumber) : iteration(iteration), sceneNumber(sceneNumber) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			iteration++;
			std::cout << "Iteration num: " << iteration << std::endl;
		}
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			if (iteration > 0) {
				iteration--;
			}
			std::cout << "Iteration num: " << iteration << std::endl;
		}
		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			sceneNumber++;
			std::cout << "Scene num: " << sceneNumber << std::endl;
		}
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			if (sceneNumber > 0) {
				sceneNumber--;
			}
			std::cout << "Scene num: " << sceneNumber << std::endl;
		}
	}
private:
	int& iteration;
	int& sceneNumber;

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
	int sceneNumber = 0;
	std::shared_ptr<MyCallbacks> Callback_ptr = std::make_shared<MyCallbacks>(iteration, sceneNumber); // Class To capture input events
	window.setCallbacks(Callback_ptr); // Can also update callbacks to new ones as needed (create more than one instance)

	// GEOMETRY
	CPU_Geometry cpuGeom; // Just a collection of vectors
	GPU_Geometry gpuGeom; // Wrapper managing VAO and VBOs, in a TIGHTLY packed format
	//https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices#Attribute_sizes

	
	// vertices (initial triangle)
	std::vector<float> ver1 = { -0.5f, -float(sqrt(3)) / 4 };
	std::vector<float> ver2 = { 0.5f, -float(sqrt(3)) / 4 };
	std::vector<float> ver3 = { 0.f, float(sqrt(3)) / 4 };

	// Initial triangle colour
	std::vector<float> colourInit = { 0.85f, 0.40f, 0.95f };

	SierpinskiTriangle triangle(ver1, ver2, ver3, colourInit);
	bool setColour = false;

	// vertices (initial line of Levy C Curve)
	glm::vec3 ver4(- 0.5f, 0.f, 0.f);
	glm::vec3 ver5( 0.5f, 0.f, 0.f );

	glm::vec3 colourLeft( 0.f, 1.f, 0.f );
	glm::vec3 colourRight(0.f, 0.f, 1.f );
	LevyCCurve line(ver4, ver5, colourLeft, colourRight);



	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents(); // Propagate events to the callback class

		shader.use(); // Use "this" shader to render
		gpuGeom.bind(); // USe "this" VAO (Geometry) on render call
		cpuGeom.verts.clear();
		cpuGeom.cols.clear();

		if (sceneNumber == 0) {
			sierpinskiTriangleCreate(triangle, iteration, setColour, cpuGeom);
			gpuGeom.setVerts(cpuGeom.verts); // Upload vertex position geometry to VBO
			gpuGeom.setCols(cpuGeom.cols); // Upload vertex colour attribute to VBO

			glEnable(GL_FRAMEBUFFER_SRGB); // Expect Colour to be encoded in sRGB standard (as opposed to RGB) 
			// https://www.viewsonic.com/library/creative-work/srgb-vs-adobe-rgb-which-one-to-use/
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear render screen (all zero) and depth (all max depth)
			glDrawArrays(GL_TRIANGLES, 0, cpuGeom.verts.size()); // Render primitives
			glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui (if used)
		}

		else if (sceneNumber == 1) {
			int totalIterations = iteration * 2; // Total number of iterations in one curve
			levyCCurveCreate(line, iteration, totalIterations, cpuGeom);
			gpuGeom.setVerts(cpuGeom.verts); // Upload vertex position geometry to VBO
			gpuGeom.setCols(cpuGeom.cols); // Upload vertex colour attribute to VBO
			
			glEnable(GL_FRAMEBUFFER_SRGB); // Expect Colour to be encoded in sRGB standard (as opposed to RGB) 
			// https://www.viewsonic.com/library/creative-work/srgb-vs-adobe-rgb-which-one-to-use/
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear render screen (all zero) and depth (all max depth)
			glDrawArrays(GL_LINES, 0, cpuGeom.verts.size()); // Render primitives
			glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui (if used)

		}
		

		window.swapBuffers(); //Swap the buffers while displaying the previous 	
	}
	glfwTerminate(); // Clean up GLFW
	return 0;
}


void sierpinskiTriangleCreate(SierpinskiTriangle triangle, int iteration, bool setColour, CPU_Geometry & cpuGeom) {
	std::vector<float> D, E, F;
	
	if (iteration > 0) {
		D = { (0.5f * triangle.A.at(0)) + (0.5f * triangle.C.at(0)), (0.5f * triangle.A.at(1)) + (0.5f * triangle.C.at(1)) };
		E = { (0.5f * triangle.C.at(0)) + (0.5f * triangle.B.at(0)), (0.5f * triangle.C.at(1)) + (0.5f * triangle.B.at(1)) };
		F = { (0.5f * triangle.B.at(0)) + (0.5f * triangle.A.at(0)), (0.5f * triangle.B.at(1)) + (0.5f * triangle.A.at(1)) };
		
		std::vector<float> topColour; // Purple 
		std::vector<float> leftColour; // Pink 
		std::vector<float> rightColour; // Orange

		if (!setColour) {
			topColour = { 0.50f , 0.40f, 0.95f }; 
			leftColour = { 0.85f, 0.40f, 0.95f }; 
			rightColour = { 0.95f, 0.40f, 0.50f }; 
		}
		else {
			leftColour = { triangle.colour.at(0) + 0.1f, triangle.colour.at(1) - 0.1f, triangle.colour.at(2) + 0.1f }; // Increase pink
			rightColour = { triangle.colour.at(0) + 0.2f, triangle.colour.at(1) - 0.0f, triangle.colour.at(2) - 0.2f }; // Increase orange
			topColour = { triangle.colour.at(0) - 0.2f, triangle.colour.at(1) - 0.0f, triangle.colour.at(2) + 0.2f }; // Increase purple
		}

		SierpinskiTriangle topTriangle(D, E, triangle.C, topColour);
		SierpinskiTriangle rightTriangle(triangle.A, F, D, rightColour);
		SierpinskiTriangle leftTriangle(F, triangle.B, E, leftColour);

		sierpinskiTriangleCreate(topTriangle, iteration - 1, true, cpuGeom);
		sierpinskiTriangleCreate(leftTriangle, iteration - 1, true, cpuGeom);
		sierpinskiTriangleCreate(rightTriangle, iteration - 1, true, cpuGeom);

		
	}
	else {
		// Add vertices to vertice vector
		cpuGeom.verts.push_back(glm::vec3(triangle.A.at(0), triangle.A.at(1), 0.f)); // Lower Left
		cpuGeom.verts.push_back(glm::vec3(triangle.B.at(0), triangle.B.at(1), 0.f)); // Lower Right
		cpuGeom.verts.push_back(glm::vec3(triangle.C.at(0), triangle.C.at(1), 0.f)); // Upper	

		// Add colours to colour vector
		cpuGeom.cols.push_back(glm::vec3(triangle.colour.at(0), triangle.colour.at(1), triangle.colour.at(2)));
		cpuGeom.cols.push_back(glm::vec3(triangle.colour.at(0), triangle.colour.at(1), triangle.colour.at(2)));
		cpuGeom.cols.push_back(glm::vec3(triangle.colour.at(0), triangle.colour.at(1), triangle.colour.at(2)));
	}
}


void levyCCurveCreate(LevyCCurve line, int iteration, int totalIterations, CPU_Geometry& cpuGeom) {
	if (iteration > 0) {

		// Get midpoint of line
		float lengthX = line.B.x - line.A.x;
		float lengthY = line.B.y - line.A.y;
		float newX = line.A.x + (lengthX / 2) - (lengthY / 2);
		float newY = line.A.y + (lengthY / 2) + (lengthX / 2);
		glm::vec3 C(newX, newY, 0.f);

		float colourMidpoint = (static_cast<float>(totalIterations) - iteration) / totalIterations; // Cast to avoid improper truncate

		glm::vec3 colourC = glm::mix(line.colourA, line.colourB, colourMidpoint);

		LevyCCurve leftLine(line.A, C, line.colourA, colourC);
		LevyCCurve rightLine(C, line.B, colourC, line.colourB);

		levyCCurveCreate(leftLine, iteration - 1,totalIterations, cpuGeom);
		levyCCurveCreate(rightLine, iteration - 1, totalIterations, cpuGeom);

	}

	else {
		// Add vertices to vertice vector
		cpuGeom.verts.push_back(line.A); // Left point
		cpuGeom.verts.push_back(line.B); // Right point

		// Add colours to colour vector
		cpuGeom.cols.push_back(glm::vec3(line.colourA));
		cpuGeom.cols.push_back(glm::vec3(line.colourB));
	}
	
	

}