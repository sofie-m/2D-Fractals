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

	// Triangle constructor
	SierpinskiTriangle(glm::vec3 x, glm::vec3 y, glm::vec3 z, glm::vec3 newColour) {

		A = x;
		B = y;
		C = z;

		colour = newColour;

	}
};

class LevyCCurve {
public:
	// Line verticles
	glm::vec3 A;
	glm::vec3 B;

	// Colours
	glm::vec3 colourA;
	glm::vec3 colourB;

	// Line constructor for C Curve
	LevyCCurve(glm::vec3 x, glm::vec3 y, glm::vec3 colourLeft, glm::vec3 colourRight) {
		A = x;
		B = y;

		colourA = colourLeft;
		colourB = colourRight;
	}
};

class Tree {
public:

	glm::vec3 base; // base of branch/leaf
	glm::vec3 top; // tip of branch/leaf

	glm::vec3 colour;

	// Tree branch/leaf constructor
	Tree(glm::vec3 base, glm::vec3 top, glm::vec3 colour) {
		this->base = base;
		this->top = top;

		this->colour = colour;
	}
};

// Function prototypes
void sierpinskiTriangleCreate(SierpinskiTriangle triangle, int iteration, int totalIterations, CPU_Geometry& cpuGeom);
void levyCCurveCreate(LevyCCurve curve, int iteration, int totalIterations, CPU_Geometry& cpuGeom);
void treeCreate(Tree branch, int iteration, int iterationCounter, CPU_Geometry& cpuGeom);


// Callbacks
class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(int& iteration, int& sceneNumber, int& maxIterations) : iteration(iteration), sceneNumber(sceneNumber), maxIterations(maxIterations) {}

	// Increase and decrease scene and iteratios with arrow keys
	virtual void keyCallback(int key, int scancode, int action, int mods) {

		// Up arrow key increases iteration
		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			
			if (iteration < maxIterations) {
				iteration++;
			}
		}

		// Down arrow key decreases iteration
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			if (iteration > 0) {
				iteration--;
			}
		}
		// Right arrow key switches to next scene
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			if (sceneNumber < 2) {
				sceneNumber++;
			}		
		}
		// Left arrow key switches to previous scene
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			if (sceneNumber > 0) {
				sceneNumber--;
			}
		}
	}

	// Increase and decrease scene and iterations with mouse
	virtual void mouseButtonCallback(int button, int action, int mods) {

		// Left click switches to next scene
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			if (sceneNumber < 2) {
				sceneNumber++;
			}
		}
		// Right click switches to previous scene
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			if (sceneNumber > 0) {
				sceneNumber--;
			}
		}	
	}

	// Controll iteration number using scroll (mouse wheel)
	virtual void scrollCallback(double xoffset, double yoffset) {
		
		// Scroll up to increase iteration
		if (yoffset > 0 && iteration < maxIterations) {
			iteration++;
		}

		// Scroll down to decrease iteration
		if (yoffset < 0 && iteration > 0) {
			iteration--;
		}
	}

private:
	int& iteration;
	int& sceneNumber;
	int& maxIterations;

};



