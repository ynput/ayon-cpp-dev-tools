#ifndef DEV_TOOLS_NAMESPACE_DEF
#define DEV_TOOLS_NAMESPACE_DEF
namespace ynput {

namespace core {
namespace utils {}
namespace config {}
namespace math {}
namespace time {}
namespace iostd {}
namespace fio {}
namespace threading {}
namespace dt {}
namespace cp {}
}   // namespace core

namespace lib {
namespace logging {}
}   // namespace lib

namespace dt {
namespace trace {}
}   // namespace dt

namespace tool {
namespace usd {}
}   // namespace tool

namespace app {}

}   // namespace ynput

#endif   // !DEV_TOOLS_NAMESPACE_DEF

#ifndef DEV_TOOLS_NAMESPACE_MACRO_DEF
#define DEV_TOOLS_NAMESPACE_MACRO_DEF

// Top Layer Namespace
#define YNPUT_NAMESPACE_USING_DIRECTIVE using namespace ynput;
#define YNPUT_NAMESPACE_OPEN            namespace ynput {
#define YNPUT_NAMESPACE_CLOSE           }

// Core Name Space
#define YNPUT_CORE_NAMESPACE_USING_DIRECTIVE           using namespace ynput::core;
#define YNPUT_CORE_NAMESPACE_OPEN                      namespace ynput::core {
#define YNPUT_CORE_NAMESPACE_CLOSE                     }

#define YNPUT_CORE_UTILS_NAMESPACE_USING_DIRECTIVE     using namespace ynput::core::utils;
#define YNPUT_CORE_UTILS_NAMESPACE_OPEN                namespace ynput::core::utils {
#define YNPUT_CORE_UTILS_NAMESPACE_CLOSE               }
#define YNPUT_CORE_CONF_NAMESPACE_USING_DIRECTIVE      using namespace ynput::core::config;
#define YNPUT_CORE_CONF_NAMESPACE_OPEN                 namespace ynput::core::config {
#define YNPUT_CORE_CONF_NAMESPACE_CLOSE                }
#define YNPUT_CORE_MATH_NAMESPACE_USING_DIRECTIVE      using namespace ynput::core::math;
#define YNPUT_CORE_MATH_NAMESPACE_OPEN                 namespace ynput::core::math {
#define YNPUT_CORE_MATH_NAMESPACE_CLOSE                }
#define YNPUT_CORE_TIME_NAMESPACE_USING_DIRECTIVE      using namespace ynput::core::time;
#define YNPUT_CORE_TIME_NAMESPACE_OPEN                 namespace ynput::core::time {
#define YNPUT_CORE_TIME_NAMESPACE_CLOSE                }
#define YNPUT_CORE_IOSTD_NAMESPACE_USING_DIRECTIVE     using namespace ynput::core::iostd;
#define YNPUT_CORE_IOSTD_NAMESPACE_OPEN                namespace ynput::core::iostd {
#define YNPUT_CORE_IOSTD_NAMESPACE_CLOSE               }
#define YNPUT_CORE_FIO_NAMESPACE_USING_DIRECTIVE       using namespace ynput::core::fio;
#define YNPUT_CORE_FIO_NAMESPACE_OPEN                  namespace ynput::core::fio {
#define YNPUT_CORE_FIO_NAMESPACE_CLOSE                 }
#define YNPUT_CORE_THREADING_NAMESPACE_USING_DIRECTIVE using namespace ynput::core::threading;
#define YNPUT_CORE_THREADING_NAMESPACE_OPEN            namespace ynput::core::threading {
#define YNPUT_CORE_THREADING_NAMESPACE_CLOSE           }
#define YNPUT_CORE_DT_NAMESPACE_USING_DIRECTIVE        using namespace ynput::core::dt;
#define YNPUT_CORE_DT_NAMESPACE_OPEN                   namespace ynput::core::dt {
#define YNPUT_CORE_DT_NAMESPACE_CLOSE                  }
#define YNPUT_CORE_CP_NAMESPACE_USING_DIRECTIVE        using namespace ynput::core::cp;
#define YNPUT_CORE_CP_NAMESPACE_OPEN                   namespace ynput::core::cp {
#define YNPUT_CORE_CP_NAMESPACE_CLOSE                  }

// lib namespace
#define YNPUT_LIB_NAMESPACE_USING_DIRECTIVE         using namespace ynput::lib;
#define YNPUT_LIB_NAMESPACE_OPEN                    namespace ynput::lib {
#define YNPUT_LIB_NAMESPACE_CLOSE                   }

#define YNPUT_LIB_LOGGING_NAMESPACE_USING_DIRECTIVE using namespace ynput::lib::logging;
#define YNPUT_LIB_LOGGING_NAMESPACE_OPEN            namespace ynput::lib::logging {
#define YNPUT_LIB_LOGGING_NAMESPACE_CLOSE           }

// dt namespace
#define YNPUT_DT_NAMESPACE_USING_DIRECTIVE       using namespace ynput::dt;
#define YNPUT_DT_NAMESPACE_OPEN                  namespace ynput::dt {
#define YNPUT_DT_NAMESPACE_CLOSE                 }

#define YNPUT_DT_TRACE_NAMESPACE_USING_DIRECTIVE using namespace ynput::dt::trace;
#define YNPUT_DT_TRACE_NAMESPACE_OPEN            namespace ynput::dt::trace {
#define YNPUT_DT_TRACE_NAMESPACE_CLOSE           }

// tool namespace
#define YNPUT_TOOL_NAMESPACE_USING_DIRECTIVE      using namespace ynput::tool;
#define YNPUT_TOOL_NAMESPACE_OPEN                 namespace ynput::tool {
#define YNPUT_TOOL_NAMESPACE_CLOSE                }

#define YNPUT_TOOL_USD_NAMESPACE_USING_DIRECTIVE  using namespace ynput::tool::usd;
#define YNPUT_TOOL_USD_NAMESPACE_OPEN             namespace ynput::tool::usd {
#define YNPUT_TOOL_USD_NAMESPACE_CLOSE            }

#define YNPUT_TOOL_AYON_NAMESPACE_USING_DIRECTIVE using namespace ynput::tool::ayon;
#define YNPUT_TOOL_AYON_NAMESPACE_OPEN            namespace ynput::tool::ayon {
#define YNPUT_TOOL_USD_NAMESPACE_CLOSE            }

// app namespace
#define YNPUT_APP_NAMESPACE_USING_DIRECTIVE using namespace ynput::app;
#define YNPUT_APP_NAMESPACE_OPEN            namespace ynput::app {
#define YNPUT_APP_NAMESPACE_CLOSE           }

#endif   // !DEV_TOOLS_NAMESPACE_MACRO_DEF
