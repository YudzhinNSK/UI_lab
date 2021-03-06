#include "../include/ILogger.h"
#include <iostream>
#include <fstream>
#include <set>
#include "../include/RC.h"

namespace {
    class MyILogger : public ILogger {
    public:
        static MyILogger* createLogger(void* pClient);
        virtual void destroyLogger(void* pClient);
        virtual void log(char const* pMsg, RESULT_CODE err);
        virtual RESULT_CODE setLogFile(char const* pLogFile);
        MyILogger() {}
        ~MyILogger() {
            if (file != nullptr)
                delete file;
            file = nullptr;
        };
    protected:
        std::set<void*> container;
        char const* file = nullptr;
    };
}

static MyILogger* logger;

ILogger::~ILogger() {}



ILogger* ILogger::createLogger(void* pClient) {
    return MyILogger::createLogger(pClient);
}

void ILogger::destroyLogger(void* pClient)
{
    if (pClient) {
        logger->destroyLogger(pClient);
    }
}

MyILogger* MyILogger::createLogger(void* pClient) {
    if (!logger) {
        logger = new MyILogger();
    }
    if (pClient != nullptr) {
        logger->container.insert(pClient);
    }
    return logger;
}

void MyILogger::destroyLogger(void* pClient) {
    if (!pClient) {
        std::cout << "I haven't client!" << std::endl;
        return;
    }
    auto client = logger->container.find(pClient);
    if (client != logger->container.end()) {
        logger->container.erase(client);
        if (logger->container.empty()) {
            logger->~MyILogger();
        }
    }
}

void MyILogger::log(char const* pMsg, RESULT_CODE err) {
    if (!pMsg || err != RESULT_CODE::SUCCESS) {
        std::ofstream fileout;
        fileout.open(file, std::ios_base::app);
        RESULT_CODE rc = err;
        switch (rc)
        {

        case RESULT_CODE::BAD_REFERENCE: {
            fileout << "BAD_REFERENCE -> " << pMsg << std::endl;
            break;
        }

        case RESULT_CODE::CALCULATION_ERROR: {
            fileout << "CALCULATION_ERROR -> " << pMsg << std::endl;
            break;
        }

        case RESULT_CODE::DIVISION_BY_ZERO: {
            fileout << "DIVISION_BY_ZERO -> " << pMsg << std::endl;
            break;
        }

        case RESULT_CODE::MULTIPLE_DEFINITION: {
            fileout << "MULTIPLE_DEFINITION -> " << pMsg << std::endl;
            break;
        }

        case RESULT_CODE::NAN_VALUE: {
            fileout << "NAN_VALUE -> " << pMsg << std::endl;
            break;
        }

        case RESULT_CODE::NOT_FOUND: {
            fileout << "NOT_FOUND -> " << pMsg << std::endl;
            break;
        }

        case RESULT_CODE::OUT_OF_BOUNDS: {
            fileout << "OUT_OF_BOUNDS -> " << pMsg << std::endl;
            break;
        }

        case RESULT_CODE::OUT_OF_MEMORY: {
            fileout << "OUT_OF_MEMORY -> " << pMsg << std::endl;
            break;
        }
        case RESULT_CODE::WRONG_ARGUMENT: {
            fileout << "WRONG_ARGUMENT -> " << pMsg << std::endl;
        }

        case RESULT_CODE::WRONG_DIM: {
            fileout << "WRONG_DIM -> " << pMsg << std::endl;
            break;
        }
        }
        fileout.close();

    }
    else {
        std::cout << pMsg << std::endl;
    }
}

RESULT_CODE MyILogger::setLogFile(char const* pLogFile) {
    RESULT_CODE rc;
    if (!pLogFile) {
        rc = RESULT_CODE::FILE_ERROR;
        std::cout << "Bad File" << std::endl;
        return rc;
    }
    file = pLogFile;
    std::ofstream fo;
    fo.open(file, std::ios_base::trunc);
    fo.close();
    rc = RESULT_CODE::SUCCESS;
    return rc;
}
