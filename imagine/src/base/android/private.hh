#pragma once

#include <util/jni.hh>

#if CONFIG_ENV_ANDROID_MINSDK >= 9
	#include "nativeGlue.hh"
#endif

namespace Base
{

extern JavaVM* jVM;
//extern JNIEnv* jEnv; // JNIEnv of main thread
JNIEnv* aEnv(); // JNIEnv of activity thread
JNIEnv* eEnv(); // JNIEnv of main event thread

#if CONFIG_ENV_ANDROID_MINSDK >= 9
// NativeActivity Instance
android_app *appInstance();
#endif

// BaseActivity JNI
extern jclass jBaseActivityCls;
extern jobject jBaseActivity;
extern JavaInstMethod<void> postUIThread;
extern JavaInstMethod<void> jShowIme, jHideIme;

#if CONFIG_ENV_ANDROID_MINSDK >= 9

// Dispatch a command from native glue
void onAppCmd(android_app* app, uint32 cmd);

// Check if EGL surface is valid
bool windowIsDrawable();

// Init the SDK level
void setSDK(uint sdk);

// Init JNI variables from native glue Activity thread
void jniInit(JNIEnv *jEnv, jobject inst);

// Android Bluetooth
static const ushort MSG_BT_SOCKET_STATUS_DELEGATE = 151;

#endif

// EditText-based Input
void sendTextEntryEnded(const char *str, jstring jStr);

}

struct ANativeWindow;

namespace Gfx
{

// SurfaceTexture JNI
struct AndroidSurfaceTextureConfig
{
	constexpr AndroidSurfaceTextureConfig() { }
	jclass jSurfaceCls = nullptr, jSurfaceTextureCls = nullptr;
	JavaInstMethod<void> jSurface, jSurfaceRelease,
		jSurfaceTexture, jUpdateTexImage, jSurfaceTextureRelease/*, jSetDefaultBufferSize*/;
	bool use = 0;
	bool texture2dBindingHack = 0;
	// Extra dlsym function from libandroid.so
	//ANativeWindow* (*ANativeWindow_fromSurfaceTexture)(JNIEnv* env, jobject surfaceTexture) = nullptr;

	void init(JNIEnv *jEnv);
	void deinit();

	bool isSupported()
	{
		return jSurfaceTextureCls;
	}

	void setUse(bool on)
	{
		if(isSupported())
			use = on;
	}
};

extern AndroidSurfaceTextureConfig surfaceTextureConf;

}