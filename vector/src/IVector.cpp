#include "../include/IVector.h"
#include "../src/ILogger.h"
#include "../include/RC.h"
#include <cmath>
#include <iostream>

namespace {
    class MyIVector : public IVector {
    public:
        virtual IVector* clone() const;
        virtual double getCoord(size_t index)const;
        virtual RESULT_CODE setCoord(size_t index, double value);
        virtual size_t getDim() const;
        virtual double norm(IVector::NORM norm) const;
        MyIVector(size_t size, double* pData, ILogger* pLogger);
        ~MyIVector();
    protected:
        ILogger* logger;
    private:
        double* data = nullptr;
        size_t dim = 0;
        MyIVector(MyIVector const& vector) = delete;
        MyIVector& operator=(MyIVector const& vector) = delete;
    };
}

MyIVector::MyIVector(size_t size, double* pData, ILogger* pLogger) {
    data = pData;
    dim = size;
    logger = pLogger;
}

IVector::~IVector() {

}

MyIVector::~MyIVector() {
    logger = nullptr;
}

IVector* MyIVector::clone()const {
    if (data != nullptr) {
        return MyIVector::createVector(dim, data, logger);
    }
    return nullptr;
};

double MyIVector::getCoord(size_t index)const {
    if (index < dim) {
        return data[index];
    }
    else {
        return NAN;
    }

};

RESULT_CODE MyIVector::setCoord(size_t index, double value) {
    if ((index < dim) && (!std::isnan(value))) {
        data[index] = value;
        return RESULT_CODE::SUCCESS;
    }
    else {
        return RESULT_CODE::NOT_FOUND;
    }
};

size_t MyIVector::getDim()const { return dim; };

