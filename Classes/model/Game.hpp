//
//  GameScene.hpp
//  empire
//
//  Created by metasoft on 16/4/26.
//
//

#ifndef Game_hpp
#define Game_hpp

#include <external/json/document.h>
#include <boost/shared_ptr.hpp>
#include <unordered_map>
#include <deque>
#include <cocos2d.h>
#include <ui/CocosGUI.h>

class GameSceneLayer;

namespace codechiev
{
    class GameAnim;
    class Role;
    
    typedef typename boost::shared_ptr<GameAnim> game_anim_ptr;
    typedef typename std::deque<game_anim_ptr> game_anim_deq;
    
    class Game
    {
    public:
        struct Player
        {
            Player():
            uid(0),hp(0),maxHp(0),defense(0),level(0),type(0),
            petrify(0),prolong(0.0f),shorten(0.0f), opertime(0.0f)
            {
                ::memset(swap, 0, sizeof swap);
                ::memset(roles, 0, sizeof roles);
            }
            Role* getRole(int);
            void reset();
            void resetBuff();
            bool checkPetrify();
            void setSelfAura();
            
            int64_t uid;
            std::string name;
            int hp;
            int maxHp;
            int defense;
            int level;
            int type;
            //buff
            int petrify;
            float prolong;
            float shorten;
            //time
            float opertime;
            //ui
            cocos2d::ui::Text *txPlayer;
            cocos2d::ui::Text *txTimer;
            cocos2d::ui::TextBMFont *txHealth;
            cocos2d::ui::LoadingBar *healthBar;
            cocos2d::ui::LoadingBar *healthBarBg;
            
            //int curswap;
            Role* roles[4];
            int swap[8];
        };

        Game():self(nullptr),enemy(nullptr),state(0),turn(0),map(0){}
        
        void initGame(const rapidjson::Document& );
        void initPlayer(struct Player& , const rapidjson::Value& );
        void onTimeup();
        void resetScene();
        Player* getPlayer(int64_t);
        inline bool isBegin(){ return state & STATE_BEGIN;}
        inline Player* getSelf(){return self;};
        inline Player* getEnemy(){return enemy;};
        
        Player player0;
        Player player1;
        Player* self;
        Player* enemy;
        int state;
        int turn;
        int map;
        game_anim_deq animDeq;
        
        static const int kMaxTurn = 4;
        static const int STATE_BEGIN = 1;
        static const int STATE_END = 2;
        static const int STATE_WAIT = 4;
        static const int STATE_ANIM = 8;
    private:
    };
    
    class GameAnim
    {
    public:
        GameAnim(GameSceneLayer *s, Game::Player* sf, Game::Player* en,
                 Role *r, Role *t):
        scene(s),self(sf),enemy(en),role(r),target(t){}
        virtual ~GameAnim(){cocos2d::log("~GameAnim");}
        
        virtual void play()=0;
        
        GameSceneLayer *scene;
        Role* role;
        Role* target;
        Game::Player* self;
        Game::Player* enemy;
    };

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#define ARG_SEQ(R, data, i, TYPE_AND_NAME) \
    , BOOST_PP_TUPLE_ELEM(2, 0, TYPE_AND_NAME) BOOST_PP_TUPLE_ELEM(2, 1, TYPE_AND_NAME)
#define GAME_ANIM(clazz, arg_seq) explicit clazz(GameSceneLayer *s,\
    Game::Player* sf,Game::Player* en,\
    Role *r, Role *t\
    BOOST_PP_SEQ_FOR_EACH_I( ARG_SEQ, , arg_seq)):GameAnim(s, sf, en, r, t)

}
#endif /* GameScene_hpp */
