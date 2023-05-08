#include "QuatCamera.h"

const static float STEP_SCALE = 0.5f;
const static int MARGIN = 10;

QuatCamera::QuatCamera(int WindowWidth, int WindowHeight)
{
    m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos          = glm::vec3(0.0f, 0.0f, 0.0f);
    m_lookAtDir       = glm::vec3(0.0f, 0.0f, -1.0f);
	m_lookAtDir		= glm::normalize(m_lookAtDir);
    m_up           = glm::vec3(0.0f, 1.0f, 0.0f);

    Init();
}


QuatCamera::QuatCamera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
{
    m_windowWidth  = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos = Pos;
	initialLookat = glm::normalize(Target-Pos);
  	m_lookAtDir = initialLookat;

	m_up = Up;
	m_up = glm::normalize(m_up);

    Init();
}

   glm::vec3 QuatCamera::GetPos() 
    {
        return m_pos;
    }

    glm::vec3 QuatCamera::GetLookAtPoint() 
    {
        return m_pos+m_lookAtDir;
    }

    glm::vec3 QuatCamera::GetUp() 
    {
        return m_up;
    }

void QuatCamera::Init()
{
   	glm::quat q(glm::lookAt(m_pos,m_pos+m_lookAtDir,m_up));
	glm::vec3 myeulers = glm::eularAngles(q);
	m_AngleH = myeulers.y;
	m_AngleV = myeulers.x;
	isFlying = false;
	m_mousePos.x  = glutGet(GLUT_WINDOW_WIDTH) / 2;
    m_mousePos.y  = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	Update();
    glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
}

void QuatCamera::setFlying(bool isflying)
{isFlying = isflying;}

bool QuatCamera::OnKeyboard(int Key)
{
    bool Ret = false;

    switch (Key) {

    case GLUT_KEY_UP:
        {
            if(isFlying)
				m_pos += (glm::normalize(m_lookAtDir) * STEP_SCALE);
			else
				m_pos += glm::normalize((glm::vec3(m_lookAtDir.x,0.0f,m_lookAtDir.z))) * STEP_SCALE;
            Ret = true;
        }
        break;

    case GLUT_KEY_DOWN:
        {
            if(isFlying)
				m_pos -= (glm::normalize(m_lookAtDir) * STEP_SCALE);
			else
				m_pos -= glm::normalize((glm::vec3(m_lookAtDir.x,0.0f,m_lookAtDir.z))) * STEP_SCALE;
            Ret = true;
        }
        break;

    case GLUT_KEY_LEFT:
        {
			glm::vec3 Left = -glm::cross(m_lookAtDir,m_up);
			Left = glm::normalize(Left);
            Left *= STEP_SCALE;
            m_pos += Left;
            Ret = true;
        }
        break;

    case GLUT_KEY_RIGHT:
        {
			glm::vec3 Right = -glm::cross(m_up,m_lookAtDir);
			Right = glm::normalize(Right);
            Right *= STEP_SCALE;
            m_pos += Right;
            Ret = true;
        }
        break;
    }

    return Ret;
}

bool QuatCamera::OnKeyboardchar(unsigned char Key)
{
    bool Ret = false;

    switch (Key) {

    case 'w':
    {
        if (isFlying)
            m_pos += (glm::normalize(m_lookAtDir) * STEP_SCALE);
        else
            m_pos += glm::normalize((glm::vec3(m_lookAtDir.x, 0.0f, m_lookAtDir.z))) * STEP_SCALE;
        Ret = true;
    }
    break;

    case 's':
    {
        if (isFlying)
            m_pos -= (glm::normalize(m_lookAtDir) * STEP_SCALE);
        else
            m_pos -= glm::normalize((glm::vec3(m_lookAtDir.x, 0.0f, m_lookAtDir.z))) * STEP_SCALE;
        Ret = true;
    }
    break;

    case 'a':
    {
        glm::vec3 Left = -glm::cross(m_lookAtDir, m_up);
        Left = glm::normalize(Left);
        Left *= STEP_SCALE;
        m_pos += Left;
        Ret = true;
    }
    break;

    case 'd':
    {
        glm::vec3 Right = -glm::cross(m_up, m_lookAtDir);
        Right = glm::normalize(Right);
        Right *= STEP_SCALE;
        m_pos += Right;
        Ret = true;
    }
    break;
    }

    return Ret;
}

glm::vec3 QuatCamera::returnFuture(unsigned char Key)
{

    switch (Key) {

    case 'w':
    {
        return m_pos + glm::normalize((glm::vec3(m_lookAtDir.x, 0.0f, m_lookAtDir.z))) * STEP_SCALE;
    }
    break;

    case 's':
    {
        return m_pos - glm::normalize((glm::vec3(m_lookAtDir.x, 0.0f, m_lookAtDir.z))) * STEP_SCALE;

    }
    break;

    case 'a':
    {
        glm::vec3 Left = -glm::cross(m_lookAtDir, m_up);
        Left = glm::normalize(Left);
        Left *= STEP_SCALE;
        return m_pos + Left;

    }
    break;

    case 'd':
    {
        glm::vec3 Right = -glm::cross(m_up, m_lookAtDir);
        Right = glm::normalize(Right);
        Right *= STEP_SCALE;
        return m_pos + Right;
    }
    break;
    }

    return glm::vec3(0.0, 0.0, 0.0);
}



void QuatCamera::OnMouse(int x, int y)
{
    //find the difference between the new mouse position that the last one (at the center)
    const int DeltaX = -(x - m_mousePos.x);
    const int DeltaY = y - m_mousePos.y;

	if(DeltaX !=0 || DeltaY!=0){
        //reset mouse to center of screen
		m_mousePos.x = glutGet(GLUT_WINDOW_WIDTH) / 2;
		m_mousePos.y = glutGet(GLUT_WINDOW_HEIGHT) / 2;
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
        if ((m_AngleV + (float)DeltaY / 20.0f) < 225.f && (m_AngleV + (float)DeltaY / 20.0f) > 135.f){// && (m_AngleV + (float)DeltaY / 20.0f) > 90.0f)){

        //calculate the change in the angle
            m_AngleH += (float)DeltaX / 20.0f;
            m_AngleV += (float)DeltaY / 20.0f;
            Update();
        }
    }
}


void QuatCamera::Update()
{

  
   const glm::vec3 Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    glm::vec3 View = initialLookat;//(0.0f, 0.0f, -1.0f);
	glm::quat q = glm::angleAxis(m_AngleH, Vaxis);
	glm::quat qinv = glm::inverse(q);
	View = q * View * qinv;
	View = glm::normalize(View);

    // Rotate the view vector by the vertical angle around the horizontal axis
	glm::vec3 Haxis = glm::cross(Vaxis, View);
	Haxis = glm::normalize(Haxis);
	q = glm::angleAxis(m_AngleV, Haxis);
	qinv = glm::inverse(q);
	View = q * View * qinv;
	View = glm::normalize(View);
      
    m_lookAtDir = View;
	m_lookAtDir = glm::normalize(m_lookAtDir);

	m_up = glm::cross(m_lookAtDir, Haxis);
	m_up = glm::normalize(m_up);
}