#include "YVRDebug.h"
#include "../OpenXR/Common.h"

void CheckGLError(const char* file, int line)
{
    for (GLint error = glGetError(); error; error = glGetError())
    {
        char* pError;
        switch (error)
        {
            case GL_NO_ERROR:
                pError = (char*)"GL_NO_ERROR";
                break;
            case GL_INVALID_ENUM:
                pError = (char*)"GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                pError = (char*)"GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                pError = (char*)"GL_INVALID_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                pError = (char*)"GL_OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                pError = (char*)"GL_INVALID_FRAMEBUFFER_OPERATION";
                break;

            default:
                YError("glError (0x%x) %s:%d\n", error, file, line);
                return;
        }

        YError("glError (%s) %s:%d\n", pError, file, line);
        return;
    }
    return;
}

XrResult CheckXrResult(const XrResult res, const char* originator, const char* sourceLocation)
{
    if (XR_FAILED(res))
    {
        YError("OpenXR ERROR: [%s] in %s, at %s", to_string(res), originator, sourceLocation);
    }

    return res;
}

void CheckEGLError(const char *file, int line)
{
    EGLint error = eglGetError();
    if (error != EGL_SUCCESS) YError("Egl error happened %d, in %s:%d", error, file, line);
}
