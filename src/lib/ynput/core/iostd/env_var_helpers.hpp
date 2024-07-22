#ifndef YNPUT_ENV_VAR_HELPER
#define YNPUT_ENV_VAR_HELPER

#include "../../../../NameSpaceDef/namespaces.hpp"
#include <cstdlib>
#include <map>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief get the value of an environment key in string format
 *
 * @param envKey the name of the environment key
 * @return std::string env key value or empty std::string if envKey == nullptr
 */
YNPUT_CORE_IOSTD_NAMESPACE_OPEN
std::string
getEnvKey(const std::string &envKey) {
    const char* charEnvKey = std::getenv(envKey.c_str());
    if (charEnvKey != nullptr) {
        std::string strEnvKey(charEnvKey);
        return strEnvKey;
    }
    return "";
};

std::string
cleanEnvKey(std::string &dirtyKey) {
    auto start = dirtyKey.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return "";
    }
    auto end = dirtyKey.find_last_not_of(" \t\n\r\f\v");

    return dirtyKey.substr(start, end - start + 1);
};

std::vector<std::string>
split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

/**
 * @brief split an env key into an std vector.
 * array definition. Delimiter ","
 *
 * @param envKey
 */
std::vector<std::string>
getEnvArray(const std::string &envKey) {
    std::string envKeyVal = getEnvKey(envKey);
    std::vector<std::string> arrayItems;
    if (envKeyVal.empty()) {
        return arrayItems;
    }
    arrayItems = split(envKeyVal, ',');

    for (std::string &dirtyItem: arrayItems) {
        dirtyItem = cleanEnvKey(dirtyItem);
    }
    return arrayItems;
};

/**
 * @brief split an environment key into a std::map
 * delimiter = `,` key value definition = {key}:{value}
 *
 * @param envKey
 * @return
 */
std::map<std::string, std::string>
getEnvMap(const std::string &envKey) {
    std::string envKeyVal = getEnvKey(envKey);
    std::map<std::string, std::string> envMap;
    if (envKeyVal.empty()) {
        return envMap;
    }
    std::vector<std::string> dirtyArrayItems = split(envKeyVal, ',');

    for (std::string &dirtyItem: dirtyArrayItems) {
        std::vector<std::string> dirtySplitItems = split(dirtyItem, ':');
        envMap.emplace(std::make_pair(cleanEnvKey(dirtySplitItems.at(0)), cleanEnvKey(dirtySplitItems.at(1))));
    }
    return envMap;
};
YNPUT_CORE_IOSTD_NAMESPACE_CLOSE
#endif   // !YNPUT_ENV_VAR_HELPER
