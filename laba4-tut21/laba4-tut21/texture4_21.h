#pragma once
#include <iostream>
#include "Camera4_21.h"
#include <Magick++.h>
using namespace Magick;
using namespace std;

class Texture
{
public:
	Texture(GLenum TextureTarget, const string& FileName); //�����������

	bool Load(); //������� ��� �������� �� ������

	void Bind(GLenum TextureUnit); //�������� � ����������� ������
private:
	string m_fileName; //��� ����� ��������
	GLenum m_textureTarget; //������� ��������
	GLuint m_textureObj; //������ ��������
	Image* m_pImage;
	Blob m_blob; //BLOB (������� �������� ������) - ��� �������� �������� ��� �������� �������������� ����������� 
	//� ������ ���, ��� ��� ����� ���� ������������ ���������� �����������
};