int main() {

	Log::debug("Starting main");

	// WINDOW
	glfwInit();//MUST call this first to set up environment (There is a terminate pair after the loop)
	Window window(800, 800, "CPSC 453 Assignment 1: Fractals"); // Can set callbacks at construction if desired

	//GLDebug::enable(); // ON Submission you may comments this out to avoid unnecessary prints to the console

	// SHADERS
	ShaderProgram shader(
		AssetPath::Instance()->Get("shaders/basic.vert"), 
		AssetPath::Instance()->Get("shaders/basic.frag")
	); // Render pipeline we will use (You can use more than one!)

	// CALLBACKS
	int iteration = 0;
	int sceneNumber = 0;
	int maxIterations;
	
	std::shared_ptr<MyCallbacks> Callback_ptr = std::make_shared<MyCallbacks>(iteration, sceneNumber, maxIterations); // Class To capture input events
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
	glm::vec3 colourInit(1.f, 0.7f, .5f );

	SierpinskiTriangle triangle(ver1, ver2, ver3, colourInit);

	// vertices (initial line of Levy C Curve)
	glm::vec3 ver4(- 0.5f, -0.3f, 0.f);
	glm::vec3 ver5( 0.5f, -0.3f, 0.f );

	glm::vec3 colourLeft( 0.f, 1.f, 0.f );
	glm::vec3 colourRight(0.f, 0.f, 1.f );
	LevyCCurve line(ver4, ver5, colourLeft, colourRight);

	// Initial tree trunk
	glm::vec3 trunkBase(0.f, -0.85f, 0.f);
	glm::vec3 trunkTop(0.f, 0.f, 0.f);

	glm::vec3 branchColour(.25f, .18f, .1f);
	Tree tree(trunkBase, trunkTop, branchColour);


	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents(); // Propagate events to the callback class

		shader.use(); // Use "this" shader to render
		gpuGeom.bind(); // USe "this" VAO (Geometry) on render call
		cpuGeom.verts.clear();
		cpuGeom.cols.clear();

		// Scene 0: Sierpinski Triangle
		if (sceneNumber == 0) {
			maxIterations = 10;
			// Prevent from generating a higher number of iterations than allowed
			if (iteration > maxIterations) {
				iteration = maxIterations;
			}

			int totalIterations = iteration;
			sierpinskiTriangleCreate(triangle, iteration, totalIterations, cpuGeom);
			gpuGeom.setVerts(cpuGeom.verts); // Upload vertex position geometry to VBO
			gpuGeom.setCols(cpuGeom.cols); // Upload vertex colour attribute to VBO

			glEnable(GL_FRAMEBUFFER_SRGB); // Expect Colour to be encoded in sRGB standard (as opposed to RGB) 
			// https://www.viewsonic.com/library/creative-work/srgb-vs-adobe-rgb-which-one-to-use/
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear render screen (all zero) and depth (all max depth)
			glDrawArrays(GL_TRIANGLES, 0, cpuGeom.verts.size()); // Render primitives
			glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui (if used)

		}

		// Scene 1: Levy C Curve
		else if (sceneNumber == 1) {
			maxIterations = 18;
			// Prevent from generating a higher number of iterations than allowed
			if (iteration > maxIterations) {
				iteration = maxIterations;
			}
			int totalIterations = iteration * 2; // TNumber of iterations in one curve
			levyCCurveCreate(line, iteration, totalIterations, cpuGeom);
			gpuGeom.setVerts(cpuGeom.verts); // Upload vertex position geometry to VBO
			gpuGeom.setCols(cpuGeom.cols); // Upload vertex colour attribute to VBO
			
			glEnable(GL_FRAMEBUFFER_SRGB); 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
			glDrawArrays(GL_LINES, 0, cpuGeom.verts.size()); 
			glDisable(GL_FRAMEBUFFER_SRGB); 
		}
		
		// Scene 3: Tree
		else if (sceneNumber == 2) {
			maxIterations = 10;
			// Prevent from generating a higher number of iterations than allowed
			if (iteration > maxIterations) {
				iteration = maxIterations;
			}
			int iterationCounter = 0;
			treeCreate(tree, iteration, iterationCounter, cpuGeom);
			gpuGeom.setVerts(cpuGeom.verts); // Upload vertex position geometry to VBO
			gpuGeom.setCols(cpuGeom.cols); // Upload vertex colour attribute to VBO

			glEnable(GL_FRAMEBUFFER_SRGB); 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
			glDrawArrays(GL_LINES, 0, cpuGeom.verts.size()); 
			glDisable(GL_FRAMEBUFFER_SRGB); 
		}		

		window.swapBuffers(); //Swap the buffers while displaying the previous 	
	}

	glfwTerminate(); // Clean up GLFW
	return 0;
}


