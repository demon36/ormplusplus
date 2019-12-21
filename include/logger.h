#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <string>
#include <memory>
#include <fstream>

#define ORMLOG(level, message) logger::log(__FILE__, level, message)
#define DEFAULT_LOG_FILE "ormplusplus.log"

namespace ormplusplus {

class logger {
public:
	enum class lvl{
		DBUG,
		INFO,
		WARNING,
		ERROR,
		OFF,
	};

	static void use_file(const std::string& file_name);
	static void set_level(lvl value);
	static void log(std::string module, lvl level, const std::string& msg);
	static std::string level_to_string(lvl level);

private:
	static std::fstream log_file;
	static bool is_handle_open;//todo:can't replace with a stream check ?
	static lvl m_level;
};

} /* namespace ormplusplus */

#endif /* INCLUDE_LOGGER_H_ */
