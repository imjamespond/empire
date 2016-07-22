#pragma once

#include "cocos2d.h"

namespace codechiev
{
	class Camera
	{
	public:
		Camera();
		~Camera();
		void transform(cocos2d::Vec2&);
		inline void setPos(const cocos2d::Vec2 &pos) { pos_ = pos; cameraDirty_ = true;};
		inline void setPosX(float x) { pos_.x = x; cameraDirty_ = true; };
		inline void setPosY(float y) { pos_.y = y; cameraDirty_ = true; };
		inline cocos2d::Vec2& getPos() { return pos_; };
		inline bool getCameraDirty() { return cameraDirty_; }
		inline void resetCameraDirty() { cameraDirty_ = false; }

		cocos2d::Mat4 mtx;
	private:
		cocos2d::Vec2 pos_;
		cocos2d::Vec3 pos3d_;
		bool cameraDirty_;
	};
}