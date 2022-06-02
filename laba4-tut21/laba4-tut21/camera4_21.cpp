#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtx/rotate_vector.hpp>

#include "Camera4_21.h"

const static float StepSize = 0.1f; //���
const static int MARGIN = 10; //�������

Camera::Camera(int WindowWidth, int WindowHeight)
{
	m_windowWidth = WindowWidth;
	m_windowHeight = WindowHeight;
	m_pos = vec3(0.0f, 0.0f, 0.0f);
	m_target = vec3(1.0f, 0.0f, 0.0f);
	normalize(m_target);
	m_up = vec3(0.0f, 1.0f, 0.0f);
	Init();
}

bool Camera::OnKeyboard(int Key) //������� ��� ����������� ������ � ������� ����������
{
	bool Ret = false;
	switch (Key)
	{
	case GLUT_KEY_UP:
	{
		m_pos += (m_target * StepSize);
		Ret = true;
	} break;
	case GLUT_KEY_DOWN:
	{
		m_pos -= (m_target * StepSize);
		Ret = true;
	} break;
	case GLUT_KEY_LEFT:
	{
		vec3 Left = cross(m_target, m_up);
		normalize(Left);
		Left *= StepSize;
		m_pos += Left;
		Ret = true;
	} break;
	case GLUT_KEY_RIGHT:
	{
		vec3 Right = cross(m_up, m_target);
		normalize(Right);
		Right *= StepSize;
		m_pos += Right;
		Ret = true;
	} break;
	}
	return Ret;
}

void Camera::Init() //�������, ��������������� ���������� ��������� ������
{
	vec3 HTarget(m_target.x, 0.0, m_target.z); //����������� �� �����������
	//�������� ��������� ��������� ������� ����������� �� ��������� XZ

	normalize(HTarget); //������������ ������� �������

	if (HTarget.z >= 0.0f) {
		if (HTarget.x >= 0.0f) {
			m_AngleH = 360.0f - degrees(asin(HTarget.z));
		}
		else {
			m_AngleH = 180.0f + degrees(asin(HTarget.z));
		}
	}
	else {
		if (HTarget.x >= 0.0f) {
			m_AngleH = degrees(asin(-HTarget.z));
		}
		else {
			m_AngleH = 90.0f + degrees(asin(-HTarget.z));
		}
	}

	m_AngleV = -degrees(asin(m_target.y)); //������������ ����

	m_OnUpperEdge = false; //���� ��� �������� �� �������� �� ������ ������� ������� ������
	m_OnLowerEdge = false; //���� ��� �������� �� �������� �� ������ ������ ������� ������
	m_OnLeftEdge = false; //���� ��� �������� �� �������� �� ������ ����� ������� ������
	m_OnRightEdge = false; //���� ��� �������� �� �������� �� ������ ������ ������� ������
	m_mousePos.x = m_windowWidth / 2;
	m_mousePos.y = m_windowHeight / 2;

	glutWarpPointer(m_mousePos.x, m_mousePos.y); //���������� ������ � ����� ������
}

void Camera::OnMouse(int x, int y) //������� ���-� ��� ���� ����� ��������, ��� ��������� ���� ����������
{
	//������� ������� ����� ������ ����-�� � ����������� �� ���� X � Y
	const int DeltaX = x - m_mousePos.x;
	const int DeltaY = y - m_mousePos.y;

	//���������� ����� �������� ��� ��������� ������� �������
	m_mousePos.x = x;
	m_mousePos.y = y;

	//��������� ������� �������������� � ������������ ���� �� ��� �������� � ���������
	m_AngleH += (float)DeltaX / 20.0f;
	m_AngleV += (float)DeltaY / 20.0f;

	if (DeltaX == 0)
	{
		if (x <= MARGIN) //���� �� ����-�� X ���� ������ � ������ ����
		{
			m_OnLeftEdge = true;
		}
		else if (x >= (m_windowWidth - MARGIN)) //���� �� ����-�� X ���� ������ � ������� ����
		{
			m_OnRightEdge = true;
		}
	}
	else
	{
		m_OnLeftEdge = false;
		m_OnRightEdge = false;
	}

	if (DeltaY == 0)
	{
		if (y <= MARGIN) //���� �� ����-�� Y ���� ������ � �������� ����
		{
			m_OnUpperEdge = true;
		}
		else if (y >= (m_windowHeight - MARGIN)) //���� �� ����-�� Y ���� ������ � ������� ���� 
		{
			m_OnLowerEdge = true;
		}
	}
	else
	{
		m_OnUpperEdge = false;
		m_OnLowerEdge = false;
	}

	Update(); //�������� ��� ����������� �������� ����������� � ������� �����, 
	//���������� �� ����� �������������� � ������������ �����

	//glutWarpPointer(m_mousePos.x, m_mousePos.y);
}
void Camera::OnRender() //�-�, ������� ���������� ��� �������, ����� ���� �� ��������, �� ��������� ����� ����� �� ������ ������
{
	bool ShouldUpdate = false;

	if (m_OnLeftEdge)
	{
		m_AngleH -= 0.1f;
		ShouldUpdate = true;
	}
	else if (m_OnRightEdge)
	{
		m_AngleH += 0.1f;
		ShouldUpdate = true;
	}

	if (m_OnUpperEdge)
	{
		if (m_AngleV > -90.0f)
		{
			m_AngleV -= 0.1f;
			ShouldUpdate = true;
		}
	}
	else if (m_OnLowerEdge)
	{
		if (m_AngleV < 90.0f)
		{
			m_AngleV += 0.1f;
			ShouldUpdate = true;
		}
	}

	if (ShouldUpdate)
	{
		Update();
	}
}

void Camera::Update() //������� ��������� �������� �������� ����������� � ����� �������� ��������������� � ������������� �����
{
	const vec3 Vaxis(0.0f, 1.0f, 0.0f);//������������ ���

	vec3 View(1.0f, 0.0f, 0.0f); //������ ������
	View = rotate(View, radians(m_AngleH), Vaxis);
	normalize(View);

	vec3 Haxis = cross(Vaxis, View); //����� �������������� ���
	normalize(Haxis);

	View = rotate(View, radians(m_AngleV), Haxis);
	normalize(View);

	m_target = normalize(View);
	m_up = normalize(cross(m_target, Haxis));
}