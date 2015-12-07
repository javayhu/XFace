APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -fexceptions
#can not put 'all' here! arm64-v8a no! 'mips64' no!
APP_ABI := armeabi armeabi-v7a mips x86
APP_PLATFORM := android-16
