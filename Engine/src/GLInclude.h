#pragma once

// glad v2 (glad/gl.h) o glad v1 (glad/glad.h): suportem els dos.
#if __has_include(<glad/gl.h>)
    #include <glad/gl.h>
    #define ENGINE_GLAD_V2 1
#else
    #include <glad/glad.h>
    #define ENGINE_GLAD_V2 0
#endif
