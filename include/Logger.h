#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <string>
#include <memory>
#include <fstream>

#define ORMLOG(level, message) Logger::log(__FILE__, level, message)
#define DEFAULT_LOG_FILE "ormplusplus.log"

namespace ORMPlusPlus {

class Logger {
public:
	enum class Lv{
		DBUG,
		INFO,
		WARNING,
		ERROR,
		OFF,
	};

	static void useFile(const std::string& fileName);
	static void setLevel(Lv level);
	static void log(std::string module, Lv level, const std::string& msg);
	static std::string levelToString(Lv level);

private:
	static std::fstream logFile;
	static bool isHandleOpen;
	static Lv m_level;
};

} /* namespace ORMPlusPlus */

#endif /* INCLUDE_LOGGER_H_ */
