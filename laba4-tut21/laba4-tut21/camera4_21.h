#pragma once
#include "pipeline4_21.h"

class Camera
{
public:
	Camera(int WindowWidth, int WindowHeight);
	Camera(int WindowWidth, int WindowHeight, const vec3& Pos, const vec3& Target, const vec3& Up)
	{
		m_windowWidth = WindowWidth;
		m_windowHeight = WindowHeight;
		m_pos = Pos; //������� ������
		m_target = Target; //����������� ������
		normalize(m_target);
		m_up = Up; //������� ������
		normalize(m_up);

		Init();

	}
	bool OnKeyboard(int Key);
	void OnMouse(int x, int y);
	void OnRender();

	const vec3& GetPos() const
	{
		return m_pos;
	}
	const vec3& GetTarget() const
	{
		return m_target;
	}
	const vec3& GetUp() const
	{
		return m_up;
	}

private:

	void Init();
	void Update();

	vec3 m_pos; //������ �������
	vec3 m_target; //������ �����������
	vec3 m_up; //������� ������

	int m_windowWidth; //������
	int m_windowHeight; //������

	float m_AngleH; //�������������� ����
	float m_AngleV; // ������������ ����

	bool m_OnUpperEdge; //���� ��� �������� �� �������� �� ������ ������� ������� ������
	bool m_OnLowerEdge; //���� ��� �������� �� �������� �� ������ ������ ������� ������
	bool m_OnLeftEdge; //���� ��� �������� �� �������� �� ������ ����� ������� ������
	bool m_OnRightEdge; //���� ��� �������� �� �������� �� ������ ������ ������� ������

	ivec2 m_mousePos; //������� �������

};