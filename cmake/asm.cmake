# Assembly support - RandomX uses its own JIT compiler, no separate asm library needed.
set(XMRIG_ASM_SOURCES "")
set(XMRIG_ASM_LIBRARY "")

if (WITH_ASM AND NOT XMRIG_ARM AND NOT XMRIG_RISCV AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_definitions(/DXMRIG_FEATURE_ASM)
    set(XMRIG_ASM_SOURCES
        src/crypto/common/Assembly.h
        src/crypto/common/Assembly.cpp
    )
else()
    remove_definitions(/DXMRIG_FEATURE_ASM)
endif()
