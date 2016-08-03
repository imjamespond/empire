//
//  effect.hpp
//  empire
//
//  Created by metasoft on 16/5/5....
//
//

#ifndef effect_hpp
#define effect_hpp

#include <cocos2d.h>

namespace codechiev
{
    class Effect : public cocos2d::Sprite
    {
    public:
        
        CREATE_FUNC(Effect);
        
        void initEffect(const struct AnimationStruct&, float delay=0.0f, bool repeat=true);
        void initEffect2(const struct AnimationStruct&);
    private:
    };
    
}

#endif /* effect_hpp */
