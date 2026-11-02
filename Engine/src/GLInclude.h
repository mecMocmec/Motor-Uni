#pragma once

#if __has_include(<glad/gl.h>)
    #include <glad/gl.h>
    #define ENGINE_GLAD_V2 1
#else
    #include <glad/glad.h>
    #define ENGINE_GLAD_V2 0
#endif