#ifndef TOOL_AYON_ROT_HELPERS_DEF
#define TOOL_AYON_ROT_HELPERS_DEF

#include "../../../../NameSpaceDef/namespaces.hpp"
#include "../../lib/logging/AyonLogger.hpp"

#include <algorithm>
#include <regex>
#include <string>
#include <unordered_map>

/**
 * @brief allows to replace the {root[xxx]} key from an Ayon resolver endpoint with the local root overwrites
 *
 * @param rootLessPath a path that needs its root replaces e.g {root[work]}/file/path.ext
 * @param siteRoots a list of roots availalbe on the system e.g {work=/path/to, temp=/path/to}
 * @return input string with the correct root replace. the {root[key]} will be removed
 */
YNPUT_TOOL_AYON_NAMESPACE_OPEN

std::string
rootReplace(const std::string &rootLessPath, const std::unordered_map<std::string, std::string> &siteRoots) {
    auto & logger = AyonLogger::getInstance();
    static const std::string kLogKeyName = "rootReplace";
    static const bool kRegistered = logger.registerLoggingKey(kLogKeyName);
    (void)kRegistered;

    auto logKey = logger.key(kLogKeyName);

    std::string rootedPath;

    std::smatch matchea;
    std::regex rootFindPattern("\\{root\\[.*?\\]\\}");
    if (std::regex_search(rootLessPath, matchea, rootFindPattern)) {
        std::string siteRootOverwriteName = matchea.str(0);

        std::smatch matcheb;
        std::regex rootBracketPattern("\\[(.*?)\\]");
        if (std::regex_search(siteRootOverwriteName, matcheb, rootBracketPattern)) {
            std::string key = matcheb.str(1);
            try {
                std::string replacement = siteRoots.at(key);
                rootedPath = std::regex_replace(rootLessPath, rootFindPattern, replacement);
                std::replace(rootedPath.begin(), rootedPath.end(), '\\', '/');
                return rootedPath;
            }
            catch (std::out_of_range &e) {
                logger.warn(logKey, "Could not find site root '{}' for path '{}'", key, rootLessPath);
                return rootLessPath;
            }
        }
    }

    return rootLessPath;
};

YNPUT_TOOL_AYON_NAMESPACE_CLOSE
#endif   // !TOOL_AYON_ROT_HELPERS_DEF
