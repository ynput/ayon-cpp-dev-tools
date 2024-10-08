#ifndef AYONCPPAPIINSTRUMENTOR_H
#define AYONCPPAPIINSTRUMENTOR_H

#include <unistd.h>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <mutex>
#include <thread>
#include <future>
#include <vector>

// TODO implement Event Types (ph) for async event display
struct ProfileResult {
        std::string Name;
        std::string cat;   // categories
                           // std::string ph; //TODO not supported yet
        long long StartTime, EndTime;
        uint32_t ThreadID, PID;
        std::map<std::string, std::string> args;
};

// TODO test if an writer thread allows to release the main thread faster
class Instrumentor {
    private:
        std::ofstream m_OutputStream;
        int m_ProfileCount;
        std::mutex m_Mutex;
        std::string m_filePath;
        std::vector<std::future<void>> m_WriteQue;

        void
        WriteHeader() {
            m_OutputStream << "{\"otherData\": {},\"displayTimeUnit\": \"ns\" ,\"traceEvents\":[";
            m_OutputStream.flush();
        }

        void
        WriteFooter() {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        void
        WriteProfile(const ProfileResult &result) {
            std::lock_guard<std::mutex> lock(m_Mutex);
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_OutputStream << "{";

            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"cat\":\"" << result.cat << "\",";
            m_OutputStream << "\"dur\":" << (result.EndTime - result.StartTime) << ',';
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":" << result.PID << ",";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.StartTime << ",";
            m_OutputStream << "\"args\":{";
            for (auto &arg: result.args) {
                m_OutputStream << "\"" << arg.first << "\":\"" << arg.second << "\",";
            };

            m_OutputStream << "}}";
            m_OutputStream.flush();
        }

        Instrumentor(): m_ProfileCount(0) {
        }

        void
        NewSessoin(const std::string &filepath) {
            if (filepath.substr(filepath.find_last_of('.') + 1) != "json") {
                throw std::invalid_argument("File needs to be JSON: " + filepath);
            }
            m_filePath = std::filesystem::absolute(filepath);

            std::lock_guard<std::mutex> lock(m_Mutex);
            if (m_OutputStream.is_open()) {
                m_OutputStream.close();
            }
            m_OutputStream.open(m_filePath);
            this->WriteHeader();
        }

    public:
        static Instrumentor &
        Get() {
            static Instrumentor instance;
            return instance;
        }

        std::string
        GetOutputFile() {
            return m_filePath;
        }

        ~Instrumentor() {
            this->EndSession();
        }

        std::string
        BeginSession(const std::string &filepath) {
            this->NewSessoin(filepath);
            return m_filePath;
        }

        std::string
        BeginSession() {
            std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch());
            std::ostringstream oss;
            oss << ms.count();
            std::string filePath = std::filesystem::temp_directory_path()
                                   / (std::string("AyonInstrumentorTrace_") + oss.str() + std::string(".json"));

            this->NewSessoin(filePath);
            return m_filePath;
        }
        // TODO ensure that we actually wait on all writes
        void
        EndSession() {
            this->WriteFooter();
            m_OutputStream.close();
            m_ProfileCount = 0;
        }
        // TODO find a good way to release the thread faster. (consider future array)
        void
        WriteProfileAsync(const ProfileResult &result) {
            std::future<void> async = std::async(std::launch::async, [this, result]() { this->WriteProfile(result); });
        }
};

class InstrumentationTimer {
    public:
        InstrumentationTimer(const char* name, const char* cat, const std::map<std::string, std::string> &args):
            m_Name(name),
            m_Cat(cat),
            m_args(args),
            m_Stopped(false) {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer() {
            if (!m_Stopped)
                Stop();
        }

        void
        Stop() {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start
                = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end
                = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            uint32_t pidID = std::hash<uint32_t>{}(getpid());

            Instrumentor::Get().WriteProfileAsync({m_Name, m_Cat, start, end, threadID, pidID, m_args});

            m_Stopped = true;
        }

    private:
        const char* m_Name;
        const char* m_Cat;
        std::map<std::string, std::string> m_args;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
};

#endif   // !AYONCPPAPIINSTRUMENTOR_H
