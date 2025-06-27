#include "Graphics/Interop/Interop.hpp"
#include "MemoryObject.hpp"
GL::MemoryObject::MemoryObject()
{
    glCreateMemoryObjectsEXT(1, &Handle);
}

GL::MemoryObject::~MemoryObject()
{
    glDeleteMemoryObjectsEXT(1, &Handle);
}

void GL::MemoryObject::ImportMemory(InteropTransaction transaction)
{
    Size = transaction.AllocationSize;
    glImportMemoryFdEXT(Handle, Size, GL_HANDLE_TYPE_OPAQUE_FD_EXT, transaction.fdHandle);
}