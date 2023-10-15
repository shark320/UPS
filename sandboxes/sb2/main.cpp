#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

int main() {
    // Initialize Log4cxx using your configuration file
    log4cxx::PropertyConfigurator::configure("config/log4cxx.properties");

    // Create a logger
    log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("MyLogger"));

    // Log messages at different levels
    LOG4CXX_DEBUG(logger, "This is a DEBUG message");
    LOG4CXX_INFO(logger, "This is an INFO message");
    LOG4CXX_WARN(logger, "This is a WARN message");
    LOG4CXX_ERROR(logger, "This is an ERROR message");

    return 0;
}
