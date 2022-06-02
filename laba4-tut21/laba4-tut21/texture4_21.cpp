#include "texture4_21.h"

Texture::Texture(GLenum TextureTarget, const string& FileName)
{
	m_textureTarget = TextureTarget; //������� ��������
	m_fileName = FileName; // ��� �����
	m_pImage = NULL;
}
bool Texture::Load() //����� ������� ��� ������ ����, ��������, ���� �� ����������, ��� ���� ImageMagick ������� ������ ���� ������
{
	try
	{
		m_pImage = new Image(m_fileName);//������������� �������� ������ ���� Magic::Image ��������� ��� ����� ��������
		//���� ����� ��������� �������� � ������
		m_pImage->write(&m_blob, "RGBA"); //���������� ����������� � ������ Magick::Blob ��������� ������ RGBA (�������, �������, ����� � ����� �����)
	}
	catch (Error& Error)
	{
		cout << "Error loading texture '" << m_fileName << "': " << Error.what() << endl;
		return false;
	}
	glGenTextures(1, &m_textureObj); //��� ���������� ��������� ����� �������� ������� � �������� �� � ��������� 
	//�� ������ GLuint(������ ��������).� ����� ������ ��� ����������� ������ 1 ������.

	glBindTexture(m_textureTarget, m_textureObj); //OpenGL ������ �����, � ����� �������� ������� ��������. ��� ���� ������� glBindTexture(). 
	//��� �������� OpenGL ������ ��������, ������� ��������� �� ���� �������, 
	//��������� � ����������, �� ��� ���, ���� ����� ������ ������� �� ����� �������
	//� ���������� � ��������� (������ ��������) �� ����� ��������� ������� ��������

	glTexImage2D(m_textureTarget, 0, GL_RGBA, m_pImage->columns(), m_pImage->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
	//������� ��� �������� ������� ����� ������� ��������, ��� �� ����, ���� ������ ��������
	//(�������, ������� ����������� (LOD), ���������� ������ (� �� OpenGL ������ ��������), ������, ������ �������� � ��������,�����,
	//������(���-�� �������; ������ ��������������� �������� �� BLOB), ���, ����� � ������)

	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//����� �� ��������� �������, ������� ����� ������������ ��� ���������� � �������������
	return true;

}
void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit); //��������� ������������� ����������� ������ �������, ��� �� ��� ���� �������� � ����������� �������
	glBindTexture(m_textureTarget, m_textureObj); //����������� ������ ������� � ������
}