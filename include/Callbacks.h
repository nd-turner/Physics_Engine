
#pragma once

#include <cstdio>
#include <Object.h>


static GLFWcursor* handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

static void glfw_Error_Callback(int error, const char* description) {

	fprintf(stderr, "Error is \n", description);
	throw("glfw_Error");
}

extern bool isDraggable(GLFWwindow* window, Object& obj) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);  // Get window size

	double Mxpos, Mypos;
	glfwGetCursorPos(window, &Mxpos, &Mypos);  // Get mouse position in pixels

	// Convert to OpenGL coordinates (-1 to 1 range)
	float normMx = (2.0f * Mxpos) / width - 1.0f;
	float normMy = (2.0f * (height - Mypos)) / height - 1.0f;


	float rad = obj.getRad();  // Get object radius
	const float* pos = obj.getPosition();  // Get object position in OpenGL space
	float Oxpos = pos[0], Oypos = pos[1];

	// Compute Euclidean distance in OpenGL coordinates
	float distance = std::sqrt((Oxpos - normMx) * (Oxpos - normMx) +
		(Oypos - normMy) * (Oypos - normMy));

	// Check if distance is within the radius
	obj.Drag = (distance < rad);

	//glfwSetCursor(window, handCursor);
	/*std::cout << "Mouse Pos: " << normMx << ", " << normMy << "\n";
	std::cout << "Object Pos: " << Oxpos << ", " << Oypos << "\n";
	std::cout << "Distance: " << distance << " Radius: " << rad << "\n";*/


	return obj.Drag;
}

void dragObj(GLFWwindow* window, Object& obj) {
	if (obj.Drag) {
		double Mxpos, Mypos;
		glfwGetCursorPos(window, &Mxpos, &Mypos);

		// Update the object's position based on the mouse cursor
		float pos[3];
		pos[0] = Mxpos - obj.getRad() / 2.0;
		pos[1] = Mypos - obj.getRad() / 2.0;

		obj.updatePosition(pos);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	// Retrieve the object from the window
	Object* obj = static_cast<Object*>(glfwGetWindowUserPointer(window));
	if (!obj) return; // Safety check

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			const float* pos = obj->getPosition();  // Get the pointer to position data
			

			obj->Drag = true;
			
		}
		else if (action == GLFW_RELEASE) {
			obj->Drag = false;  // Stop dragging on release
		}
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	Object* obj = static_cast<Object*>(glfwGetWindowUserPointer(window));
	if (!obj || !obj->Drag) return;


}


