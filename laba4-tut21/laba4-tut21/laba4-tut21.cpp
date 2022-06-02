#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <Magick++.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "pipeline4_21.h"
#include "camera4_21.h"
#include "texture4_21.h"
#include "lighting_technique21.h"
#include "glut_backend21.h"
#include "util21.h"
#include "callbacks21.h"
#include "technique21.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

using namespace glm;

struct Vertex
{
	vec3 m_pos;
	vec2 m_tex;
	vec3 m_normal;

	Vertex() {}

	Vertex(vec3 pos, vec2 tex)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = vec3(0.0f, 0.0f, 0.0f);
	}
};

class Main : public ICallbacks
{
public:
	Main()
	{
		m_pGameCamera = NULL;
		m_pTexture = NULL;
		m_pEffect = NULL;
		m_scale = 0.0f;
		m_directionalLight.Color = vec3(1.0f, 1.0f, 1.0f); //установка цвета 
		m_directionalLight.AmbientIntensity = 0.0f; //установка инсенсивности фонового освещения

		m_directionalLight.DiffuseIntensity = 0.0f;//установка интесивности рассеивания света
		m_directionalLight.Direction = vec3(1.0f, 0.0, 0.0);// установка направления света в мировом пространстве
	}
	~Main()
	{
		delete m_pEffect;
		delete m_pGameCamera;
		delete m_pTexture;
	}
	bool Init() //берет на себя заботу о создании эффектов, загрузки текстуры и создание буферов вершин и индексов
	{
		vec3 Pos(-5.0f, 0.0f, -7.0f);
		vec3 Target(1.0f, 0.0f, 1.0f);
		vec3 Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		unsigned int Indices[] = { 0, 2, 1,
								   0, 3, 2 };

		CreateIndexBuffer(Indices, sizeof(Indices));
		CreateVertexBuffer(Indices, ARRAY_SIZE_IN_ELEMENTS(Indices));

		m_pEffect = new LightingTechnique();

		if (!m_pEffect->Init())
		{
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pEffect->Enable();

		m_pEffect->SetTextureUnit(0);

		m_pTexture = new Texture(GL_TEXTURE_2D, "C://test.png"); //создаем объект Текстуры и загружаем его

		if (!m_pTexture->Load()) //вызываем функцию для того чтобы проверить на ошибки
		{
			return false;
		}

		return true;
	}
	void Run() //вызывает GLUTBackendRun() и передает себя в качестве параметра
	{
		GLUTBackendRun(this);
	}
	virtual void RenderSceneCB()
	{
		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT);

		m_scale += 0.01f;

		SpotLight sl[2];
		sl[0].DiffuseIntensity = 15.0f;
		sl[0].Color = vec3(1.0f, 1.0f, 0.7f);
		sl[0].Position = vec3(-0.0f, -1.9f, -0.0f);
		sl[0].Direction = vec3(sinf(m_scale), 0.0f, cosf(m_scale));
		sl[0].Attenuation.Linear = 0.1f;
		sl[0].Cutoff = 20.0f;

		sl[1].DiffuseIntensity = 5.0f;
		sl[1].Color = vec3(0.0f, 1.0f, 1.0f);
		sl[1].Position = m_pGameCamera->GetPos();
		sl[1].Direction = m_pGameCamera->GetTarget();
		sl[1].Attenuation.Linear = 0.1f;
		sl[1].Cutoff = 10.0f;

		m_pEffect->SetSpotLights(2, sl);

		//PointLight pl[3];
		//pl[0].DiffuseIntensity = 0.5f; //интесивность рассеивания света
		//pl[0].Color = vec3(1.0f, 0.0f, 0.0f); //точечный источник света красного цвета
		////pl[0].Position = vec3(sinf(m_scale) * 10, 1.0f, cosf(m_scale) * 10);
		//pl[0].Position = vec3(9.0f, -1.0f, 9.0f); //позиция точечного источника света
		//pl[0].Attenuation.Linear = 0.1f; //затухание света

		//pl[1].DiffuseIntensity = 0.5f;
		//pl[1].Color = vec3(0.0f, 1.0f, 0.0f); //точечный источник света зелёного цвета
		////pl[1].Position = vec3(sinf(m_scale + 2.1f) * 10, 1.0f, cosf(m_scale + 2.1f) * 10);
		//pl[1].Position = vec3(-7.0f, -1.0f, 2.0f);
		//pl[1].Attenuation.Linear = 0.1f;

		//pl[2].DiffuseIntensity = 0.5f;
		//pl[2].Color = vec3(0.0f, 0.0f, 1.0f); //точечный источник света синего цвета
		////pl[2].Position = vec3(sinf(m_scale + 4.2f) * 10, 1.0f, cosf(m_scale + 4.2f) * 10);
		//pl[2].Position = vec3(-2.0f, -1.0f, 7.0f);
		//pl[2].Attenuation.Linear = 0.1f;

		//m_pEffect->SetPointLights(3, pl);



		Pipeline p;
		//p.Rotate(0.0f, Scale, 0.0f);
		p.WorldPos(0.0f, 0.0f, 3.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

		m_pEffect->SetWVP(p.GetWVPTrans());

		const mat4* WorldTransformation = p.GetWorldTrans();
		m_pEffect->SetWorldMatrix(WorldTransformation);
		m_pEffect->SetDirectionalLight(m_directionalLight);

		m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
		m_pEffect->SetMatSpecularIntensity(1.0f);
		m_pEffect->SetMatSpecularPower(32);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		m_pTexture->Bind(GL_TEXTURE0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glutSwapBuffers();
	}
	virtual void IdleCB()
	{
		RenderSceneCB();
	}
	virtual void SpecialKeyboardCB(int Key, int x, int y)
	{
		m_pGameCamera->OnKeyboard(Key);
	}
	virtual void KeyboardCB(unsigned char Key, int x, int y)
	{
		switch (Key)
		{
		case 'q':
			exit(0);
		case 'a': //увеличение интенсивности фонового освещения
			m_directionalLight.AmbientIntensity += 0.05f;
			break;
		case 's': //уменьшение интеснивности фонового освещения
			m_directionalLight.AmbientIntensity -= 0.05f;
			break;

		case 'z': //увеличение интенсивности рассеивания света
			m_directionalLight.DiffuseIntensity += 0.05f;
			break;
		case 'x': //уменьшение интеснивности рассеивания света
			m_directionalLight.DiffuseIntensity -= 0.05f;
			break;
		}
	}
	virtual void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}

private:

	//функция принимает массив индексов, получает вершины треугольников, полагаясь на них, и вычисляет нормали
	void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount, Vertex* pVertices, unsigned int VertexCount)
	{
		for (unsigned int i = 0; i < IndexCount; i += 3) //набираем нормали для каждой тройки вершин
		{
			unsigned int Index0 = pIndices[i];
			unsigned int Index1 = pIndices[i + 1];
			unsigned int Index2 = pIndices[i + 2];
			vec3 v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
			vec3 v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
			vec3 Normal = cross(v1, v2);
			Normal = normalize(Normal); //Перед добавлением нормаль в массив, ее нормируем. Причина в том, что 
			//результат векторного произведения не обязательно единичный вектор
			pVertices[Index0].m_normal += Normal;
			pVertices[Index1].m_normal += Normal;
			pVertices[Index2].m_normal += Normal;
		}

		for (unsigned int i = 0; i < VertexCount; i++) //cканируем массив вершин напрямую (больше нам не нужны индексы) и нормируем нормаль каждой вершины
		{
			pVertices[i].m_normal = normalize(pVertices[i].m_normal);
		}
	}

	void CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
	{
		Vertex Vertices[4] = {
		Vertex(vec3(-10.0f, -2.0f, -10.0f), vec2(0.0f, 0.0f)),
		Vertex(vec3(10.0f, -2.0f, -10.0f), vec2(1.0f, 0.0f)),
		Vertex(vec3(10.0f, -2.0f, 10.0f),  vec2(1.0f, 1.0f)),
		Vertex(vec3(-10.0f, -2.0f, 10.0f),   vec2(0.0f, 1.0f)) };

		unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);

		CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	}

	void CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes)
	{
		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
	}

	GLuint m_VBO;
	GLuint m_IBO;
	LightingTechnique* m_pEffect;
	Texture* m_pTexture;
	Camera* m_pGameCamera;
	float m_scale;
	DirectionalLight m_directionalLight;
};

int main(int argc, char **argv)
{
	GLUTBackendInit(argc, argv);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "Tutorial 21"))
	{
		return 1;
	}

	Main* pApp = new Main();

	if (!pApp->Init())
	{
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}