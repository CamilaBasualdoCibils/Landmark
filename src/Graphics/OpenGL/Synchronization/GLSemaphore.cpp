#include "GLSemaphore.hpp"
#include "Graphics/Interop/Interop.hpp"


GL::Semaphore::Semaphore()
{
 
glGenSemaphoresEXT(1, &Handle);

}
GL::Semaphore::~Semaphore(){

}

void GL::Semaphore::ImportMemory(InteropTransaction transaction)
{
    glImportSemaphoreFdEXT(Handle, GL_HANDLE_TYPE_OPAQUE_FD_EXT, transaction.fdHandle);
}