#pragma once
#include "technique21.h"
//#define MAX_POINT_LIGHTS 3

struct BaseLight 
{
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = vec3(0.0f, 0.0f, 0.0f);
		AmbientIntensity = 0.0f;
		DiffuseIntensity = 0.0f;
	}
};

struct DirectionalLight : public BaseLight 
{
	vec3 Direction;

	DirectionalLight()
	{
		Direction = vec3(0.0f, 0.0f, 0.0f);
	}
};

struct PointLight : public BaseLight 
{
	vec3 Position;

	struct
	{
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight()
	{
		Position = vec3(0.0f, 0.0f, 0.0f);
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Exp = 0.0f;
	}
};

struct SpotLight : public PointLight
{
	vec3 Direction;
	float Cutoff;

	SpotLight()
	{
		Direction = vec3(0.0f, 0.0f, 0.0f);
		Cutoff = 0.0f;
	}
};

class LightingTechnique : public Technique //представляет собой инвентарь света
{
public:

	static const unsigned int MAX_POINT_LIGHTS = 2;
	static const unsigned int MAX_SPOT_LIGHTS = 2;

	LightingTechnique();
	virtual bool Init(); //вызывает Technique::AddShader() и Techique::Finalize() для генерации программы GLSL.

	void SetWVP(const mat4* WVP);
	//void SetWorldMatrix(const mat4& WVP);
	void SetWorldMatrix(const mat4* WVP);
	void SetTextureUnit(unsigned int TextureUnit);
	void SetDirectionalLight(const DirectionalLight& Light);

	void SetEyeWorldPos(const vec3& EyeWorldPos);
	void SetMatSpecularIntensity(float Intensity);
	void SetMatSpecularPower(float Power);

	void SetPointLights(unsigned int NumLights, const PointLight* pLights);
	void SetSpotLights(unsigned int NumLights, const SpotLight* pLights);

private:
	GLuint m_WVPLocation;
	GLuint m_WorldMatrixLocation;
	GLuint m_samplerLocation;
	//GLuint m_dirLightColorLocation;
	//GLuint m_dirLightAmbientIntensityLocation;
	GLuint m_eyeWorldPosLocation; // позиция глаза
	GLuint m_matSpecularIntensityLocation; // интенсивность отражения
	GLuint m_matSpecularPowerLocation; // коэффициент материала

	GLuint m_numPointLightsLocation; 
	GLuint m_numSpotLightsLocation;


	struct {
		GLuint Color; // цвет. Цвет определяет какой канал цвета может отражен и в какой интенсивности
	//Пример: если цвет (1.0, 0.5, 0.0), то красный канал отразится полностью, зеленый наполовину, а синий потеряется целиком
		GLuint AmbientIntensity; // фоновая интенсивность
	//Фоновая интенсивность определяет насколько темный или яркий свет. Вы можете иметь полностью белый с интенсивностью 1.0, 
	//тогда объект будет хорошо освещен, или 0.1, тогда объект разглядеть можно будет только с большим трудом.
		GLuint Direction; //направление в виде 3 вектора, указываемое в мировом пространстве 
		GLuint DiffuseIntensity; //интенсивность рассеивания
	} m_dirLightLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct
		{
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_pointLightsLocation[MAX_POINT_LIGHTS];

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		GLuint Direction;
		GLuint Cutoff;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_spotLightsLocation[MAX_SPOT_LIGHTS];
};