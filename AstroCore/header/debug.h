#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace Astrolib
{
    class Debug
    {
    public:
        inline static void SetFileName(std::string newFileName)
        {
            if (outputFile.is_open())
            {
                outputFile.close();
            }

            fileName = newFileName;
            outputFile.open(GetLogPath().c_str());
        }

        inline static void SetFileLocation(std::string newFileLocation)
        {
            if (outputFile.is_open())
            {
                outputFile.close();
            }
            // Escape '\' characters so the stream is properly opened
            // std::replace(newFileLocation.begin(), newFileLocation.end(), '\\', '\\\\');
            pathToLog = newFileLocation;
            outputFile.open(GetLogPath().c_str());
        }

        /// @brief Log a new line to the debug log
        /// @param newLogItem
        inline static void Log(std::string newLogItem)
        {
            if (!outputFile.is_open())
            {
                outputFile.open(GetLogPath().c_str());
            }

            // Write to output stream
            newLogItem += '\n';
            outputFile << newLogItem.c_str();
            // Flush output buffer to the file
            outputFile.flush();
        }

        /// @brief Get the path to the current log file
        /// @return The (relative) path to the current log file's directory
        inline static std::string GetLogPath()
        {
            return pathToLog + fileName + fileExtension;
        }

        /// @brief Get the debug instance (singleton)
        /// @return A pointer to the instance of the debug class
        inline static Debug *instance()
        {
            static Debug *instance = new Debug();
            return instance;
        };

        ~Debug()
        {
            if (outputFile.is_open())
            {
                outputFile.close();
            }
        };

    private:
        Debug()
        {
            outputFile.open(GetLogPath().c_str());
            std::string path = GetLogPath();
        };

        static inline std::string fileName = "debug_log";
        static inline std::string pathToLog = "";
        static inline std::ofstream outputFile;
        static inline const std::string fileExtension = ".txt";
    };

};
#endif // __DEBUG_H__