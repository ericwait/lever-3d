////////////////////////////////////////////////////////////////////////////////////
//
//     Copyright 2014 Andrew Cohen, Eric Wait, and Mark Winter
//
//     This file is part of LEVER 3-D - the tool for 5-D stem cell segmentation,
//     tracking, and lineaging. See http://bioimage.coe.drexel.edu 'software' section
// 	  for details.
//
//     LEVER 3-D is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by the Free
//     Software Foundation, either version 3 of the License, or (at your option) any
//     later version.
// 
//     LEVER 3-D is distributed in the hope that it will be useful, but WITHOUT ANY
//     WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
//     A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with LEVer in file "gnu gpl v3.txt".  If not, see 
//     <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Vec.h"
#include <DirectXMath.h>
#include "Renderer.h"

class Camera
{
public:
	Camera(Vec<float> cameraPositionIn, Vec<float> lookPositionIn, Vec<float> upDirectionIn);

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void zoomIncrement();
	void zoomDecrement();
	void resetCamera();
	void setCameraPosition(Vec<float> cameraPositionIn);
	void setLookPosition(Vec<float> lookPositionIn);
	void setUpDirection(Vec<float> upDirectionIn);
	void setCamera(Vec<float> cameraPositionIn, Vec<float> lookPositionIn, Vec<float> upDirectionIn);
	virtual void updateProjectionTransform();

	DirectX::XMMATRIX getProjectionTransform() const {return projectionTransform;}
	DirectX::XMMATRIX getViewTransform() const {return viewTransform;}
	void getRay(int iMouseX, int iMouseY, Vec<float>& pointOut, Vec<float>& directionOut);

protected:
	Camera(){}

	virtual void updateViewTransform();
	Vec<float> cameraPosition;
	Vec<float> lookPosition;
	Vec<float> upDirection;
	Vec<float> defaultCameraPosition;
	Vec<float> defaultLookPosition;
	Vec<float> defaultUpDirection;
	DirectX::XMMATRIX viewTransform;
	DirectX::XMMATRIX projectionTransform;
	float zoomFactor;
};


class OrthoCamera : public Camera
{
public:
	OrthoCamera(Vec<float> cameraPostionIn, Vec<float> lookPostionIn, Vec<float> upDirectionIn);

	void updateProjectionTransform();

private:
	OrthoCamera();
};