IVector* IVector::add(IVector const* pOperand1, IVector const* pOperand2, ILogger* pLogger) {
    if (pOperand1 != nullptr && pOperand2 != nullptr) {
        if (pOperand1->getDim() == pOperand2->getDim()) {
            size_t h = pOperand1->getDim();
            double* k = new double[h];
            for (size_t i = 0; i < h; i++) {
                k[i] = pOperand1->getCoord(i) + pOperand2->getCoord(i);
            }
            IVector* add = IVector::createVector(h, k, pLogger);
            delete[] k;
            return add;
        }
        if (pLogger) {
            pLogger->log("BAD Operation! Please, check the DIMS of input arguments ", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }
    if (pLogger) {
        pLogger->log("BAD Operation! Please, check the input arguments. (VECTORS)", RESULT_CODE::BAD_REFERENCE);
    }
    return nullptr;
}

IVector* IVector::sub(IVector const* pOperand1, IVector const* pOperand2, ILogger* pLogger) {
    if (pOperand1 != nullptr && pOperand2 != nullptr) {
        if (pOperand1->getDim() == pOperand2->getDim()) {
            size_t h = pOperand1->getDim();
            double* k = new double[h];
            for (size_t i = 0; i < h; i++) {
                k[i] = pOperand1->getCoord(i) - pOperand2->getCoord(i);
            }
            IVector* vec = IVector::createVector(h, k, pLogger);
            delete[] k;
            return  vec;
        }
        if (pLogger) {
            pLogger->log("BAD Operation! Please, check the DIMS of input arguments ", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }
    if (pLogger) {
        pLogger->log("BAD Operation! Please, check the input arguments. (VECTORS)", RESULT_CODE::BAD_REFERENCE);
    }
    return nullptr;
}

IVector* IVector::mul(IVector const* pOperand1, double scaleParam, ILogger* pLogger) {
    if (pOperand1 != nullptr) {
        if (scaleParam == NAN) {
            return nullptr;
        }
        size_t h = pOperand1->getDim();
        double* k = new double[h];
        for (size_t i = 0; i < h; i++) {
            k[i] = pOperand1->getCoord(i) * scaleParam;
        }
        IVector* vec = IVector::createVector(h, k, pLogger);
        delete[] k;
        return vec;
    }
    if (pLogger) {
        pLogger->log("BAD Operation! Please, check the input arguments. (VECTORS)", RESULT_CODE::BAD_REFERENCE);
    }
    return nullptr;
}

double IVector::mul(IVector const* pOperand1, IVector const* pOperand2, ILogger* pLogger) {
    if (pOperand1 != nullptr && pOperand2 != nullptr) {
        if (pOperand1->getDim() == pOperand2->getDim()) {
            size_t j = pOperand1->getDim();
            double mul = 0;
            for (size_t i = 0; i < j; i++) {
                mul += pOperand1->getCoord(i) * pOperand2->getCoord(i);
            }
            return mul;
        }
        if (pLogger) {
            pLogger->log("BAD Operation! Please, check the DIMS of input arguments ", RESULT_CODE::WRONG_DIM);
        }
        return NAN;
    }
    if (pLogger) {
        pLogger->log("BAD Operation! Please, check the input arguments. (VECTORS)", RESULT_CODE::BAD_REFERENCE);
    }
    return NAN;
};

IVector* IVector::createVector(size_t dim, double* pData, ILogger* pLogger) {
    if (pData != nullptr) {
        if (dim <= 0) {
            if (pLogger) {
                pLogger->log("zero or negative dim! check input data!", RESULT_CODE::WRONG_DIM);
            }
            return nullptr;
        }
        for (size_t i = 0; i < dim; i++) {
            if (std::isnan(pData[i])) {
                return nullptr;
            }
        }
        size_t size = sizeof(MyIVector) + dim * sizeof(double);
        void* ptr = new(std::nothrow)unsigned char[size];
        MyIVector* newvec = new(ptr)MyIVector(dim,
            reinterpret_cast<double*>((unsigned char*)ptr + sizeof(MyIVector)), pLogger);
        memcpy((unsigned char*)ptr + sizeof(MyIVector), pData, dim * sizeof(double));
        return newvec;
    }
    else {
        if (pLogger) {
            pLogger->log("Check the input data!", RESULT_CODE::BAD_REFERENCE);
        }
        return nullptr;
    }
}

double MyIVector::norm(IVector::NORM norm) const {
    if (data != nullptr) {
        double result = 0;
        double val = 0;
        switch (norm)
        {
        case IVector::NORM::NORM_1:
            for (size_t i = 0; i < dim; i++) {
                result += std::fabs(data[i]);
            }
            break;
        case IVector::NORM::NORM_2:
            for (size_t i = 0; i < dim; i++) {
                val = std::fabs(data[i]);
                val *= val;
                result += val;
            }
            result = sqrt(result);
            break;
        case IVector::NORM::NORM_INF:
            result = std::fabs(data[0]);
            for (size_t i = 1; i < dim; i++) {
                if (result < std::fabs(data[i])) {
                    result = std::fabs(data[i]);
                }
            }
            break;
        default:
            return NAN;
        }

        return result;

    }

    return NAN;
}

RESULT_CODE IVector::equals(IVector const* pOperand1, IVector const* pOperand2, NORM norm, double tolerance, bool* result, ILogger* pLogger) {
    if ((pOperand1 == nullptr) || (pOperand2 == nullptr)) {
        if (pLogger) {
            pLogger->log(" BAD Operation! Please, check the input arguments. (VECTORS)", RESULT_CODE::BAD_REFERENCE);
        }
        *result = false;
        return RESULT_CODE::BAD_REFERENCE;
    }

    if (pOperand1->getDim() != pOperand2->getDim()) {
        if (pLogger) {
            pLogger->log("BAD Operation! Check the vectors dims!", RESULT_CODE::WRONG_DIM);
        }
        *result = false;
        return RESULT_CODE::CALCULATION_ERROR;
    }

    if ((tolerance < 0) || (std::isnan(tolerance))) {
        if (pLogger) {
            pLogger->log("Tolerance can't be less 0, check the input argumet", RESULT_CODE::WRONG_ARGUMENT);
        }
        *result = false;
        return RESULT_CODE::WRONG_ARGUMENT;
    }
    IVector* subVec = nullptr;
    double res = 0;
    subVec = IVector::sub(pOperand1, pOperand2, pLogger);
    res = subVec->norm(norm);
    if (res <= tolerance) {
        delete subVec;
        subVec = nullptr;
        *result = true;
        return RESULT_CODE::SUCCESS;
    }
    else {
        delete subVec;
        subVec = nullptr;
        *result = false;
        return RESULT_CODE::SUCCESS;
    }
}
