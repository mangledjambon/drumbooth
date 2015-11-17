# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_ARM_MODE := arm
endif

LOCAL_MODULE := juce_jni
LOCAL_SRC_FILES := \
  ../../../Source/Transforms/ISTFT.cpp\
  ../../../Source/Transforms/STFT.cpp\
  ../../../Source/HarmonicPercussiveSeparator.cpp\
  ../../../Source/Main.cpp\
  ../../../../../Cpp/JUCE/modules/juce_audio_basics/juce_audio_basics.cpp\
  ../../../../../Cpp/JUCE/modules/juce_audio_devices/juce_audio_devices.cpp\
  ../../../../../Cpp/JUCE/modules/juce_audio_formats/juce_audio_formats.cpp\
  ../../../../../Cpp/JUCE/modules/juce_audio_processors/juce_audio_processors.cpp\
  ../../../../../Cpp/JUCE/modules/juce_core/juce_core.cpp\
  ../../../../../Cpp/JUCE/modules/juce_cryptography/juce_cryptography.cpp\
  ../../../../../Cpp/JUCE/modules/juce_data_structures/juce_data_structures.cpp\
  ../../../../../Cpp/JUCE/modules/juce_events/juce_events.cpp\
  ../../../../../Cpp/JUCE/modules/juce_graphics/juce_graphics.cpp\
  ../../../../../Cpp/JUCE/modules/juce_gui_basics/juce_gui_basics.cpp\
  ../../../../../Cpp/JUCE/modules/juce_gui_extra/juce_gui_extra.cpp\

ifeq ($(NDK_DEBUG),1)
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -g -I "../../JuceLibraryCode" -I "../../../../Cpp/JUCE/modules" -O0 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_medianfiltering_MedianFiltering" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/medianfiltering/MedianFiltering\" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2 -landroid -lEGL
  LOCAL_CFLAGS += -fsigned-char -fexceptions -frtti -g -I "../../JuceLibraryCode" -I "../../../../Cpp/JUCE/modules" -O0 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_medianfiltering_MedianFiltering" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/medianfiltering/MedianFiltering\" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2 -landroid -lEGL
else
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -I "../../JuceLibraryCode" -I "../../../../Cpp/JUCE/modules" -O3 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_medianfiltering_MedianFiltering" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/medianfiltering/MedianFiltering\" -D "NDEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2 -landroid -lEGL
  LOCAL_CFLAGS += -fsigned-char -fexceptions -frtti -I "../../JuceLibraryCode" -I "../../../../Cpp/JUCE/modules" -O3 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_medianfiltering_MedianFiltering" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/medianfiltering/MedianFiltering\" -D "NDEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2 -landroid -lEGL
endif

include $(BUILD_SHARED_LIBRARY)
