#ifndef TOOL_AYON_ROT_HELPERS_DEF
#define TOOL_AYON_ROT_HELPERS_DEF

#include <regex>
#include <string>
#include "../../../../NameSpaceDef/namespaces.hpp"

/**
 * @brief allows to replace the {root[xxx]} key from an Ayon resolver endpoint with the local root overwrites
 *
 * @param rootLessPath a path that needs its root replaces e.g {root[work]}/file/path.ext
 * @param siteRoots a list of roots availalbe on the system e.g {work=/path/to, temp=/path/to}
 * @return input string with the correct root replace. the {root[key]} will be removed
 */
YNPUT_TOOL_AYON_NAMESPACE_OPEN

// TODO after we implemented a singleton logger we will Re implement logging into this function
std::string
rootReplace(const std::string &rootLessPath, const std::unordered_map<std::string, std::string> &siteRoots) {
    std::string rootedPath;

    std::smatch matchea;
    std::regex rootFindPattern("\\{root\\[.*?\\]\\}");
    if (std::regex_search(rootLessPath, matchea, rootFindPattern)) {
        std::string siteRootOverwriteName = matchea.str(0);

        std::smatch matcheb;
        std::regex rootBraketPattern("\\[(.*?)\\]");
        if (std::regex_search(rootLessPath, matcheb, rootBraketPattern)) {
            std::string breakedString = matcheb.str(0);
            breakedString = breakedString.substr(1, breakedString.length() - 2);
            try {
                std::string replacement = siteRoots.at(breakedString);
                rootedPath = std::regex_replace(rootLessPath, rootFindPattern, replacement);
                return rootedPath;
            }
            catch (std::out_of_range &e) {
                return rootLessPath;
            }
        }
    }

    return rootLessPath;
};

YNPUT_TOOL_AYON_NAMESPACE_CLOSE
#endif   // !TOOL_AYON_ROT_HELPERS_DEF
