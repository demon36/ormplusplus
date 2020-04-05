#include <logger.h>
#include <iomanip>
#include <chrono>

using namespace std;

#define DEFAULT_LOG_FILE "ormplusplus.log"

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

namespace utils {
	fstream logger::log_file;
	bool logger::is_handle_open = false;
	logger::lvl logger::m_level = lvl::WARNING;
	mutex logger::log_lock;
	//TODO: make thread safe
	void logger::use_file(const string& file_name){
		if(is_handle_open){
			log_file.close();
			is_handle_open = false;
			log_file.open(file_name, ios::out);
			is_handle_open = true;
		}else{
			log_file.open(file_name, ios::out | ios::app);
			is_handle_open = true;
		}
	}

	void logger::set_level(lvl value){
		m_level = value;
	}

	void logger::log(const std::string& module, const std::string& fn, int line, lvl level, const std::string& msg){
		if(level < m_level){
			return;
		}

		if(!is_handle_open){
			log_file.open(DEFAULT_LOG_FILE, ios::out | ios::app);
			is_handle_open = true;
		}

		string relative_fname = module;//probably not a generic soln here
		size_t slash_index = relative_fname.rfind(PATH_SEPARATOR);
		if(slash_index != relative_fname.npos){
			relative_fname = relative_fname.substr(slash_index+1);
		}

	    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	    char date_buffer[100] = {0};
	    std::strftime(date_buffer, sizeof(date_buffer), "%F %X", std::localtime(&now));

	    log_lock.lock();
		log_file << date_buffer << " | "<< left;
		log_file << setw(7) << level_to_string(level) << " | ";
		log_file << left << setw(30) << relative_fname + ":" + to_string(line) << " | ";
		log_file << left << setw(20) << fn << " | ";
		log_file << msg << endl;
		log_lock.unlock();
	}

	std::string logger::level_to_string(lvl level){
		switch(level){
			case lvl::DBUG:
				return "DEBUG";
				break;
			case lvl::INFO:
				return "INFO";
				break;
			case lvl::WARNING:
				return "WARNING";
				break;
			case lvl::ERROR:
				return "ERROR";
				break;
			default:
				throw new runtime_error("bad level provided at logger::level_to_string");
				break;
		}
	}

} /* namespace ormplusplus */
