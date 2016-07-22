#pragma once
#include <cocos2d.h>

namespace codechiev
{
    class Effect;
	class RoleSprite : public cocos2d::Sprite
	{
	public:
        enum Orientation
        {
            ORN_NORMAL,
            ORN_FACE,
            ORN_BACK,
            ORN_FLIP,
        };
        
        RoleSprite();
		virtual ~RoleSprite();
        virtual bool init();

		CREATE_FUNC(RoleSprite);

        void idle();
		void walk();
        void attack();
        void hurt();
        
        void forever(cocos2d::Animate*);
        void once(cocos2d::Animate*);

        cocos2d::Animate* idleAction;
        cocos2d::Animate* walkAction;
        cocos2d::Animate* attackAction;
        cocos2d::Animate* faceAction;
        cocos2d::Animate* backAction;
        
	private:
	};

}

