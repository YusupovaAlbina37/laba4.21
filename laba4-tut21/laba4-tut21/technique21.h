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
	virtual bool Init(); //инициализация техник с помощью этой функции
	void Enable(); //Функция Enable() на самом деле выполняет glUseProgram(), так что она должна быть вызвана всякий раз, 
	//когда мы меняем технику перед вызовом функции отрисовки

protected:
	bool AddShader(GLenum ShaderType, const char* pShaderText);
	bool Finalize(); //вызывается для линковки объектов
	GLint GetUniformLocation(const char* pUniformName);

private:
	GLuint m_shaderProg;
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};