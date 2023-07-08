#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

// Transformation flags
// Since each flag is stored as a single bit, they need to be shifted to be in the proper positions
// https://dietertack.medium.com/using-bit-flags-in-c-d39ec6e30f08
enum DEBUG_FLAGS
{
    DRAW_PHYSICS_BOUNDS = 1 << 0, // 1
    DRAW_SPRITE_BOUNDS = 1 << 1,  // 2
    // SCL_UNIQUE = 1 << 2  // 4
};

namespace Astrolib
{
    class Debug
    {
    public:
        static void SetDebugFlag(DEBUG_FLAGS flag)
        {
            debugFlags |= (int)flag;
        };
        static void UnsetDebugFlag(DEBUG_FLAGS flag)
        {
            debugFlags &= ~(int)flag;
        };
        static bool IsDebugFlagSet(DEBUG_FLAGS flag)
        {
            return (debugFlags & (int)flag) == (int)flag;
        };

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
        static inline uint8_t debugFlags = 0;
    };

};
#endif // __DEBUG_H__