#include "ActionSprite.h"

USING_NS_CC;
using namespace codechiev;

Vector<SpriteFrame*>
codechiev::getAnimation(const struct AnimationStruct& anim)
{
	auto spritecache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	char str[100];
	for (int i = anim.begin; i <= anim.end; i++)
	{
		sprintf(str, anim.name.c_str(), i);
        auto sprite = spritecache->getSpriteFrameByName(str);
        if(sprite)
            animFrames.pushBack(sprite);
	}
	return animFrames;
}

Animate*
codechiev::getAction(const struct AnimationStruct& anim)
{
    if(!anim.name.length())
        return nullptr;
    // sprite
    auto frames = getAnimation(anim);
    if (frames.size())
    {
        //SpriteFrame* sf = frames.at(0);
        //const Rect& rect = sf->getRect();
        //this->setContentSize(rect.size);
    }else
    {
        return nullptr;
    }
    auto spriteAnim = Animation::createWithSpriteFrames(frames, anim.duration / (anim.end>anim.begin?anim.end-anim.begin:1));//
    Animate* action = Animate::create(spriteAnim);
    action->retain();
    return action;
}

namespace codechiev {
    struct AnimationStruct gAnimShalou;
}

void
AnimationConfig::init()
{
    auto fu = FileUtils::getInstance();
    std::string fullname=fu->fullPathForFilename("plist/config-anim.plist");
    ValueMap valMap = fu->getValueMapFromFile(fullname);
    //init role
    ValueVector& roles = valMap["roles"].asValueVector();
    for(ValueVector::iterator rolesIt = roles.begin();
        rolesIt != roles.end();
    rolesIt++)
    {
        ValueMap& role = rolesIt->asValueMap();
        int id = role["id"].asInt();
        struct RoleStruct &roleStruct = roleStructMap[id];
        roleStruct.id = id;
        
        Value& name = role["name"];
        assert(!name.isNull());
        roleStruct.name =name.asString();
        
        Value& desc = role["descri"];
        if(!desc.isNull())
            roleStruct.description =desc.asString();
        Value& profession = role["profess"];
        if(!profession.isNull())
            roleStruct.profession =profession.asString();
        Value& skill = role["skill"];
        if(!skill.isNull())
            roleStruct.skill =skill.asString();
        Value& skillName = role["skillname"];
        if(!skillName.isNull())
            roleStruct.skillName =skillName.asString();
        
        Value& type = role["type"];
        assert(!type.isNull());
        roleStruct.type = type.asInt();
        
        ValueMap& data = role["data"].asValueMap();
        
        ValueMap& walk = data["walk"].asValueMap();
        roleStruct.walk.name = walk["name"].asString();
        roleStruct.walk.begin = walk["begin"].asInt();
        roleStruct.walk.end = walk["end"].asInt();
        roleStruct.walk.duration = walk["duration"].asFloat();
        
        ValueMap& attack = data["attack"].asValueMap();
        roleStruct.attack.name = attack["name"].asString();
        roleStruct.attack.begin = attack["begin"].asInt();
        roleStruct.attack.end = attack["end"].asInt();
        roleStruct.attack.duration = attack["duration"].asFloat();
        
        ValueMap& idle = data["idle"].asValueMap();
        roleStruct.idle.name = idle["name"].asString();
        roleStruct.idle.begin = idle["begin"].asInt();
        roleStruct.idle.end = idle["end"].asInt();
        roleStruct.idle.duration = idle["duration"].asFloat();
        
        ValueMap& face = data["walk-face"].asValueMap();
        roleStruct.face.name = face["name"].asString();
        roleStruct.face.begin = face["begin"].asInt();
        roleStruct.face.end = face["end"].asInt();
        roleStruct.face.duration = face["duration"].asFloat();
        
        ValueMap& back = data["walk-back"].asValueMap();
        roleStruct.back.name = back["name"].asString();
        roleStruct.back.begin = back["begin"].asInt();
        roleStruct.back.end = back["end"].asInt();
        roleStruct.back.duration = back["duration"].asFloat();
    }
    //init effect
    ValueVector& effect = valMap["effect"].asValueVector();
    ValueMap& shalou = effect[0].asValueMap();
    gAnimShalou.name = shalou["name"].asString();
    gAnimShalou.begin = shalou["begin"].asInt();
    gAnimShalou.end = shalou["end"].asInt();
    gAnimShalou.duration = shalou["duration"].asInt();
}