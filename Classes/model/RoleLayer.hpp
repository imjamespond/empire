//
//  RoleLayer.hpp
//  empire
//
//  Created by metasoft on 16/5/4.
//
//

#ifndef RoleLayer_hpp
#define RoleLayer_hpp

#include <cocos2d.h>
#include <boost/shared_ptr.hpp>

namespace codechiev {
    class RoleSprite;
    class Effect;
    class Role : public cocos2d::Layer
    {
    public:
        enum RoleID
        {
            ID_YANWANG=1,
            ID_WUCHANG=2,
            ID_MENGPO=3,
            ID_NIUTOU=4,
            
            ID_Anubis=5,
            ID_Sphinx=6,
            ID_Pharaoh=7,
            ID_Cleopatra=8,
            
            ID_LinLu=9,
            ID_YuShe=10,
            ID_JiShen=11,
            ID_YunShen=12,
            
            ID_Knight=13,
            ID_Titan=14,
            ID_Angel=15,
            ID_Medusa=16,
        };
        
        Role();
        virtual ~Role();
        virtual bool initRole();
        
        CREATE_FUNC(Role);
        
        //void initName(const std::string&);
        void setPetrify(bool);
        float walkTo(const cocos2d::Vec2&, float);
        
        std::string name;
        int id;
        
        RoleSprite *role;
        Effect* aura;
    private:
    };
    //typedef boost::shared_ptr<Role> role_ptr;
}

#endif /* RoleLayer_hpp */
