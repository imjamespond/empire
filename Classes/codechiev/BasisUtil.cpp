#include "BasisUtil.h"

USING_NS_CC;

Vec2 gVisibleSize(0, 0);
Vec2 gOrigin(0, 0);

Vec2 gRightBottom(0, 0);
Vec2 gCenter(0, 0);
Vec2 gLeftBottom(0, 0); 
Vec2 gLeftTop(0, 0);

using namespace codechiev;


BasisUtil::BasisUtil()
{
	//appData();//must call from cocos init?
}
BasisUtil::~BasisUtil() 
{
}

void 
BasisUtil::appData(){}
void 
BasisUtil::appDataSave()
{
	//auto fu = FileUtils::getInstance();
	//std::string path = fu->getWritablePath();
	//path += APPDATA_FILE;
	//if (fu->writeValueMapToFile(valMap_, path)){log("see the plist file at %s", path.c_str());}
	//else{log("write plist file failed");}
    UserDefault::getInstance()->flush();

}
void
BasisUtil::appDataClear()
{
    //valMap_.clear();
    //valMap_.erase("username");
    UserDefault::getInstance()->deleteValueForKey(kAppKeys[AK_UNAME]);
    UserDefault::getInstance()->deleteValueForKey(kAppKeys[AK_PASSWD]);
}


void
BasisUtil::setAppString(AppKey key, const std::string& str)
{
    UserDefault::getInstance()->setStringForKey(kAppKeys[key], str.c_str());
    base::Singleton<BasisUtil>::get()->appDataSave();
}
void
BasisUtil::setAppBool(AppKey key, bool val)
{
    UserDefault::getInstance()->setBoolForKey(kAppKeys[key], val);
    base::Singleton<BasisUtil>::get()->appDataSave();
}
void
BasisUtil::setAppFloat(AppKey key, float val)
{
    UserDefault::getInstance()->setFloatForKey(kAppKeys[key], val);
    base::Singleton<BasisUtil>::get()->appDataSave();
}
bool
BasisUtil::getAppBool(AppKey key)
{
    return UserDefault::getInstance()->getBoolForKey(kAppKeys[key]);
}
std::string
BasisUtil::getAppString(AppKey key)
{
    return UserDefault::getInstance()->getStringForKey(kAppKeys[key]);
}
float
BasisUtil::getAppFloat(AppKey key)
{
    return UserDefault::getInstance()->getFloatForKey(kAppKeys[key]);
}

BaseDebug::BaseDebug(){}
BaseDebug::~BaseDebug(){log("~BaseDebug");}

void 
BaseDebug::add(const std::string& msg)
{
	msgDeq_.push_front(msg);

	if (msgDeq_.size()>20)
	{
		msgDeq_.pop_back();
	}
	dirty = true;
}

std::string 
BaseDebug::toString()
{
	std::string message;
	for (const std::string &m : msgDeq_)
	{
		message += m + "\n";
	}
	dirty = false;
	return message;
}

//UUID
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/lexical_cast.hpp>
std::string
codechiev::getUuid()
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string uuidStr = boost::lexical_cast<std::string>(uuid);
    return uuidStr;
}

//random password
std::string
codechiev::randomPasswd(int len)
{
    static const char source[] = "QWERTYUIOPASDFGHJKLZXCVBNM1234567890qwertyuiopasdfghjklzxcvbnm";
    float sourceLen = (sizeof source)/sizeof(char)-1;//exclude \0 terminator
    std::string passwd;
    for(int i=0; i<len; i++)
    {
        float ramdon = std::floor( cocos2d::random(0.0f, sourceLen));
        passwd+=source[static_cast<int>(ramdon)];
    }
    return passwd;
}

//IME
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
// load jni libraries
#include <platform/android/jni/JniHelper.h>
std::string 
codechiev::getDeviceID() {
	std::string str;
	cocos2d::JniMethodInfo methodInfo;

	cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "getDeviceID", "()Ljava/lang/String;"); // find our Java method
	jstring jpath = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);      // For now we use a static method to retrieve a string value using CallStaticObjectMethod
	const char* npath = methodInfo.env->GetStringUTFChars(jpath, NULL);
	str = npath;

	// release objects/methods/anything we no longer need
	methodInfo.env->ReleaseStringUTFChars(jpath, npath);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	return str.c_str();
}
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_IOS )
//defined in BasisUtil.mm
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
std::string 
codechiev::getDeviceID()
{
    return getUuid();
}
#else
// Nothing to do here
#endif

void
codechiev::setRightTop(Node* node, float margin)
{
    node->setPositionX(gVisibleSize.x-node->getContentSize().width*.5f-margin);
    node->setPositionY(gVisibleSize.y-node->getContentSize().height*.5f-margin);
}

void
codechiev::setRightBottom(Node* node, float margin)
{
    node->setPositionX(gVisibleSize.x-node->getContentSize().width*.5f-margin);
    node->setPositionY(node->getContentSize().height*.5f+margin);
}

void
codechiev::setCenterBottom(Node* node, float margin)
{
    node->setPositionX(gCenter.x-node->getContentSize().width*.5f);
    node->setPositionY(node->getContentSize().height*.5f+margin);
}

void
codechiev::showElasticDialog(Layer* layer, float time)
{
    layer->setVisible(true);
    layer->setScale(.3f);
    layer->runAction(EaseElasticOut::create(ScaleTo::create(1, 1)));
}

#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS )
// Nothing to do here
#else
void
codechiev::showKeyboard()
{
}

void
codechiev::onDismissKeyboard()
{
}
#endif

bool
codechiev::isFileExist(const char* pFileName)
{
    FileUtils* fu = FileUtils::getInstance();
    if( !pFileName ) return false;
    std::string filePath = fu->getWritablePath();
    filePath += pFileName;
    
    FILE *fp = fopen(filePath.c_str(),"r");
    if(fp)
    {
        fclose(fp);
        return true;
    }
    return false;
}

void
codechiev::copyData(const char* pFileName)
{
    FileUtils* fu = FileUtils::getInstance();
    
    std::string strPath = fu->fullPathForFilename(pFileName);
    std::string data = fu->getStringFromFile(strPath.c_str());
    std::string destPath = fu->getWritablePath();
    destPath += pFileName;
    cocos2d::log("copyData %s, %s, %s", strPath.c_str(), destPath.c_str(), data.c_str());
    FILE *fp = fopen(destPath.c_str(),"w+");
    fwrite(data.c_str(),sizeof(char),data.length(),fp);
    fclose(fp);
}

