if (BUILD_STATIC AND RXS_OS_UNIX AND WITH_OPENCL)
    message(WARNING "OpenCL backend is not compatible with static build, use -DWITH_OPENCL=OFF to suppress this warning")

    set(WITH_OPENCL OFF)
endif()

if (WITH_OPENCL)
    add_definitions(/DRXS_FEATURE_OPENCL /DCL_USE_DEPRECATED_OPENCL_1_2_APIS)

    set(HEADERS_BACKEND_OPENCL
        src/backend/opencl/cl/OclSource.h
        src/backend/opencl/interfaces/IOclRunner.h
        src/backend/opencl/OclBackend.h
        src/backend/opencl/OclCache.h
        src/backend/opencl/OclConfig.h
        src/backend/opencl/OclConfig_gen.h
        src/backend/opencl/OclGenerator.h
        src/backend/opencl/OclLaunchData.h
        src/backend/opencl/OclThread.h
        src/backend/opencl/OclThreads.h
        src/backend/opencl/OclWorker.h
        src/backend/opencl/runners/OclBaseRunner.h
        src/backend/opencl/runners/tools/OclSharedData.h
        src/backend/opencl/runners/tools/OclSharedState.h
        src/backend/opencl/wrappers/OclContext.h
        src/backend/opencl/wrappers/OclDevice.h
        src/backend/opencl/wrappers/OclError.h
        src/backend/opencl/wrappers/OclKernel.h
        src/backend/opencl/wrappers/OclLib.h
        src/backend/opencl/wrappers/OclPlatform.h
        src/backend/opencl/wrappers/OclVendor.h
        )

    set(SOURCES_BACKEND_OPENCL
        src/backend/opencl/cl/OclSource.cpp
        src/backend/opencl/OclBackend.cpp
        src/backend/opencl/OclCache.cpp
        src/backend/opencl/OclConfig.cpp
        src/backend/opencl/OclLaunchData.cpp
        src/backend/opencl/OclThread.cpp
        src/backend/opencl/OclThreads.cpp
        src/backend/opencl/OclWorker.cpp
        src/backend/opencl/runners/OclBaseRunner.cpp
        src/backend/opencl/runners/tools/OclSharedData.cpp
        src/backend/opencl/runners/tools/OclSharedState.cpp
        src/backend/opencl/wrappers/OclContext.cpp
        src/backend/opencl/wrappers/OclDevice.cpp
        src/backend/opencl/wrappers/OclError.cpp
        src/backend/opencl/wrappers/OclKernel.cpp
        src/backend/opencl/wrappers/OclLib.cpp
        src/backend/opencl/wrappers/OclPlatform.cpp
        )

    if (RXS_OS_APPLE)
        add_definitions(/DCL_TARGET_OPENCL_VERSION=120)
        list(APPEND SOURCES_BACKEND_OPENCL src/backend/opencl/wrappers/OclDevice_mac.cpp)
    elseif (WITH_OPENCL_VERSION)
        add_definitions(/DCL_TARGET_OPENCL_VERSION=${WITH_OPENCL_VERSION})
    endif()

    if (WIN32)
        list(APPEND SOURCES_BACKEND_OPENCL src/backend/opencl/OclCache_win.cpp)
    else()
        list(APPEND SOURCES_BACKEND_OPENCL src/backend/opencl/OclCache_unix.cpp)
    endif()
        list(APPEND HEADERS_BACKEND_OPENCL
             src/backend/opencl/kernels/rx/Blake2bHashRegistersKernel.h
             src/backend/opencl/kernels/rx/Blake2bInitialHashBigKernel.h
             src/backend/opencl/kernels/rx/Blake2bInitialHashDoubleKernel.h
             src/backend/opencl/kernels/rx/Blake2bInitialHashKernel.h
             src/backend/opencl/kernels/rx/ExecuteVmKernel.h
             src/backend/opencl/kernels/rx/FillAesKernel.h
             src/backend/opencl/kernels/rx/FindSharesKernel.h
             src/backend/opencl/kernels/rx/HashAesKernel.cpp
             src/backend/opencl/kernels/rx/InitVmKernel.h
             src/backend/opencl/kernels/rx/RxJitKernel.h
             src/backend/opencl/kernels/rx/RxRunKernel.h
             src/backend/opencl/runners/OclRxBaseRunner.h
             src/backend/opencl/runners/OclRxJitRunner.h
             src/backend/opencl/runners/OclRxVmRunner.h
             )

        list(APPEND SOURCES_BACKEND_OPENCL
             src/backend/opencl/generators/ocl_generic_rx_generator.cpp
             src/backend/opencl/kernels/rx/Blake2bHashRegistersKernel.cpp
             src/backend/opencl/kernels/rx/Blake2bInitialHashBigKernel.cpp
             src/backend/opencl/kernels/rx/Blake2bInitialHashDoubleKernel.cpp
             src/backend/opencl/kernels/rx/Blake2bInitialHashKernel.cpp
             src/backend/opencl/kernels/rx/ExecuteVmKernel.cpp
             src/backend/opencl/kernels/rx/FillAesKernel.cpp
             src/backend/opencl/kernels/rx/FindSharesKernel.cpp
             src/backend/opencl/kernels/rx/HashAesKernel.cpp
             src/backend/opencl/kernels/rx/InitVmKernel.cpp
             src/backend/opencl/kernels/rx/RxJitKernel.cpp
             src/backend/opencl/kernels/rx/RxRunKernel.cpp
             src/backend/opencl/runners/OclRxBaseRunner.cpp
             src/backend/opencl/runners/OclRxJitRunner.cpp
             src/backend/opencl/runners/OclRxVmRunner.cpp
             )
    if (WITH_STRICT_CACHE)
        add_definitions(/DRXS_STRICT_OPENCL_CACHE)
    else()
        remove_definitions(/DRXS_STRICT_OPENCL_CACHE)
    endif()

    if (WITH_INTERLEAVE_DEBUG_LOG)
        add_definitions(/DRXS_INTERLEAVE_DEBUG)
    endif()

    if (WITH_ADL AND (RXS_OS_WIN OR RXS_OS_LINUX))
        add_definitions(/DRXS_FEATURE_ADL)

        list(APPEND HEADERS_BACKEND_OPENCL
             src/backend/opencl/wrappers/AdlHealth.h
             src/backend/opencl/wrappers/AdlLib.h
             )

        if (RXS_OS_WIN)
            list(APPEND SOURCES_BACKEND_OPENCL src/backend/opencl/wrappers/AdlLib.cpp)
        else()
            list(APPEND SOURCES_BACKEND_OPENCL src/backend/opencl/wrappers/AdlLib_linux.cpp)
        endif()
    else()
       remove_definitions(/DRXS_FEATURE_ADL)
    endif()
else()
    remove_definitions(/DRXS_FEATURE_OPENCL)
    remove_definitions(/DRXS_FEATURE_ADL)

    set(HEADERS_BACKEND_OPENCL "")
    set(SOURCES_BACKEND_OPENCL "")
endif()