/*
* Creates vertices and colours for Sierpinski Triangle and adds them to the vertex and colour vectors
* 
* @param triangle	Triangle from previous iteration
* @param iteration	Number of iterations to generate
* @param totalIterations	Number of iterations to be generated in total
* @param cpuGeom	Collection of vectors for geometry
* 
*/
void sierpinskiTriangleCreate(SierpinskiTriangle triangle, int iteration, int totalIterations, CPU_Geometry & cpuGeom) {
	
	if (iteration > 0) {
		// New vertices from midpoins of triangle sides
		glm::vec3 D(0.5f * (triangle.A + triangle.C));
		glm::vec3 E(0.5f * (triangle.C + triangle.B));
		glm::vec3 F(0.5f * (triangle.B + triangle.A));
		
		float increment = (static_cast<float>(iteration) / totalIterations) * 0.33f; // Colour incremented based on iterations
		glm::vec3	leftColour = { triangle.colour.x, triangle.colour.y, triangle.colour.z + increment}; // increase blue
		glm::vec3	rightColour = { triangle.colour.x, triangle.colour.y, triangle.colour.z - increment }; // decrease blue 
		glm::vec3	topColour = { triangle.colour.x, triangle.colour.y - increment, triangle.colour.z}; // decrease green
	
		// Create new trianges using new vertices and vertices of previous triangles
		SierpinskiTriangle topTriangle(D, E, triangle.C, topColour);
		SierpinskiTriangle rightTriangle(triangle.A, F, D, rightColour);
		SierpinskiTriangle leftTriangle(F, triangle.B, E, leftColour);

		sierpinskiTriangleCreate(topTriangle, iteration - 1, totalIterations, cpuGeom);
		sierpinskiTriangleCreate(leftTriangle, iteration - 1, totalIterations, cpuGeom);
		sierpinskiTriangleCreate(rightTriangle, iteration - 1, totalIterations, cpuGeom);

		
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

/*
* Creates vertices and colours for Levy C Curve and adds them to the vertex and colour vectors
*
* @param line	Line from previous iteration
* @param iteration	Number of iterations to generate
* @param totalIterations	Number of iterations to be generated in total
* @param cpuGeom	Collection of vectors for geometry
*
*/
void levyCCurveCreate(LevyCCurve line, int iteration, int totalIterations, CPU_Geometry& cpuGeom) {
	if (iteration > 0) {

		// Get size of lines
		float lengthX = line.B.x - line.A.x;
		float lengthY = line.B.y - line.A.y;
		// Add size of lines to point A
		float newX = line.A.x + (lengthX / 2) - (lengthY / 2);
		float newY = line.A.y + (lengthY / 2) + (lengthX / 2);
		glm::vec3 C(newX, newY, 0.f); // Create new vertex C 
		

		// Normalize iteration range so that each iteration has a value between 0 and 1
		float colourMidpoint = (static_cast<float>(totalIterations) - iteration) / totalIterations; // Cast to avoid improper truncate

		// Mix blue and green using colourMidpoint interpolant
		glm::vec3 colourC = glm::mix(line.colourA, line.colourB, colourMidpoint);

		// Create two new lines meeting at vertex C
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

/*
* Creates vertices and colours for Tree scene and adds them to the vertex and colour vectors
*
* @param branch		branch (or leaf) from previous iteration
* @param iteration	Number of iterations to generate
* @param iterationCounter	tracks number of iterations completed to detect when to start creating leaves
* @param cpuGeom	Collection of vectors for geometry
*
*/
void treeCreate(Tree branch, int iteration, int iterationCounter, CPU_Geometry& cpuGeom) {
	glm::vec3 colour;
	if (iteration > 0) {
		iterationCounter++;

		// Change to colour to leaf colour if past iteration 3
		if (iterationCounter > 3) {
			glm::vec3 leaves(0.1f, 0.4f, 0.f);
			colour = leaves;
		}
		else (colour = branch.colour);

		// Make new branch 1/2 the size of the previous
		float lengthX = (branch.top.x - branch.base.x)/2;
		float lengthY = (branch.top.y - branch.base.y)/2;

		glm::vec3 topTip(branch.top.x + lengthX, branch.top.y + lengthY, 0.f); // Vertice of the tip of the top branch
		glm::vec3 topConnect(branch.top); // Vertice that connects top branch to tree
		Tree topBranch(topConnect, topTip, colour); // Create top branch
		treeCreate(topBranch, iteration - 1, iterationCounter, cpuGeom);

		
		glm::vec3 midpoint((branch.base + branch.top) * .5f); // connecting point to trunk
		glm::vec dirVec(branch.top - branch.base); // direction vector

		// Make new branch 1/2 the size of the previous and rotated +25.7 degrees (left)
		glm::mat4 rotate(glm::rotate(glm::mat4(1.0f), glm::radians(25.7f), glm::vec3(0.f, 0.f, 1.f))); // rotation matrix
		glm::vec3 leftBranchTip(glm::vec3(rotate * glm::vec4(dirVec, 1.f)) * 0.5f); // left branch rotated and half the length of previous branch
		leftBranchTip += midpoint; // translate to middle of previous branch

		Tree leftBranch(midpoint, leftBranchTip, colour);
		treeCreate(leftBranch, iteration - 1, iterationCounter, cpuGeom);


		// Make new branch 1/2 the size of the previous and rotated -25.7 degrees (right)
		rotate = (glm::rotate(glm::mat4(1.0f), glm::radians(-25.7f), glm::vec3(0.f, 0.f, 1.f))); // rotation matrix
		glm::vec3 rightBranchTip(glm::vec3(rotate * glm::vec4(dirVec, 1.f)) * 0.5f); // right branch rotated and half the length of previous branch
		rightBranchTip += midpoint; // translate to midpoint of previous branch

		Tree rightBranch(midpoint, rightBranchTip, colour);
		treeCreate(rightBranch, iteration - 1, iterationCounter, cpuGeom);

	}
	

	// Add vertices to vertice vector
	cpuGeom.verts.push_back(branch.base); // Left point
	cpuGeom.verts.push_back(branch.top); // Right point

	// Add colours to colour vector
	cpuGeom.cols.push_back(glm::vec3(branch.colour));
	cpuGeom.cols.push_back(glm::vec3(branch.colour));

}