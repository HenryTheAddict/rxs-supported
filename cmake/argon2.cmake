if (WITH_ARGON2)
    # RandomX requires argon2 as an internal KDF - build the library only, no mining algo
    add_subdirectory(src/3rdparty/argon2)
    set(ARGON2_LIBRARY argon2)
else()
    set(ARGON2_LIBRARY "")
endif()
