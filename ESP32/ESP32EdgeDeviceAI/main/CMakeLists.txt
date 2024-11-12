set(MODEL_FOLDER ..)
set(EI_SDK_FOLDER ../edge-impulse-sdk)

set(include_dirs
    ${MODEL_FOLDER}
    ${MODEL_FOLDER}/tflite-model
    ${MODEL_FOLDER}/model-parameters
    ${EI_SDK_FOLDER}
)

include(${EI_SDK_FOLDER}/cmake/utils.cmake)

RECURSIVE_FIND_FILE_EXCLUDE_DIR(SOURCE_FILES "../edge-impulse-sdk" "CMSIS" "*.cpp")
RECURSIVE_FIND_FILE_EXCLUDE_DIR(MODEL_FILES "../tflite-model" "CMSIS" "*.cpp")
RECURSIVE_FIND_FILE_EXCLUDE_DIR(CC_FILES "../edge-impulse-sdk" "CMSIS" "*.cc")
RECURSIVE_FIND_FILE_EXCLUDE_DIR(S_FILES "../edge-impulse-sdk" "CMSIS" "*.s")
RECURSIVE_FIND_FILE_EXCLUDE_DIR(C_FILES "../edge-impulse-sdk" "CMSIS" "*.c")

list(APPEND SOURCE_FILES ${S_FILES})
list(APPEND SOURCE_FILES ${C_FILES})
list(APPEND SOURCE_FILES ${CC_FILES})
list(APPEND SOURCE_FILES ${MODEL_FILES})

idf_component_register(SRCS "main.cpp" "${SOURCE_FILES}" "wifi.c"
                       INCLUDE_DIRS "${include_dirs}" ".")

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++11)

