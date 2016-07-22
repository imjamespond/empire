#pragma once
#include "cocos2d.h"
#include <unordered_map>

namespace codechiev
{
    struct AnimationStruct;
	extern cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const struct AnimationStruct&);
    extern cocos2d::Animate* getAction(const struct AnimationStruct&);
    
    struct AnimationStruct
    {
        AnimationStruct(const char* n, int b, int e, float d):
        name(n),begin(b),end(e),duration(d){};
        AnimationStruct(){}
        std::string name;
        int begin;
        int end;
        float duration;
    };
    
    struct RoleStruct
    {
        RoleStruct():id(0),type(0){}
        std::string name;
        int id;
        int type;
        struct AnimationStruct walk;
        struct AnimationStruct idle;
        struct AnimationStruct attack;
        struct AnimationStruct face;
        struct AnimationStruct back;
    };
    
    static struct AnimationStruct gAnimExplode("explosion/img_%4d.png", 7132, 7147, 1);
    static struct AnimationStruct gAnimAura("aura/img_%4d.png", 1095, 1110, 1);
    
    class AnimationConfig
    {
    public:
        void init();
        
        typedef std::unordered_map<int, struct RoleStruct> role_struct_map;
        role_struct_map roleStructMap;
    };
}


