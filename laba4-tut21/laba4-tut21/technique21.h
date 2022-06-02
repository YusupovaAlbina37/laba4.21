#pragma once
#include <iostream>
#include <GL/glew.h>
#include <list>
#include "camera4_21.h"

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

class Technique
{
public:
	Technique();
	~Technique();
	virtual bool Init(); //������������� ������ � ������� ���� �������
	void Enable(); //������� Enable() �� ����� ���� ��������� glUseProgram(), ��� ��� ��� ������ ���� ������� ������ ���, 
	//����� �� ������ ������� ����� ������� ������� ���������

protected:
	bool AddShader(GLenum ShaderType, const char* pShaderText);
	bool Finalize(); //���������� ��� �������� ��������
	GLint GetUniformLocation(const char* pUniformName);

private:
	GLuint m_shaderProg;
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};