#include "i_curl_http_client.h"
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include "boost/format.hpp"
#if WIN32
#include <Windows.h>
#else // linux
#include <unistd.h>
#endif // WIN32
#include "SQLiteCpp/SQLiteCpp.h"
#include "SQLiteCpp/VariadicBind.h"
#include "utility.h"

#include "boost/smart_ptr/shared_ptr.hpp"
#include "boost/core/null_deleter.hpp"
#include "boost/log/common.hpp"
#include "boost/log/exceptions.hpp"
#include "boost/log/core.hpp"
#include "boost/log/sinks.hpp"
#include "boost/log/expressions.hpp"
#include "boost/log/attributes.hpp"
#include "boost/log/trivial.hpp"
#include <boost/log/sinks/async_frontend.hpp>
#include <thread>
#include <chrono>
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;


//enum severity_level
//{
//	normal,
//	notification,
//	warning,
//	error,
//	critical
//};
//std::ostream &operator<<(std::ostream &strm, severity_level level)
//{
//	static const char *strings[] =
//	{
//		"normal",
//		"notification",
//		"warning",
//		"error",
//		"critical" };
//
//	if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings))
//		strm << strings[level];
//	else
//		strm << static_cast<int>(level);
//
//	return strm;
//}
//
//typedef sinks::asynchronous_sink<sinks::text_ostream_backend, sinks::bounded_fifo_queue<1000, sinks::drop_on_overflow>> sink_t;
//
//boost::shared_ptr<sink_t> init_logging()
//{
//	logging::add_common_attributes();
//	boost::shared_ptr<logging::core> core = logging::core::get();
//	boost::shared_ptr<sinks::text_ostream_backend> backend = boost::make_shared<sinks::text_ostream_backend>();
//	boost::shared_ptr<sink_t> sink(new sink_t(backend));
//	core->add_sink(sink);
//	core->add_global_attribute("ThreadID", attrs::current_thread_id());
//	core->add_global_attribute("Process", attrs::current_process_name());
//	sink->set_filter(expr::attr<severity_level>("Severity") >= warning);
//	sink->set_formatter(
//		expr::stream
//		<< "["
//		<< expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "]["
//		<< expr::attr<attrs::current_thread_id::value_type>("ThreadID") << ":"
//		<< expr::attr<unsigned int>("LineID") << "]["
//		<< expr::attr<std::string>("Process")
//		<< "][" << expr::attr<severity_level>("Severity")
//		<< "] "
//		<< ":" << expr::smessage);
//	{
//		sink_t::locked_backend_ptr p = sink->locked_backend();
//		p->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
//	}
//	return sink;
//}
//
//void stop_logging(boost::shared_ptr<sink_t> &sink)
//{
//	boost::shared_ptr<logging::core> core = logging::core::get();
//	// Remove the sink from the core, so that no records are passed to it
//	core->remove_sink(sink);
//	// Break the feeding loop
//	sink->stop();
//	// Flush all log records that may have left buffered
//	sink->flush();
//
//	sink.reset();
//}
int main()
{
	std::string logPath = getExecPath();

	getCurrentTimeStamp();
	std::string exePath;
	if (!getExePath(exePath))
	{
		return 0;
	}
	using Path = boost::filesystem::path;
	Path  path(exePath);
	std::cerr << path.parent_path().string<std::string>() << std::endl;
	std::string execDirPath = path.parent_path().string<std::string>();
	using  json = nlohmann::json;

	const char *configPath = "/config/run.json";
	std::string configfullPath = path.parent_path().string<std::string>() + configPath;
	std::cerr << configfullPath << std::endl;
	json configData;
	try
	{
		std::fstream f;
		f.exceptions(std::ios_base::failbit | std::ios_base::badbit);
		f.open(configfullPath);
		configData << f;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 0;
	}

	std::string sqlitePath;
	sqlitePath = path.parent_path().string<std::string>();
	sqlitePath += "/data/test.db";
	SQLite::Database db(sqlitePath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
	try
	{
		if (!db.tableExists("k8sevents"))
		{
			db.exec("CREATE TABLE k8sevents (time varchar(128), value TEXT)");
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::unique_ptr<ICurlHttpClient, std::function<void(ICurlHttpClient*)>> pICurlHttpClient(createCurlHttpClient(),releaseCurlHttpClient);
	
	pICurlHttpClient->setVerbose(true);

	pICurlHttpClient->setTCPKeepAlive(true);
	UrlObject urlObj("https", configData["domain"]);

	std::string token = "Authorization: Bearer ";
	token += std::string(configData["token"]);
	pICurlHttpClient->appendHeader(token);

	pICurlHttpClient->setCert("");
	
	std::string namespaceStr = configData["namespace"];
	//get node list
	//{
	//	std::string tmpUrl = url;
	//	std::string path = "/api/v1/nodes";
	//	tmpUrl += path;

	//	//get
	//	if (pICurlHttpClient->get(tmpUrl))
	//	{
	//		std::cout << pICurlHttpClient->getResponse() << std::endl;
	//	}
	//}

	//delete node 
	//{
	//	std::string tmpUrl = url;
	//	std::string path = "/api/v1/nodes/work1.node";
	//	tmpUrl += path;

	//	//delete
	//	if (pICurlHttpClient->delete_(tmpUrl))
	//	{
	//		std::cout << pICurlHttpClient->getResponse() << std::endl;
	//	}
	//}
	while (true)
	{
		std::string resourceVersion;
		//GET  /api/v1/events
		{
			urlObj.setUrl("/api/v1/events");
			if (pICurlHttpClient->get(urlObj))
			{
				try
				{
					std::cout << pICurlHttpClient->getResponse() << std::endl;
					json resData;
					std::stringstream ss(pICurlHttpClient->getResponse());
					resData << ss;
					resourceVersion = std::string(resData["metadata"]["resourceVersion"]);
					std::string filename = getCurrentTimeStamp();
					{
						SQLite::Statement query(db, "INSERT INTO k8sevents VALUES (?, ?)");
						SQLite::bind(query,filename,resData.dump());
						int nb = query.exec();
					}
				}
				catch (const std::exception& e)
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
			}
		}

		//GET /api/v1/watch/events
		{
			urlObj.setUrl("/api/v1/watch/events");
			urlObj.appendUrlParam({"resourceVersion",resourceVersion},"?");
			urlObj.appendUrlParam({"watch","true"});
			pICurlHttpClient->setResponseCallBack([&](const std::string& res, std::string& resStr) {
				std::string bakStr = resStr;
				try
				{
					std::string sqlitePath = getExecPath();
					sqlitePath += "/data/test.db";
					SQLite::Database db(sqlitePath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

					SQLite::Statement query(db, "INSERT INTO k8sevents VALUES (?, ?)");
					size_t pos = 0;

					while ((pos = resStr.find_first_of("\n")) != std::string::npos)
					{
						std::string filename = getCurrentTimeStamp();
						std::string dataSql = resStr.substr(0, pos);

						if (pos +1 < resStr.size())
						{
							resStr = resStr.substr(pos + 1, std::string::npos);
						}
						else
						{
							resStr.clear();
						}
						std::cerr << filename << " " << dataSql << std::endl;
						if (!dataSql.empty())
						{
							json resData;
							std::stringstream ss(dataSql);
							resData << ss;
							SQLite::bind(query, filename, resData.dump());
							int nb = query.exec();
						}

					}
				}
				catch (const std::exception& e)
				{
					std::cerr << e.what() << std::endl;
				}
			});
			if (pICurlHttpClient->get(urlObj))
			{
				std::string filename = getCurrentTimeStamp();
			}
			pICurlHttpClient->setResponseCallBack();
		}
	}

	//std::string resourceVersion;
	////GET  /api/v1/events
	//{
	//	std::string tmpUrl = url;
	//	std::string path = "/api/v1/events";
	//	tmpUrl += path;

	//	//delete
	//	if (pICurlHttpClient->get(tmpUrl))
	//	{
	//		std::cout << pICurlHttpClient->getResponse() << std::endl;
	//		std::string filename = getCurrentTimeStamp();
	//		try
	//		{
	//			std::stringstream ss;
	//			ss.str(pICurlHttpClient->getResponse());
	//			json data;
	//			data << ss;
	//			std::fstream f;
	//			std::string fullpath = execDirPath;
	//			fullpath += "/";
	//			fullpath += "config/";
	//			fullpath += "hhehe";
	//			fullpath += ".json";
	//			f.exceptions(std::ios_base::failbit | std::ios_base::badbit);
	//			f.open(fullpath, std::ios_base::out);
	//			f << data;
	//			resourceVersion = std::string(data["metadata"]["resourceVersion"]);
	//		}
	//		catch (const std::exception& e)
	//		{
	//			std::cerr << e.what() << std::endl;
	//			return 0;
	//		}

	//	}
	//}
	
	////GET /api/v1/watch/events
	//{
	//	std::string tmpUrl = url;
	//	std::string path = "/api/v1/watch/events";
	//	tmpUrl += path;
	//	{
	//		std::pair<std::string, std::string> paramPair;
	//		paramPair.first = "timeoutSeconds";
	//		paramPair.second = std::to_string(60 * 5);
	//		append(tmpUrl, paramPair, true);
	//	}
	//	
	//	{
	//		std::pair<std::string, std::string> paramPair;
	//		paramPair.first = "resourceVersion";
	//		paramPair.second = resourceVersion;
	//		append(tmpUrl, paramPair);
	//	}
	//	{
	//		std::pair<std::string, std::string> paramPair;
	//		paramPair.first = "watch";
	//		paramPair.second = "true";
	//		append(tmpUrl, paramPair);
	//	}

	//	
	//	//delete
	//	if (pICurlHttpClient->get(tmpUrl))
	//	{
	//		std::cout << pICurlHttpClient->getResponse() << std::endl;
	//		std::string filename = getCurrentTimeStamp();
	//		try
	//		{
	//			std::stringstream ss;
	//			ss.str(pICurlHttpClient->getResponse());
	//			json data;
	//			data << ss;
	//			std::fstream f;
	//			std::string fullpath = execDirPath;
	//			fullpath += "/";
	//			fullpath += "config/";
	//			fullpath += "hhehe_watch";
	//			fullpath += ".json";
	//			f.exceptions(std::ios_base::failbit | std::ios_base::badbit);
	//			f.open(fullpath, std::ios_base::out);
	//			f << data;
	//			
	//		}
	//		catch (const std::exception& e)
	//		{
	//			std::cerr << e.what() << std::endl;
	//			return 0;
	//		}

	//	}
	//}
	//releaseCurlHttpClient(pICurlHttpClient);
}