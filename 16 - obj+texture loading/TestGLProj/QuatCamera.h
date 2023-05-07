#pragma once
#include "camera.h"
class QuatCamera	
{
public:
    QuatCamera(int WindowWidth, int WindowHeight);

	QuatCamera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up);
	
	 bool OnKeyboard(int Key);

	 glm::vec3 returnFuture(unsigned char Key);

	 // call this when an arrow key is hit

	 bool OnKeyboardchar(unsigned char);


	 void OnMouse(int x, int y); // call this when mouse moves

	glm::vec3 GetPos(); //returns current position
    
    glm::vec3 GetLookAtPoint(); // returns current look at point

    glm::vec3 GetUp();  // returns current up direction.
   
	void setFlying(bool isflying); // if true, move in the direction of looking rather than onlyu on the x-z plane.

private:
	glm::vec3 m_pos;    // current position
	glm::vec3 m_lookAtDir; // current look at direction
    glm::vec3 m_up;// current up direction
    void Init();
    void Update();

	int m_windowWidth;
    int m_windowHeight;

    float m_AngleH;// horizontal delta from mouse x
    float m_AngleV;//vertical delta from mouse y

	bool isFlying;
	glm::vec2 m_mousePos; // the last frame's mouse position
	glm::vec3 initialLookat; // the initial look at direction
};
