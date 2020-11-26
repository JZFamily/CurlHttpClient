#include "utility.h"

bool execCmd(const std::string& cmd, std::string retStr)
{
#if WIN32

	system(cmd.c_str());
#else
	char buf[512] = { 0 };
	FILE *pFile = popen(cmd.c_tr(), "r");
	if (pFile == nullptr)
	{
		return false;
	}
	while (fgets(buf, sizeof(buf), pFile) != nullptr)
	{
		retStr.append(buf);
	}
	std::cerr << retStr << std::endl;
	pclose(pFile);
#endif // WIN32
	return true;
}

//更改工作目录和获取工作目录
//chdir()
//getcwd()
bool getExePath(std::string& exePath)
{
	const size_t  MAX_PATH_LENGTH = 260;
	char bufferFileName[MAX_PATH_LENGTH] = { 0 };
#if WIN32

	DWORD ret = GetModuleFileNameA(nullptr, bufferFileName, MAX_PATH_LENGTH);
	if (ret <= 0 || ret >= MAX_PATH_LENGTH)
	{
		return false;
	}
#else
	ssize_t ret = readlink("/proc/self/exe", bufferFileName, MAX_PATH_LENGTH);
	if (ret <= 0 || ret >= MAX_PATH_LENGTH)
	{
		return false;
	}
#endif // WIN32
	exePath = bufferFileName;
	std::cerr << exePath << std::endl;
	return true;
}
//std::string&& getCurrentTimeStamp()
//{
//	auto tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
//	time_t tt = std::chrono::system_clock::to_time_t(tp);
//	struct tm * tm = localtime(&tt);
//	std::stringstream ss;
//	ss << std::setiosflags(std::ios::right) << std::setw(4) << tm->tm_year + 1900 << "-" 
//		<< std::setfill('0') << std::setw(2)  << tm->tm_mon+1 << "-" << tm->tm_mday << " "
//		<< std::setw(2) << tm->tm_hour <<":" << std::setw(2) << tm->tm_min << ":"<< std::setw(2) << tm->tm_sec
//		<< "." << tp.time_since_epoch().count()%1000;
//	std::cerr << ss.str()  << std::endl;
//	return ss.str();
//}

//返回的时间戳无法当作文件名无论是2020-11-01 23:36:12.111 还是2020-11-01-23:36:12.111
std::string getCurrentTimeStamp()
{
	using format = boost::format;
	auto tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	time_t tt = std::chrono::system_clock::to_time_t(tp);
	struct tm * tm = localtime(&tt);
	format ft("%1$04d-%2$02d-%3$02d %4$02d:%5$02d:%6$02d.%7$03d");
	std::cerr << boost::str(ft
		% (tm->tm_year + 1900)
		% (tm->tm_mon + 1)
		% tm->tm_mday
		% tm->tm_hour
		% tm->tm_min
		% tm->tm_sec
		% (tp.time_since_epoch().count() % 1000)) << std::endl;
	return std::move(boost::str(ft
		% (tm->tm_year + 1900)
		% (tm->tm_mon + 1)
		% tm->tm_mday
		% tm->tm_hour
		% tm->tm_min
		% tm->tm_sec
		% (tp.time_since_epoch().count() % 1000)));
}

std::string getExecPath()
{
	std::string exePath;
	if (!getExePath(exePath))
	{
		return 0;
	}
	using Path = boost::filesystem::path;
	Path  path(exePath);
	std::cerr << path.parent_path().string<std::string>() << std::endl;
	return path.parent_path().string<std::string>();
}