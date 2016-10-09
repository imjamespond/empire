LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := crypto_static
LOCAL_SRC_FILES := /Users/metasoft/Documents/document/openssl-OpenSSL_1_0_2-stable/libcrypto.a
LOCAL_EXPORT_C_INCLUDES := /Users/metasoft/Documents/document/openssl-OpenSSL_1_0_2-stable/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ssl_static
LOCAL_SRC_FILES := /Users/metasoft/Documents/document/openssl-OpenSSL_1_0_2-stable/libssl.a
LOCAL_EXPORT_C_INCLUDES := /Users/metasoft/Documents/document/openssl-OpenSSL_1_0_2-stable/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/./AppDelegate.cpp \
../../Classes/./AudioManager.cpp \
../../Classes/./codechiev/AStar.cpp \
../../Classes/./codechiev/BaseCmd.cpp \
../../Classes/./codechiev/BasisUtil.cpp \
../../Classes/./codechiev/MsgHandler.cpp \
../../Classes/./codechiev/Network.cpp \
../../Classes/./codechiev/base/aes.cpp \
../../Classes/./codechiev/base/base64.cpp \
../../Classes/./codechiev/base/hash.cpp \
../../Classes/./codechiev/base/rsa.cpp \
../../Classes/./codechiev/base/TimeUtil.cpp \
../../Classes/./codechiev/control/ChatController.cpp \
../../Classes/./codechiev/control/GameController.cpp \
../../Classes/./codechiev/control/LoginController.cpp \
../../Classes/./codechiev/transform.cpp \
../../Classes/./layer/GameLayer.cpp \
../../Classes/./layer/GameMenuLayer.cpp \
../../Classes/./layer/GameSceneAnim.cpp \
../../Classes/./layer/GameSceneLayer.cpp \
../../Classes/./layer/LoadingLayer.cpp \
../../Classes/./layer/LoginLayer.cpp \
../../Classes/./layer/MenuLayer.cpp \
../../Classes/./layer/NotificationLayer.cpp \
../../Classes/./model/ActionSprite.cpp \
../../Classes/./model/CardLayer.cpp \
../../Classes/./model/ClipLayer.cpp \
../../Classes/./model/Configure.cpp \
../../Classes/./model/CountDown.cpp \
../../Classes/./model/Effect.cpp \
../../Classes/./model/ModalLayer.cpp \
../../Classes/./model/RankLayer.cpp \
../../Classes/./model/Game.cpp \
../../Classes/./model/GameAnim.cpp \
../../Classes/./model/GameEndLayer.cpp \
../../Classes/./model/HallLayer.cpp \
../../Classes/./model/MessageLayer.cpp \
../../Classes/./model/NavigateLayer.cpp \
../../Classes/./model/Particle.cpp \
../../Classes/./model/RoleLayer.cpp \
../../Classes/./model/RoleSprite.cpp \
../../Classes/./model/CycleShowPageView.cpp \
../../Classes/./model/ScreenShake.cpp \
../../Classes/./model/PvpSearchLayer.cpp \
../../Classes/./model/RedeemLayer.cpp \
../../Classes/./model/RecieveOkLayer.cpp \
../../Classes/./model/UpgradeOkLayer.cpp \
../../Classes/./model/ConfirmLayer.cpp \
../../Classes/./model/ExchangeLayer.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += /Users/metasoft/Documents/projects/source/boost_1_60_0

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += crypto_static
LOCAL_STATIC_LIBRARIES += ssl_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
