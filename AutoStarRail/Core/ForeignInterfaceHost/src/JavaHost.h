#ifndef ASR_CORE_FOREIGNINTERFACEHOST_JAVAHOST_H
#define ASR_CORE_FOREIGNINTERFACEHOST_JAVAHOST_H

#include <filesystem>
#ifdef ASR_EXPORT_JAVA

#include <AutoStarRail/Core/ForeignInterfaceHost/Config.h>
#include <AutoStarRail/Core/ForeignInterfaceHost/IForeignLanguageRuntime.h>
#include <memory>


#define ASR_NS_JAVAHOST_BEGIN                                                 \
    namespace JavaHost                                                         \
    {

#define ASR_NS_JAVAHOST_END }

ASR_CORE_FOREIGNINTERFACEHOST_NS_BEGIN

ASR_NS_JAVAHOST_BEGIN

struct JavaRuntimeDesc : public ForeignLanguageRuntimeFactoryDesc
{
    std::filesystem::path jvm_dll_path;
};

ASR_NS_JAVAHOST_END

ASR_CORE_FOREIGNINTERFACEHOST_NS_END

#endif // ASR_EXPORT_JAVA

#endif // ASR_CORE_FOREIGNINTERFACEHOST_JAVAHOST_H
