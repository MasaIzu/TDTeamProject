#pragma once
//ワーニング消す

#define MY_PRAGMA(x)				__pragma(x)

#define MY_SUPPRESS_WARNING_PUSH MY_PRAGMA(warning(push))

#define MY_SUPPRESS_WARNING_POP MY_PRAGMA(warning(pop))

#define MY_WARNING(w) MY_PRAGMA(warning(disable: w))

#define MY_SUPPRESS_WARNING
MY_WARNING(4061)				\
MY_WARNING(4062)				\
MY_WARNING(4127)				\
MY_WARNING(4263)				\
MY_WARNING(4264)				\
MY_WARNING(4266)				\
MY_WARNING(4265)				\
MY_WARNING(4365)				\
MY_WARNING(4514)				\
MY_WARNING(4619)				\
MY_WARNING(4623)				\
MY_WARNING(4625)				\
MY_WARNING(4626)				\
MY_WARNING(4668)				\
MY_WARNING(4702)				\
MY_WARNING(4710)				\
MY_WARNING(4711)				\
MY_WARNING(4777)				\
MY_WARNING(4820)				\
MY_WARNING(5027)				\
MY_WARNING(5039)				\
MY_WARNING(5054)				\
MY_WARNING(5204)				\
MY_WARNING(5219)				\
MY_WARNING(5220)				\
MY_WARNING(5246)				\
MY_WARNING(5266)				\
MY_WARNING(5267)
#define MY_SUPPRESS_WARNINGS_BEGIN
MY_SUPPRESS_WARNING_PUSH
MY_SUPPRESS_WARNING

#define MY_SUPPRESS_WARNINGS_END
MY_SUPPRESS_WARNING_POP

#define MY_UTILITY_NAMESPACE_BEGIN
namespace MyUtility
{


#define MY_UTILITY_NAMESPACE_END
}
