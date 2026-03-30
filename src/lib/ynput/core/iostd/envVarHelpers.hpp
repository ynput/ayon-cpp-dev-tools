#ifndef YNPUT_ENV_VAR_HELPER
#define YNPUT_ENV_VAR_HELPER

#include "../../../../NameSpaceDef/namespaces.hpp"
#include "../../lib/logging/AyonLogger.hpp"
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
    auto & logger = AyonLogger::getInstance();
    static const std::string kLogKeyName = "getEnvKey";
    static const bool kRegistered = logger.registerLoggingKey(kLogKeyName);
    (void)kRegistered;

    auto logKey = logger.key(kLogKeyName);
    const char* charEnvKey = std::getenv(envKey.c_str());
    if (charEnvKey != nullptr) {
        std::string strEnvKey(charEnvKey);
        logger.info(logKey, "Loaded environment key '{}'", envKey);
        return strEnvKey;
    }
    logger.warn(logKey, "Environment key '{}' was not found", envKey);
    return "";
};

/**
 * @brief cleans an given environment key from any hidden characters line brakes and similar
 *
 * @param dirtyKey input key
 * @return will return a clean Env Key
 */
std::string
cleanEnvKey(std::string &dirtyKey) {
    auto & logger = AyonLogger::getInstance();
    static const std::string kLogKeyName = "cleanEnvKey";
    static const bool kRegistered = logger.registerLoggingKey(kLogKeyName);
    (void)kRegistered;

    auto logKey = logger.key(kLogKeyName);
    auto start = dirtyKey.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        logger.warn(logKey, "Environment key value was empty after cleanup");
        return "";
    }
    auto end = dirtyKey.find_last_not_of(" \t\n\r\f\v");

    logger.info(logKey, "Cleaned environment key value");
    return dirtyKey.substr(start, end - start + 1);
};

/**
 * @brief splits a given string string via a given delimiter. useful for splitting Env keys that should be used as a
 * vector or an Map
 *
 * @param str input String to be split
 * @param delimiter delimiter used to split the string
 * @return vector of elements resulting from splitting the string by the delimiter. the delimiter will be removed
 */
std::vector<std::string>
split(const std::string &str, char delimiter) {
    auto & logger = AyonLogger::getInstance();
    static const std::string kLogKeyName = "splitEnvValue";
    static const bool kRegistered = logger.registerLoggingKey(kLogKeyName);
    (void)kRegistered;

    auto logKey = logger.key(kLogKeyName);
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    logger.info(logKey, "Split environment value into {} token(s)", tokens.size());
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
    auto & logger = AyonLogger::getInstance();
    static const std::string kLogKeyName = "getEnvArray";
    static const bool kRegistered = logger.registerLoggingKey(kLogKeyName);
    (void)kRegistered;

    auto logKey = logger.key(kLogKeyName);
    std::string envKeyVal = getEnvKey(envKey);
    std::vector<std::string> arrayItems;
    if (envKeyVal.empty()) {
        logger.warn(logKey, "Environment array key '{}' was empty", envKey);
        return arrayItems;
    }
    arrayItems = split(envKeyVal, ',');

    for (std::string &dirtyItem: arrayItems) {
        dirtyItem = cleanEnvKey(dirtyItem);
    }
    logger.info(logKey, "Loaded environment array '{}' with {} item(s)", envKey, arrayItems.size());
    return arrayItems;
};

/**
 * @brief split an environment key into a std::map
 * delimiter = `,` key value definition = {key}={value}
 *
 * @param envKey
 * @return
 */
std::map<std::string, std::string>
getEnvMap(const std::string &envKey) {
    auto & logger = AyonLogger::getInstance();
    static const std::string kLogKeyName = "getEnvMap";
    static const bool kRegistered = logger.registerLoggingKey(kLogKeyName);
    (void)kRegistered;

    auto logKey = logger.key(kLogKeyName);
    std::string envKeyVal = getEnvKey(envKey);
    std::map<std::string, std::string> envMap;
    if (envKeyVal.empty()) {
        logger.warn(logKey, "Environment map key '{}' was empty", envKey);
        return envMap;
    }
    std::vector<std::string> dirtyArrayItems = split(envKeyVal, ',');

    for (std::string &dirtyItem: dirtyArrayItems) {
        // Split on first '=' only — colon ':' breaks Windows drive letters (e.g. work=W:)
        auto eqPos = dirtyItem.find('=');
        if (eqPos != std::string::npos) {
            std::string key = dirtyItem.substr(0, eqPos);
            std::string val = dirtyItem.substr(eqPos + 1);
            envMap.emplace(std::make_pair(cleanEnvKey(key), cleanEnvKey(val)));
        }
    }
    logger.info(logKey, "Loaded environment map '{}' with {} item(s)", envKey, envMap.size());
    return envMap;
};
YNPUT_CORE_IOSTD_NAMESPACE_CLOSE
#endif   // !YNPUT_ENV_VAR_HELPER
