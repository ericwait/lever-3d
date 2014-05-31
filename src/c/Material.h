#pragma once
#include "Renderer.h"
#include <DirectXMath.h>

class Material
{
public:
	friend class Renderer;
	virtual ~Material();

	void setWireframe(bool wireframe);

protected:
	Material(){}
	Material(Renderer* rendererIn);

	virtual void updateParams(){}
	void setShader(const std::string& shaderFilename, const std::string& shaderFunction);
	
	Renderer* renderer;

private:
	int shaderIdx;
	bool wireframe;
};


class SingleColoredMaterial : public Material
{
public:
	SingleColoredMaterial(Renderer* renderer);
	SingleColoredMaterial(Renderer* renderer, Vec<float> colorIn);
	SingleColoredMaterial(Renderer* renderer, Vec<float> colorIn, float alpha);
	~SingleColoredMaterial();

	void setColor(Vec<float> colorIn, float alpha);
	void setColorModifier(Vec<float> colorMod, float alphaMod);

	void updateParams();

private:
	SingleColoredMaterial(){}
	
	struct ColorBuffer
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 colorModifier;
	} colorBuffer;

	ID3D11Buffer* constBuffer;
};
