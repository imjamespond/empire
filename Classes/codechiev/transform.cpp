#include "transform.h"

using namespace codechiev;

Camera::Camera():pos_(0.0f,0.0f), pos3d_(0.0f, 0.0f, 0.0f), cameraDirty_(false)
{
}

Camera::~Camera()
{
}

void Camera::transform(cocos2d::Vec2& layerPos)
{
	mtx = cocos2d::Mat4::IDENTITY;
	pos3d_.x = pos_.x;
	pos3d_.y = pos_.y;
	mtx.translate(pos3d_);
	mtx.inverse();
	mtx.getTranslation(&pos3d_);
	layerPos.x = pos3d_.x;
	layerPos.y = pos3d_.y;
}
