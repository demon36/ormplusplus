#include <logger.h>
#include <iomanip>
#include <chrono>

using namespace std;

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

namespace ormplusplus {
	fstream logger::log_file;
	bool logger::is_handle_open = false;
	logger::lvl logger::m_level = lvl::WARNING;
	//TODO: make thread safe
	//TODO: add rotation maybe
	void logger::use_file(const string& fileName){
		if(is_handle_open){
			log_file.close();
			is_handle_open = false;
			log_file.open(fileName, ios::out);
			is_handle_open = true;
		}else{
			log_file.open(fileName, ios::out | ios::app);
			is_handle_open = true;
		}
	}

	void logger::set_level(lvl value){
		m_level = value;
	}

	void logger::log(string module, lvl level, const string& msg){
		if(level < m_level){
			return;
		}
		if(!is_handle_open){
			log_file.open(DEFAULT_LOG_FILE, ios::out | ios::app);
			is_handle_open = true;
		}
		size_t slashIndex = module.rfind(PATH_SEPARATOR);
		if(slashIndex != module.npos){
			module = module.substr(slashIndex+1);
		}

	    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	    char dateBuffer[100] = {0};
	    std::strftime(dateBuffer, sizeof(dateBuffer), "%F %X", std::localtime(&now));

		log_file << dateBuffer << " | " << left << setw(7) << levelToString(level) << " | ";
		log_file << left << setw(20) << module << " | " << msg << endl;
	}

	std::string logger::levelToString(lvl level){
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
				throw new runtime_error("bad level provided at Logger::levelToString");
				break;
		}
	}

} /* namespace ORMPlusPlus */
