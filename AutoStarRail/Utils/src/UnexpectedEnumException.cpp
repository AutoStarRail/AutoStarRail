#include <AutoStarRail/Utils/UnexpectedEnumException.h>
#include <AutoStarRail/Utils/fmt.h>
#include <magic_enum_format.hpp>

ASR_UTILS_NS_BEGIN

UnexpectedEnumException::UnexpectedEnumException(
    const std::string_view u8_enum_value)
    : Base{ASR::fmt::format("Unexpected enum found. Value = {}", u8_enum_value)}
{
}

UnexpectedEnumException::UnexpectedEnumException(
    std::int64_t enum_value)
    : Base{ASR::fmt::format("Unexpected enum found. Value = {}", enum_value)}
{
}

UnexpectedEnumException::UnexpectedEnumException(std::uint64_t enum_value)
    : Base{ASR::fmt::format("Unexpected enum found. Value = {}", enum_value)}
{
}

ASR_UTILS_NS_END
