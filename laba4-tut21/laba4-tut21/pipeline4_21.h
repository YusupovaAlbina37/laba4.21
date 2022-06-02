#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
using namespace glm;

class Pipeline
{
public:
	Pipeline()
	{
		m_scale = vec3(1.0f, 1.0f, 1.0f);
		m_worldPos = vec3(0.0f, 0.0f, 0.0f);
		m_rotateInfo = vec3(0.0f, 0.0f, 0.0f);
	}

	void Scale(float ScaleX, float ScaleY, float ScaleZ)
	{
		m_scale.x = ScaleX;
		m_scale.y = ScaleY;
		m_scale.z = ScaleZ;
	}

	void WorldPos(float x, float y, float z)
	{
		m_worldPos.x = x;
		m_worldPos.y = y;
		m_worldPos.z = z;
	}

	void Rotate(float RotateX, float RotateY, float RotateZ)
	{
		m_rotateInfo.x = RotateX;
		m_rotateInfo.y = RotateY;
		m_rotateInfo.z = RotateZ;
	}

	void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
	{
		m_persProj.FOV = FOV;
		m_persProj.Width = Width;
		m_persProj.Height = Height;
		m_persProj.zNear = zNear;
		m_persProj.zFar = zFar;
	}

	void SetCamera(const vec3& Pos, const vec3& Target, const vec3& Up)
	{
		m_camera.Pos = Pos; //������� ������
		m_camera.Target = Target; //����������� ������
		m_camera.Up = Up; //������� ������
	}

	void InitCameraTransform(mat4& m, const vec3& Target, const vec3& Up)
	{
		vec3 N = Target;
		N = normalize(N);
		vec3 U = Up;
		U = normalize(U);
		//U = cross(U, Target);
		U = cross(U, N);
		vec3 V = cross(N, U); //��������������

		m[0][0] = U.x; m[0][1] = U.y; m[0][2] = U.z; m[0][3] = 0.0f;
		m[1][0] = V.x; m[1][1] = V.y; m[1][2] = V.z; m[1][3] = 0.0f;
		m[2][0] = N.x; m[2][1] = N.y; m[2][2] = N.z; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	const mat4* GetWVPTrans();////

	const mat4* GetWorldTrans();////

private:
	void InitScaleTransform(mat4& m) const;
	void InitRotateTransform(mat4& m) const;
	void InitTranslationTransform(float x, float y, float z, mat4& m) const;
	void InitPerspectiveProj(mat4& m) const;

	vec3 m_scale; //�������
	vec3 m_worldPos; //������� � ����
	vec3 m_rotateInfo; //��������

	//������������� ��������
	struct {
		float FOV; //���� ������
		float Width; //������
		float Height; //������
		float zNear; //������� ���������
		float zFar; //������� ���������
	} m_persProj;

	//��������� ������
	struct {
		vec3 Pos; //������� ������
		vec3 Target; //����������� ������
		vec3 Up; //������� ������
	} m_camera;

	mat4 m_WVPtransformation;

	mat4 m_WorldTransformation;
	//mat4 m_transformation;
};