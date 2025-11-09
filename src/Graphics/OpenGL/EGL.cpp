#include "EGL.hpp"
#include "pch.h"

#define CHECK_EGL(call)                                                                                                \
    if (!(call))                                                                                                       \
    {                                                                                                                  \
        std::cerr << "EGL error at " << #call << ": 0x" << std::hex << eglGetError() << std::endl;                     \
        exit(1);                                                                                                       \
    }

GL::EGL::EGL()
{
    eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC)eglGetProcAddress("eglQueryDevicesEXT");
    eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
    eglQueryDeviceStringEXT = (PFNEGLQUERYDEVICESTRINGEXTPROC)eglGetProcAddress("eglQueryDeviceStringEXT");
    eglQueryDeviceAttribEXT = (PFNEGLQUERYDEVICEATTRIBEXTPROC)eglGetProcAddress("eglQueryDeviceAttribEXT");
    eglQueryDeviceBinaryEXT = (PFNEGLQUERYDEVICEBINARYEXTPROC)eglGetProcAddress("eglQueryDeviceBinaryEXT");
    LASSERT(eglQueryDevicesEXT && eglGetPlatformDisplayEXT && eglQueryDeviceStringEXT && eglQueryDeviceAttribEXT,
            "Required EGL extensions not present");

    std::array<EGLDeviceEXT, 64> DevicesEXT = {0};
    int32_t NumDevices;
    CHECK_EGL(eglQueryDevicesEXT(DevicesEXT.size(), DevicesEXT.data(), &NumDevices));
    AvailableDevices.resize(NumDevices);

    std::cout << "Found " << NumDevices << " EGL devices:\n";
    for (int i = 0; i < NumDevices; i++)
    {
        std::shared_ptr<Device> device = std::make_shared<Device>();
        AvailableDevices[i] = device;
        device->Handle = DevicesEXT[i];

        EGLDisplay display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, device->Handle, nullptr);
        if (display == EGL_NO_DISPLAY)
        {
            std::cerr << "Failed to get EGL display\n";
            continue;
        }
        const char* cDrmPath =eglQueryDeviceStringEXT(device->Handle, EGL_DRM_DEVICE_FILE_EXT);
        if (cDrmPath)
        {
            device->DRMPath = cDrmPath;
            std::cout << device->DRMPath << std::endl;
        }
        const char* cRenderNodePath = eglQueryDeviceStringEXT(device->Handle, EGL_DRM_RENDER_NODE_FILE_EXT);
        if (cRenderNodePath)
        {
            device->RenderNodePath = cRenderNodePath;
            std::cout << device->RenderNodePath << std::endl;


        }
        eglQueryDeviceBinaryEXT(device->Handle, EGL_DRIVER_UUID_EXT, device->DriverUUID.size(),
                                device->DriverUUID.data(), nullptr);
        eglQueryDeviceBinaryEXT(device->Handle, EGL_DEVICE_UUID_EXT, device->DriverUUID.size(),
                                device->DeviceUUID.data(), nullptr);
    }
}
void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam)
{
    std::cerr << "GL DEBUG: " << message << std::endl;
    LASSERT(severity == GL_DEBUG_SEVERITY_NOTIFICATION, message);
}
std::shared_ptr<GL::Context> GL::EGL::CreateContext(std::shared_ptr<Device> Device)
{
    std::shared_ptr<Context> EGLcontext = std::make_shared<Context>();
    EGLcontext->RenderingDevice = Device;
    EGLDisplay display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, Device->Handle, nullptr);
    if (display == EGL_NO_DISPLAY)
    {
        fprintf(stderr, "Failed to get EGLDisplay.\n");
    }
    EGLcontext->Display = display;
    EGLint major, minor;
    if (!eglInitialize(display, &major, &minor))
    {
        fprintf(stderr, "Failed to initialize EGL.\n");
    }
    printf("Initialized EGL %d.%d\n", major, minor);

    const EGLint configAttribs[] = {EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                                    // EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
                                    //  EGL_CONTEXT_MAJOR_VERSION_KHR, 4,
                                    //  EGL_CONTEXT_MINOR_VERSION_KHR, 5,
                                    //  EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
                                    EGL_NONE};

    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs == 0)
    {
        fprintf(stderr, "Failed to choose EGL config.\n");
    }
    const EGLint pbufferAttribs[] = {
        EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE,
    };
    EGLSurface surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
    if (surface == EGL_NO_SURFACE)
    {
        fprintf(stderr, "Failed to create EGL PBuffer surface.\n");
    }

    eglBindAPI(EGL_OPENGL_API);
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, nullptr);
    if (context == EGL_NO_CONTEXT)
    {
        fprintf(stderr, "Failed to create EGL context.\n");
    }

    // Step 7: Make context current (headless)
    if (!eglMakeCurrent(display, surface, surface, context))
    {
        fprintf(stderr, "Failed to make EGL context current.\n");
    }
    glewExperimental = true;
    const GLenum GlewStatus = glewInit();

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    std::cerr << "OpenGL Version: " << glGetString(GL_VERSION)
              << ". GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    ActiveContexts.push_back(EGLcontext);
    return EGLcontext;
}
