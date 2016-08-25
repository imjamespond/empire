#include "GameSceneLayer.h"
#include "GameMenuLayer.hpp"
#include "GameLayer.h"

#include "NotificationLayer.h"
#include "../model/RoleLayer.hpp"
#include "../model/RoleSprite.hpp"
#include "../model/Effect.hpp"
#include "../model/ActionQueue.hpp"
#include "../model/ActionSprite.h"
#include "../model/ScreenShake.hpp"
#include "../model/GameAnim.h"
#include "../model/GameEndLayer.hpp"
#include "../codechiev/BasisUtil.h"
#include <cocostudio/CocoStudio.h>
#include <boost/bind.hpp>

USING_NS_CC;
using namespace codechiev;
using namespace rapidjson;

void
GameSceneLayer::combatEvent(const Document &doc)
{
    if(!(game->state&Game::STATE_BEGIN))
    {
        return;
    }
    
    stopTimer();
    
    const rapidjson::Value& data = doc["data"];
    int turn = data["turn"].GetInt();
    int state = data["state"].GetInt();
    const rapidjson::Value& oper0 = data["oper0"];
    const rapidjson::Value& oper1 = data["oper1"];
    const rapidjson::Value& swap0 = data["swap0"];
    const rapidjson::Value& swap1 = data["swap1"];
    //assert(!swap0.IsNull() && swap0.IsArray() );
    //assert(!swap1.IsNull() && swap1.IsArray() );
    const rapidjson::Value& gameAnim0 = data["anim0"];
    const rapidjson::Value& gameAnim1 = data["anim1"];
    assert(!gameAnim0.IsNull() && gameAnim0.IsArray() && !gameAnim1.IsNull() && gameAnim1.IsArray());
    //assert(gameAnim0.Size()==gameAnim1.Size());
    
    /*if(game->turn==turn)
    {
        return;
    }*/
    
    gGameLayer->gameMenu->removeOperBtn(game->player0, oper0.GetInt());
    gGameLayer->gameMenu->removeOperBtn(game->player1, oper1.GetInt());
    
    //log("combatEvent resetBuff");
    game->player0.resetBuff();
    game->player1.resetBuff();
    
    for(SizeType i=0; i<3; i++)
    {
        if(i<gameAnim0.Size())
        {
            const rapidjson::Value& a0 = gameAnim0[i];
            combatAnimAdd(&game->player0, &game->player1, a0);
        }
        if(i<gameAnim1.Size())
        {
            const rapidjson::Value& a1 = gameAnim1[i];
            combatAnimAdd(&game->player1, &game->player0, a1);
        }
        
        //assert(!a0.IsNull()&&!a1.IsNull());
    }
    
    gNotificationLayer->onError(StringUtils::format("回合:%d", turn));
    game->turn = turn;
    game->state = state;
    //animation
    ActionQueue* queue = base::Singleton<ActionQueue>::get();
    if(!swap0.IsNull() && swap0.Size())
    {
        std::vector<int> ids;
        for(int i(0); i<swap0.Size(); i++)
        {
            ids.push_back(swap0[i].GetInt());
        }
        queue->addQueue(boost::bind(&GameSceneLayer::swapAnim, this, ids, game->player0));
    }
    if(!swap1.IsNull() && swap1.Size())
    {
        std::vector<int> ids;
        for(int i(0); i<swap1.Size(); i++)
        {
            ids.push_back(swap1[i].GetInt());
        }
        queue->addQueue(boost::bind(&GameSceneLayer::swapAnim, this, ids, game->player1));
    }
    combatAnimQueue();
    queue->addQueue(boost::bind(&GameSceneLayer::combatAnimEnd, this));
    queue->playStart();
}

