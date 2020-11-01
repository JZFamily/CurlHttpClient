# CurlHttpClient
a libcurl c++ warpper for http

# 概述
怀旧操作。
该库简单易懂易用，最重要的是一个非常不错的c++&cmake实践和参考模板
本库可以理论上支持win/lin编译，由于偷懒cmakelists中没有进行linux的配置。
# 进展
* 实现了简单的get和post封装。
* 支持https
* @2020-10-29 add DELETE PUT method ， but no test
* @2020-10-31 添加了一个测试用例，实际是k8s客户端。如果能弄好打算工作时用一下。 json作配置文件，nlohmann json 库来解析。
* @2020-11-01 添加了boost的使用，测试了delete接口
# 待进行
实践中可能还需要以下几个

* zlib进行数据压缩解压的解析
* utf8编码问题
* json和xml等格式问题
* put等方法实现和自定义方法


