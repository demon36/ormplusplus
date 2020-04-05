#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <string>
#include <memory>
#include <fstream>
#include <mutex>

#define ORMLOG(level, message) ::utils::logger::log(__FILE__, __FUNCTION__, __LINE__, level, message)

namespace utils {

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
	static void log(const std::string& module, const std::string& fn, int line, lvl level, const std::string& msg);
	static std::string level_to_string(lvl level);

private:
	static std::fstream log_file;
	static bool is_handle_open;//todo:can't replace with a stream check ?
	static lvl m_level;
	static std::mutex log_lock;
};

} /* namespace ormplusplus */

#endif /* INCLUDE_LOGGER_H_ */
