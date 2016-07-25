//
//  GameAnim.h
//  empire
//
//  Created by metasoft on 16/6/3.
//
//

#ifndef GameAnim_h
#define GameAnim_h

#include "Game.hpp"

namespace codechiev
{
    class YanWangAnim : public GameAnim
    {
    public:
        //GAME_ANIM(YanWangAnim, ){}
        GAME_ANIM(YanWangAnim, ((int,hitpoint))((int,ht))){hp=hitpoint;hurt=ht;}
        void play();//override
        
        int hp;
        int hurt;
    };
    
    class WuChangAnim : public GameAnim
    {
    public:
        GAME_ANIM(WuChangAnim, ((int,pl))){prolong=pl;}
        void play();
        
        int prolong;
    };
    
    class MengPoAnim : public GameAnim
    {
    public:
        GAME_ANIM(MengPoAnim, ((int,hitpoint))((int,hl))){hp=hitpoint;heal=hl;}
        void play();
        
        int hp;
        int heal;
    };
    
    class NiuTouAnim : public GameAnim
    {
    public:
        GAME_ANIM(NiuTouAnim, ((int,st))){shorten=st;}
        void play();
        
        int shorten;
    };
    
    class AnubisAnim : public GameAnim
    {
    public:
        GAME_ANIM(AnubisAnim, ((int,hitpoint))((int,ht))){hp=hitpoint;hurt=ht;}
        void play();
        
        int hp;
        int hurt;
    };
    
    class SphinxAnim : public GameAnim
    {
    public:
        GAME_ANIM(SphinxAnim, ((int,hp))((int,ht))((int,rf)))
        {this->hp=hp;hurt=ht;reflect=rf;}
        void play();
        
        int hp;
        int hurt;
        int reflect;
    };
    
    class PharaohAnim : public GameAnim
    {
    public:
        GAME_ANIM(PharaohAnim, ((int,hp))((int,heal)))
        {this->hp=hp;this->heal=heal;}
        void play();
        
        int hp;
        int heal;
    };
    
    class CleopatraAnim : public GameAnim
    {
    public:
        GAME_ANIM(CleopatraAnim, ){}
        void play();
    };
    
    class LinLuAnim : public GameAnim
    {
    public:
        GAME_ANIM(LinLuAnim, ((int,hitpoint))((int,ht))){hp=hitpoint;hurt=ht;}
        void play();
        
        int hp;
        int hurt;
    };
    
    class YuSheAnim : public GameAnim
    {
    public:
        GAME_ANIM(YuSheAnim, ((int,hp0))((int,hp1))((int,heal))((int,hurt)) )
        {this->hp0=hp0;this->hp1=hp1;this->heal=heal;this->hurt=hurt;}
        void play();
        
        int hp0;
        int hp1;
        int heal;
        int hurt;
    };
    class JiShenAnim : public GameAnim
    {
    public:
        GAME_ANIM(JiShenAnim, ((int,hp))((int,heal)))
        {this->hp=hp;this->heal=heal;}
        void play();
        
        int hp;
        int heal;
    };
    class YunShenAnim : public GameAnim
    {
    public:
        GAME_ANIM(YunShenAnim, ){}
        void play();
    };
    
    class KnightAnim : public GameAnim
    {
    public:
        GAME_ANIM(KnightAnim, ((int,hp))((int,hurt)))
        {this->hp=hp;this->hurt=hurt;}
        void play();
        
        int hp;
        int hurt;
    };
    
    class TitanAnim : public GameAnim
    {
    public:
        GAME_ANIM(TitanAnim, ((int,hp))((int,hurt)))
        {this->hp=hp;this->hurt=hurt;}
        void play();
        
        int hp;
        int hurt;
    };
    
    class AngelAnim : public GameAnim
    {
    public:
        GAME_ANIM(AngelAnim, ((int,hp))((int,heal)))
        {this->hp=hp;this->heal=heal;}
        void play();
        
        int hp;
        int heal;
    };
    class MedusaAnim : public GameAnim
    {
    public:
        GAME_ANIM(MedusaAnim, ){
            enemy->petrify=1;
        }
        
        void play();
    };

}

#endif /* GameAnim_h */
