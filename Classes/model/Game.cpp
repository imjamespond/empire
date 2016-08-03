//
//  GameScene.cpp
//  empire
//
//  Created by metasoft on 16/4/26.
//
//

#include "Game.hpp"
#include "ActionSprite.h"
#include "RoleLayer.hpp"
#include "RoleSprite.hpp"
#include "User.h"
#include "Effect.hpp"
#include "../codechiev/base/singleton.h"

using namespace codechiev;
using namespace rapidjson;

Role*
Game::Player::getRole(int id)
{
    for(int i=0; i<(sizeof roles)/sizeof(Role*); i++)
    {
        if(id&&id==roles[i]->id)
            return roles[i];
    }
    return nullptr;
}
void
Game::Player::reset()
{
    for(int i=0; i<(sizeof roles)/sizeof(Role*); i++)
    {
        if(roles[i])
            roles[i]->removeFromParent();
    }
    ::memset(swap, 0, sizeof swap);
    ::memset(roles, 0, sizeof roles);
    resetBuff();
}
void
Game::Player::resetBuff()
{
    petrify=0;
    prolong=0;
}
bool
Game::Player::checkPetrify()
{
    if(petrify>0)
    {
        for(int i=0; i<(sizeof roles)/sizeof(Role*); i++)
        {
            roles[i]->setPetrify(true);
        }
        return true;
    }else
    {
        for(int i=0; i<(sizeof roles)/sizeof(Role*); i++)
        {
            roles[i]->setPetrify(false);
        }
    }
    return false;
}
void
Game::Player::setSelfAura()
{
    /*for(int i=0; i<(sizeof roles)/sizeof(Role*); i++)
    {
        roles[i]->aura->setColor(cocos2d::Color3B::RED);
    }*/
}

void
Game::initGame(const Document & doc)
{
    self = nullptr;
    const Value& data = doc["data"];
    const Value& p0 = data["player0"];
    initPlayer(player0, p0);
    const Value& p1 = data["player1"];
    initPlayer(player1, p1);
    
    turn = data["turn"].IsNull()?0:data["turn"].GetInt();
    map = data["map"].IsNull()?0:data["map"].GetInt();
    state = Game::STATE_BEGIN;
    player0.resetBuff();
    player1.resetBuff();
}

void
Game::initPlayer(struct Player& player, const Value &val)
{
    AnimationConfig *config = base::Singleton<AnimationConfig>::get();
    
    player.uid = val["uid"].GetInt64();
    player.name = val["name"].IsNull()?"":val["name"].GetString();
    player.defense = val["defense"].GetInt();
    player.level = val["level"].GetInt();
    player.type = val["type"].GetInt();
    player.hp = val["hp"].GetInt();
    player.maxHp = player.hp;
    const Value& roles = val["roles"];
    assert(roles.IsArray());
    for(SizeType i=0; i<(sizeof player.roles)/sizeof(Role*); i++)
    {
        if(i<roles.Size())
        {
            player.roles[i] = Role::create();
            player.roles[i]->id = roles[i].GetInt();
            RoleStruct& roleStruct = config->roleStructMap[player.roles[i]->id];
            player.roles[i]->name = roleStruct.name;
            player.roles[i]->initRole();
        }
    }
    
    const Value& swap = val["swap"];
    assert(swap.IsArray());
    for(SizeType i=0; i<(sizeof player.swap)/sizeof(int); i++)
    {
        if(i<swap.Size())
        {
            player.swap[i] = swap[i].GetInt();
        }
    }
    
    //check is self
    User *user = base::Singleton<User>::get();
    if(user->uid == player.uid)
    //if(user->name.compare(player.name)==0)
    {
        self = &player;
        self->setSelfAura();
    }else
    {
        enemy = &player;
    }
    //turn
    turn = 1;
}

void
Game::resetScene()
{
    self = nullptr;
    turn = 0;
    state = 0;
    
    player0.reset();
    player1.reset();
}

Game::Player*
Game::getPlayer(int64_t uid)
{
    if(player0.uid == uid){
        return &player0;
    }else if(player1.uid == uid){
        return &player1;
    }
    return nullptr;
}
