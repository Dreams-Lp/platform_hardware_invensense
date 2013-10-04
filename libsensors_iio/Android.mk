# Copyright (C) 2008 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# Modified 2011 by InvenSense, Inc

LOCAL_PATH := $(call my-dir)

# InvenSense fragment of the HAL
include $(CLEAR_VARS)

LOCAL_MODULE := libinvensense_hal

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := invensense

LOCAL_CFLAGS := -DLOG_TAG=\"Sensors\"
# Comment out for ICS. Affects Android LOG macros.
LOCAL_CFLAGS += -DANDROID_JELLYBEAN

ifeq ($(ENG_BUILD),1)
ifeq ($(COMPILE_INVENSENSE_COMPASS_CAL),1)
LOCAL_CFLAGS += -DINVENSENSE_COMPASS_CAL
endif
ifeq ($(COMPILE_THIRD_PARTY_ACCEL),1)
LOCAL_CFLAGS += -DTHIRD_PARTY_ACCEL
endif
ifeq ($(COMPILE_COMPASS_YAS53x),1)
LOCAL_CFLAGS += -DCOMPASS_YAS53x
endif
ifeq ($(COMPILE_COMPASS_AK8975),1)
LOCAL_CFLAGS += -DCOMPASS_AK8975
endif
ifeq ($(COMPILE_COMPASS_AMI306),1)
LOCAL_CFLAGS += -DCOMPASS_AMI306
endif
else # release builds, default
LOCAL_CFLAGS += -DINVENSENSE_COMPASS_CAL
endif

LOCAL_SRC_FILES := SensorBase.cpp
LOCAL_SRC_FILES += MPLSensor.cpp
LOCAL_SRC_FILES += MPLSupport.cpp
LOCAL_SRC_FILES += InputEventReader.cpp
LOCAL_SRC_FILES += CompassSensor.IIO.9150.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/software/core/mllite
LOCAL_C_INCLUDES += $(LOCAL_PATH)/software/core/mllite/linux
LOCAL_C_INCLUDES += $(LOCAL_PATH)/software/core/driver/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/software/core/driver/include/linux

LOCAL_SHARED_LIBRARIES := liblog
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libmllite

# Additions for SysPed
LOCAL_SHARED_LIBRARIES += libmplmpu
LOCAL_C_INCLUDES += $(LOCAL_PATH)/software/core/mpl
LOCAL_CPPFLAGS += -DLINUX=1
LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libmplmpu
LOCAL_SRC_FILES := libmplmpu.so
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := invensense
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT)/lib
OVERRIDE_BUILT_MODULE_PATH := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)
include $(BUILD_PREBUILT)

# Build libmllite from source

MLLITE_DIR := $(LOCAL_PATH)/software/core/mllite

libmllite_src_files := \
    $(MLLITE_DIR)/data_builder.c \
    $(MLLITE_DIR)/hal_outputs.c \
    $(MLLITE_DIR)/message_layer.c \
    $(MLLITE_DIR)/ml_math_func.c \
    $(MLLITE_DIR)/mpl.c \
    $(MLLITE_DIR)/results_holder.c \
    $(MLLITE_DIR)/start_manager.c \
    $(MLLITE_DIR)/storage_manager.c \
    $(MLLITE_DIR)/linux/mlos_linux.c \
    $(MLLITE_DIR)/linux/ml_stored_data.c \
    $(MLLITE_DIR)/linux/ml_load_dmp.c \
    $(MLLITE_DIR)/linux/ml_sysfs_helper.c

libmllite_includes := \
    $(MLLITE_DIR) \
    $(MLLITE_DIR)/linux

libmllite_cflags := \
    -DNDEBUG \
    -D_REENTRANT \
    -DANDROID \
    -DANDROID_JELLYBEAN \
    -DLINUX

include $(CLEAR_VARS)
LOCAL_MODULE := libmllite
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := invensense
LOCAL_C_INCLUDES := $(libmllite_includes)
LOCAL_CFLAGS += $(libmllite_cflags)
LOCAL_SRC_FILES := $(libmllite_cflags)
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libmllite
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_OWNER := invensense
LOCAL_C_INCLUDES := $(libmllite_includes)
LOCAL_CFLAGS += $(libmllite_cflags)
LOCAL_SRC_FILES := $(libmllite_cflags)
include $(BUILD_STATIC_LIBRARY)
