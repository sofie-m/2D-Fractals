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
	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 C;

	// Triangle colour
	glm::vec3 colour;

	SierpinskiTriangle(glm::vec3 x, glm::vec3 y, glm::vec3 z, glm::vec3 newColour) {

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


void sierpinskiTriangleCreate(SierpinskiTriangle triangle, int iteration,bool setColour, CPU_Geometry& cpuGeom);
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
	glm::vec3 ver1(- 0.75f, -float(sqrt(27)) / 8, 0.f );
	glm::vec3 ver2(0.75f, -float(sqrt(27)) / 8, 0.f);
	glm::vec3 ver3(0.f, float(sqrt(27)) / 8, 0.f);

	// Initial triangle colour
	glm::vec3 colourInit(0.5f, 0.5f, 1.f );

	SierpinskiTriangle triangle(ver1, ver2, ver3, colourInit);
	bool setColour = false;

	// vertices (initial line of Levy C Curve)
	glm::vec3 ver4(- 0.5f, -0.3f, 0.f);
	glm::vec3 ver5( 0.5f, -0.3f, 0.f );

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
	
	if (iteration > 0) {
		glm::vec3 D(0.5f * (triangle.A + triangle.C));
		glm::vec3 E(0.5f * (triangle.C + triangle.B));
		glm::vec3 F(0.5f * (triangle.B + triangle.A));
		
		glm::vec3 topColour; // Purple 
		glm::vec3 leftColour; // Pink 
		glm::vec3 rightColour; // Red

		if (!setColour) {
			topColour = triangle.colour; 
			leftColour = { 1.f, 0.5f, 1.f };
			rightColour = { 1.f, 0.5f, 0.5f }; 
		}
		else {
			leftColour = { triangle.colour.x, triangle.colour.y, triangle.colour.z + .1f}; // Increase pink
			rightColour = { triangle.colour.x, triangle.colour.y - .1f, triangle.colour.z - .1f }; // Increase red
			topColour = { triangle.colour.x - .1f, triangle.colour.y, triangle.colour.z + .1f}; // Increase purple
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
		cpuGeom.verts.push_back(glm::vec3(triangle.A)); // Lower Left
		cpuGeom.verts.push_back(glm::vec3(triangle.B)); // Lower Right
		cpuGeom.verts.push_back(glm::vec3(triangle.C)); // Upper	

		// Add colours to colour vector
		cpuGeom.cols.push_back(glm::vec3(triangle.colour));
		cpuGeom.cols.push_back(glm::vec3(triangle.colour));
		cpuGeom.cols.push_back(glm::vec3(triangle.colour));
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