void
GameSceneLayer::combatAnimAdd(Game::Player *player0, Game::Player *player1, const rapidjson::Value& anim)
{
    int id0 = anim["id"].IsNull()?0 : anim["id"].GetInt();
    int id1 = anim["tid"].IsNull()?0 : anim["tid"].GetInt();
    //int id1 = a1["id"].IsNull()?0 : a1["id"].GetInt();
    
    Role *role0 = player0->getRole(id0);
    Role *role1 = player1->getRole(id1);
    assert(role0&&role1);
    
    const rapidjson::Value& anim0 = anim["anim"];
    //const rapidjson::Value& anim1 = a1["anim"];
    
    if(role0->id == Role::ID_YANWANG && !anim0.IsNull()){
        player1->hp = anim0["hp"].IsNull()?player0->hp:anim0["hp"].GetInt();
        int hurt = anim0["hurt"].IsNull()?0:anim0["hurt"].GetInt();
        game_anim_ptr anim(new YanWangAnim(this, player0, player1, role0, role1, player1->hp, hurt));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_Angel){
        player0->hp = anim0["hp"].IsNull()?player0->hp:anim0["hp"].GetInt();
        int heal = anim0["heal"].IsNull()?0:anim0["heal"].GetInt();
        game_anim_ptr anim(new AngelAnim(this, player0, player1, role0, role1, player0->hp, heal));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_Anubis){
        player1->hp = anim0["hp"].IsNull()?player0->hp:anim0["hp"].GetInt();
        int hurt = anim0["hurt"].IsNull()?0:anim0["hurt"].GetInt();
        game_anim_ptr anim(new AnubisAnim(this, player0, player1, role0, role1, player0->hp, hurt));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_Cleopatra){
        game_anim_ptr anim(new CleopatraAnim(this, player0, player1, role0, role1));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_JiShen){
        player0->hp = anim0["hp"].IsNull()?player0->hp:anim0["hp"].GetInt();
        int heal = anim0["heal"].IsNull()?0:anim0["heal"].GetInt();
        game_anim_ptr anim(new JiShenAnim(this, player0, player1, role0, role1, player0->hp, heal));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_Knight){
        player1->hp = anim0["hp"].IsNull()?player0->hp:anim0["hp"].GetInt();
        int hurt = anim0["hurt"].IsNull()?0:anim0["hurt"].GetInt();
        game_anim_ptr anim(new KnightAnim(this, player0, player1, role0, role1, player0->hp, hurt));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_LinLu){
        player1->hp = anim0["hp"].IsNull()?player0->hp:anim0["hp"].GetInt();
        int hurt = anim0["hurt"].IsNull()?0:anim0["hurt"].GetInt();
        game_anim_ptr anim(new LinLuAnim(this, player0, player1, role0, role1, player0->hp, hurt));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_Medusa){
        game_anim_ptr anim(new MedusaAnim(this, player0, player1, role0, role1));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_MENGPO){
        player0->hp = anim0["hp"].IsNull()?player0->hp:anim0["hp"].GetInt();
        int heal = anim0["heal"].IsNull()?0:anim0["heal"].GetInt();
        game_anim_ptr anim(new MengPoAnim(this, player0, player1, role0, role1, player0->hp, heal));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_NIUTOU){
        int shorten = anim0["shorten"].IsNull()?0:anim0["shorten"].GetInt();
        game_anim_ptr anim(new NiuTouAnim(this, player0, player1, role0, role1, shorten));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_Pharaoh){
        player0->hp = anim0["hp"].IsNull()?player0->hp:anim0["hp"].GetInt();
        int heal = anim0["heal"].IsNull()?0:anim0["heal"].GetInt();
        game_anim_ptr anim(new PharaohAnim(this, player0, player1, role0, role1, player0->hp, heal));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_Sphinx){
        player0->hp = anim0["hp"].IsNull()?player0->hp:anim0["hp"].GetInt();
        int hurt = anim0["hurt"].IsNull()?0:anim0["hurt"].GetInt();
        int reflect = anim0["reflect"].IsNull()?0:anim0["reflect"].GetInt();
        game_anim_ptr anim(new SphinxAnim(this, player0, player1, role0, role1, player0->hp, hurt, reflect));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_Titan){
        player1->hp = anim0["hp"].IsNull()?player0->hp:anim0["hp"].GetInt();
        int hurt = anim0["hurt"].IsNull()?0:anim0["hurt"].GetInt();
        game_anim_ptr anim(new TitanAnim(this, player0, player1, role0, role1, player1->hp, hurt));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_WUCHANG){
        int prolong = anim0["prolong"].IsNull()?0:anim0["prolong"].GetInt();
        game_anim_ptr anim(new WuChangAnim(this, player0, player1, role0, role1, prolong));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_YunShen){
        game_anim_ptr anim(new YunShenAnim(this, player0, player1, role0, role1));
        game->animDeq.push_back( anim );
        
    }else if(role0->id == Role::ID_YuShe){
        player0->hp = anim0["hp0"].IsNull()?player0->hp:anim0["hp0"].GetInt();
        player1->hp = anim0["hp1"].IsNull()?player1->hp:anim0["hp1"].GetInt();
        int heal = anim0["heal"].IsNull()?0:anim0["heal"].GetInt();
        int hurt = anim0["hurt"].IsNull()?0:anim0["hurt"].GetInt();
        game_anim_ptr anim(new YuSheAnim(this, player0, player1, role0, role1, player0->hp, player1->hp, heal, hurt));
        game->animDeq.push_back( anim );
        
    }
}

void
GameSceneLayer::combatAnimQueue()
{
    ActionQueue* queue = base::Singleton<ActionQueue>::get();
    
    game_anim_deq::iterator it;
    it = game->animDeq.begin();
    while(it!=game->animDeq.end())
    {
        game_anim_ptr& anim = *it;
        queue->addQueue(boost::bind(&GameAnim::play, anim));
        
        game->animDeq.erase(it);
        it = game->animDeq.begin();
    }
}

void
GameSceneLayer::combatAnimEnd()
{
    log("combatAnimEnd");
    if(game->isBegin())
    {
        startTimer();
    }
    
    base::Singleton<ActionQueue>::get()->playNext();
}
