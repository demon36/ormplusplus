#include "Logger.h"

#include <iomanip>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>

using namespace std;
using namespace Poco;

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

namespace ORMPlusPlus {
	fstream Logger::logFile;
	bool Logger::isHandleOpen = false;
	Logger::Lv Logger::m_level = Lv::WARNING;
	//TODO: make thread safe
	//TODO: add rotation maybe
	void Logger::useFile(const string& fileName){
		if(isHandleOpen){
			logFile.close();
			isHandleOpen = false;
			logFile.open(fileName, ios::out);
			isHandleOpen = true;
		}else{
			logFile.open(fileName, ios::out | ios::app);
			isHandleOpen = true;
		}
	}

	void Logger::setLevel(Lv value){
		m_level = value;
	}

	void Logger::log(string module, Lv level, const string& msg){
		if(level < m_level){
			return;
		}
		if(!isHandleOpen){
			logFile.open(DEFAULT_LOG_FILE, ios::out | ios::app);
			isHandleOpen = true;
		}
		size_t slashIndex = module.rfind(PATH_SEPARATOR);
		if(slashIndex != module.npos){
			module = module.substr(slashIndex+1);
		}
		string dateString = Poco::DateTimeFormatter::format(DateTime(), "%Y-%m-%d %h:%M:%S");
		logFile << dateString << " | " << left << setw(7) << levelToString(level) << " | ";
		logFile << left << setw(20) << module << " | " << msg << endl;
	}

	std::string Logger::levelToString(Lv level){
		switch(level){
			case Lv::DBUG:
				return "DEBUG";
				break;
			case Lv::INFO:
				return "INFO";
				break;
			case Lv::WARNING:
				return "WARNING";
				break;
			case Lv::ERROR:
				return "ERROR";
				break;
			default:
				throw new runtime_error("bad level provided at Logger::levelToString");
				break;
		}
	}

} /* namespace ORMPlusPlus */
