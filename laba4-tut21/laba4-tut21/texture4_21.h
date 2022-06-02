#pragma once
#include <iostream>
#include "Camera4_21.h"
#include <Magick++.h>
using namespace Magick;
using namespace std;

class Texture
{
public:
	Texture(GLenum TextureTarget, const string& FileName); //конструктор

	bool Load(); //функция для проверки на ошибки

	void Bind(GLenum TextureUnit); //привязка к текстурному модулю
private:
	string m_fileName; //имя файла текстуры
	GLenum m_textureTarget; //позиция текстуры
	GLuint m_textureObj; //объект текстуры
	Image* m_pImage;
	Blob m_blob; //BLOB (большой бинарный объект) - это полезный механизм для хранения зашифрованного изображения 
	//в память так, что оно может быть использовано сторонними программами
};