#include "../include/ISet.h"
#include "IVector.h"
#include "ILogger.h"
#include <vector>
#include<cmath>
#include <set>
#include <iostream>


namespace{
    class MyISet : public ISet {
    public:
        virtual RESULT_CODE insert(const IVector* pVector, IVector::NORM norm, double tolerance);
        virtual RESULT_CODE get(IVector*& pVector, size_t index)const;
        virtual RESULT_CODE get(IVector*& pVector, IVector const* pSample, IVector::NORM norm, double tolerance)const;
        virtual size_t getDim() const; //space dimension
        virtual size_t getSize() const; //num elements in set
        virtual void clear(); // delete all
        virtual RESULT_CODE erase(size_t index);
        virtual RESULT_CODE erase(IVector const* pSample, IVector::NORM norm, double tolerance);
        virtual ISet* clone()const;
        ~MyISet();
        MyISet(ILogger* pLogger);
        MyISet(size_t size, std::vector<const IVector*> data, ILogger* pLogger);
    protected:
        ILogger *myLog_;
        size_t dim_ = 0;
    private:
        std::vector<const IVector*> container_;
        MyISet(MyISet const& set) = delete;
        MyISet& operator=(MyISet const& vector) = delete;
    };
}

RESULT_CODE MyISet::insert(const IVector* pVector, IVector::NORM norm, double tolerance) {
    if (pVector != nullptr) {
        if (tolerance < 0) {
            if (myLog_) {
                myLog_->log("wrong tolerace value! can't be less than 0", RESULT_CODE::WRONG_ARGUMENT);
            }
            return RESULT_CODE::WRONG_ARGUMENT;
        }
        if (std::isnan(tolerance)){
            if (myLog_) {
                myLog_->log("wrong tolerance value! can't be equals NAN", RESULT_CODE::NAN_VALUE);
            }
            return RESULT_CODE::NAN_VALUE;
        }
        if (container_.empty()) {
            container_.push_back(pVector->clone());
            dim_ = pVector->getDim();
            return RESULT_CODE::SUCCESS;
        }
        if (pVector->getDim() != dim_) {
            if (myLog_) {
                myLog_->log("wrong dim in function insert()", RESULT_CODE::WRONG_DIM);
            }
            return RESULT_CODE::WRONG_DIM;
        }
        bool tr = false;
        for(size_t i = 0; i < container_.size(); i++){
            RESULT_CODE rc = IVector::equals(pVector, container_[i], norm, tolerance, &tr, myLog_);
        }
        if (tr == false) {
            container_.push_back(pVector->clone());
            return RESULT_CODE::SUCCESS;
        }
        return RESULT_CODE::SUCCESS;
    }
    if (myLog_) {
        myLog_->log("nullptr in function insert()", RESULT_CODE::BAD_REFERENCE);
    }
    return RESULT_CODE::BAD_REFERENCE;
}

MyISet::MyISet(ILogger* pLogger) {
    myLog_ = pLogger;
}

MyISet::~MyISet() {
   // myLog_ = nullptr;
    if (!container_.empty()) {
        for (unsigned i = 0; i < container_.size(); i++) {
            delete container_[i];
        }
    }
    container_.clear();
}

RESULT_CODE MyISet::get(IVector*& pVector, size_t index)const {
    if (index < container_.size()) {
        pVector = const_cast<IVector*>(container_[index]->clone());
        return RESULT_CODE::SUCCESS;
    }
    if (myLog_) {
        myLog_->log("ivalide index in function get()", RESULT_CODE::OUT_OF_BOUNDS);
    }
    return RESULT_CODE::OUT_OF_BOUNDS;
}

RESULT_CODE MyISet::get(IVector*& pVector, IVector const* pSample, IVector::NORM norm, double tolerance)const {
    if (pSample != nullptr) {
        if (tolerance < 0) {
            if (myLog_) {
                myLog_->log("wrong tolerace value! can't be less than 0", RESULT_CODE::WRONG_ARGUMENT);
            }
            return RESULT_CODE::WRONG_ARGUMENT;
        }
        if (std::isnan(tolerance)) {
            if (myLog_) {
                myLog_->log("wrong tolerance value! can't be equals NAN", RESULT_CODE::NAN_VALUE);
            }
            return RESULT_CODE::NAN_VALUE;
        }
        if (pSample->getDim() == dim_) {
            for (int i = 0; i < container_.size(); i++) {
                IVector* test;
                RESULT_CODE rc = get(test, i);
                if (rc == RESULT_CODE::SUCCESS) {
                    IVector* subVec = IVector::sub(test, pSample, myLog_);
                    double n = subVec->norm(norm);
                    if (n < tolerance) {
                        pVector = test->clone();
                        delete test;
                        delete subVec;
                        return RESULT_CODE::SUCCESS;
                    }
                    delete subVec;
                }
                delete test;
            }
            return RESULT_CODE::NOT_FOUND;
        }
        else {
            if (myLog_) {
                myLog_->log("wrong dim in function get()", RESULT_CODE::WRONG_DIM);
            }
            return RESULT_CODE::WRONG_DIM;
        }
    }
    if (myLog_) {
        myLog_->log("nullptr in get()", RESULT_CODE::BAD_REFERENCE);
    }
    return RESULT_CODE::BAD_REFERENCE;
}

size_t MyISet::getDim() const {
    return dim_;
}

size_t MyISet::getSize() const {
    return container_.size();
}

void MyISet::clear() {
    for (int i = 0; i < container_.size(); i++) {
        delete container_[i];
        container_[i] = nullptr;
    }
    container_.clear();
    dim_ = 0;
}

RESULT_CODE MyISet::erase(size_t index){
    if (index < container_.size()) {
        auto del = container_.at(index);
        container_.erase(std::remove(container_.begin(), container_.end(), del), container_.end());
        delete del;
        return RESULT_CODE::SUCCESS;
    }
    if (myLog_) {
        myLog_->log("ivalide index in function erase()", RESULT_CODE::OUT_OF_BOUNDS);
    }
    return RESULT_CODE::OUT_OF_BOUNDS;
}

RESULT_CODE MyISet::erase(IVector const* pSample, IVector::NORM norm, double tolerance) {
    if (pSample != nullptr) {
        if (tolerance < 0) {
            if (myLog_) {
                myLog_->log("wrong tolerace value! can't be less than 0", RESULT_CODE::WRONG_ARGUMENT);
            }
            return RESULT_CODE::WRONG_ARGUMENT;
        }
        if (std::isnan(tolerance)) {
            if (myLog_) {
                myLog_->log("wrong tolerance value! can't be equals NAN", RESULT_CODE::NAN_VALUE);
            }
            return RESULT_CODE::NAN_VALUE;
        }
        if (pSample->getDim() != dim_) {
            if (myLog_) {
                myLog_->log("wrong dim, check input data", RESULT_CODE::WRONG_DIM);
            }
            return RESULT_CODE::WRONG_DIM;
        }
        RESULT_CODE res = RESULT_CODE::SUCCESS;
        bool resc;
        int dim = container_.size();
        int deleted_val = 0;
        int i = 0;
        while(i < dim){
            IVector* c = container_[i]->clone();
            res = IVector::equals(c, pSample, norm, tolerance, &resc, myLog_);
            if (resc) {
                MyISet::erase(i);
                i--;
                dim--;
                deleted_val++;
            }
            delete c;
            i++;
        }
        if (!deleted_val) {
            return RESULT_CODE::NOT_FOUND;
        }
        return RESULT_CODE::SUCCESS;
    }
    if (myLog_) {
        myLog_->log("nullptr in function erease()", RESULT_CODE::BAD_REFERENCE);
    }
    return RESULT_CODE::BAD_REFERENCE;
}

MyISet::MyISet(size_t dim, std::vector<const IVector*> data, ILogger* pLogger) {
    dim_ = dim;
    myLog_ = pLogger;
    container_ = std::vector<const IVector*>();
    for (int i = 0; i < data.size(); i++) {
        container_.push_back(data[i]->clone());
    }
}

ISet* MyISet::clone()const {
    return new MyISet(dim_,container_,myLog_);
}

ISet::~ISet() {

}

ISet* ISet::createSet(ILogger* pLogger) {
    return new MyISet(pLogger);
}

ISet* ISet::add(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger) {
    if ((pOperand1 != nullptr) && (pOperand2 != nullptr)) {
        if (tolerance < 0) {
            if (pLogger) {
                pLogger->log("wrong tolerace value! can't be less than 0", RESULT_CODE::WRONG_ARGUMENT);
            }
            return nullptr;
        }
        if (std::isnan(tolerance)) {
            if (pLogger) {
                pLogger->log("wrong tolerance value! can't be equals NAN", RESULT_CODE::NAN_VALUE);
            }
            return nullptr;
        }
        if (pOperand1->getDim() == pOperand2->getDim()) {
            ISet* add = pOperand1->clone();
            size_t size = pOperand2->getSize();
            for (size_t i = 0; i < size; i++) {
                IVector* first;
                IVector* second = nullptr;
                pOperand2->get(first, i);
                RESULT_CODE rc = pOperand1->get(second, first, norm, tolerance);
                if (rc == RESULT_CODE::NOT_FOUND) {
                    add->insert(first, norm, tolerance);
                }
                if (first) {
                    delete first;
                    first = nullptr;
                }
                if (second) {
                    delete second;
                    first = nullptr;
                }
            }
            return add;
        }
        if (pLogger) {
            pLogger->log("wrong dim in function add of sets", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }
    if (pLogger) {
        pLogger->log("nullptr sets in function add of sets", RESULT_CODE::BAD_REFERENCE);
    }
    return nullptr;
}

ISet* ISet::intersect(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger) {
    if (pOperand1 != nullptr && pOperand2 != nullptr) {
        if (tolerance < 0) {
            if (pLogger) {
                pLogger->log("wrong tolerace value! can't be less than 0", RESULT_CODE::WRONG_ARGUMENT);
            }
            return nullptr;
        }
        if (std::isnan(tolerance)) {
            if (pLogger) {
                pLogger->log("wrong tolerance value! can't be equals NAN", RESULT_CODE::NAN_VALUE);
            }
            return nullptr;
        }
        if (pOperand1->getDim() == pOperand2->getDim()) {
            ISet* intersect = new MyISet(pLogger);
            size_t size = pOperand2->getSize();
            IVector* first = nullptr;
            IVector* second = nullptr;
            for (size_t i = 0; i < size; i++) {
                pOperand2->get(first, i);
                RESULT_CODE rc = pOperand1->get(second, first, norm, tolerance);
                if (rc == RESULT_CODE::SUCCESS) {
                    intersect->insert(second, norm, tolerance);
                }
                if (first) {
                    delete first;
                    first = nullptr;
                }
                if (second) {
                    delete second;
                    second = nullptr;
                }
            }
            return intersect;
        }
        if (pLogger) {
            pLogger->log("wrong dim in function intersect of sets", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }
    if (pLogger) {
        pLogger->log("nullptr sets in function intersect of sets", RESULT_CODE::BAD_REFERENCE);
    }
    return nullptr;
}

ISet* ISet::sub(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger) {
    if (pOperand1 != nullptr && pOperand2 != nullptr) {
        if (tolerance < 0) {
            if (pLogger) {
                pLogger->log("wrong tolerace value! can't be less than 0", RESULT_CODE::WRONG_ARGUMENT);
            }
            return nullptr;
        }
        if (std::isnan(tolerance)) {
            if (pLogger) {
                pLogger->log("wrong tolerance value! can't be equals NAN", RESULT_CODE::NAN_VALUE);
            }
            return nullptr;
        }
        if (pOperand1->getDim() == pOperand2->getDim()) {
            ISet* sub = pOperand1->clone();
            size_t size = pOperand2->getSize();
            IVector* first = nullptr;
            IVector* second = nullptr;
            for (size_t i = 0; i < size; i++) {
                pOperand2->get(first, i);
                RESULT_CODE rc = pOperand1->get(second, first, norm, tolerance);
                if (rc == RESULT_CODE::SUCCESS) {
                    sub->erase(second, norm, tolerance);
                }

                if (first) {
                    delete first;
                    first = nullptr;
                }
                if (second) {
                    delete second;
                    second = nullptr;
                }
            }
            return sub;
        }
        if (pLogger) {
            pLogger->log("wrong dim in function sub of sets", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }
    if (pLogger) {
        pLogger->log("nullptr sets in function sub of sets", RESULT_CODE::BAD_REFERENCE);
    }
    return nullptr;
}

ISet* ISet::symSub(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger) {
    if (pOperand1 != nullptr && pOperand2 != nullptr) {
        if (tolerance < 0) {
            if (pLogger) {
                pLogger->log("wrong tolerace value! can't be less than 0", RESULT_CODE::WRONG_ARGUMENT);
            }
            return nullptr;
        }
        if (std::isnan(tolerance)) {
            if (pLogger) {
                pLogger->log("wrong tolerance value! can't be equals NAN", RESULT_CODE::NAN_VALUE);
            }
            return nullptr;
        }
        if (pOperand1->getDim() == pOperand2->getDim()) {
            ISet* add = ISet::add(pOperand1, pOperand2, norm, tolerance, pLogger);
            ISet* intersect = ISet::intersect(pOperand1, pOperand2, norm, tolerance, pLogger);
            ISet* symSub = ISet::sub(add, intersect, norm, tolerance, pLogger);
            add->clear();
            delete add;
            add = nullptr;
            intersect->clear();
            delete intersect;
            intersect = nullptr;
            return symSub;
        }
        if (pLogger) {
            pLogger->log("wrong dim in function sub of sets", RESULT_CODE::WRONG_DIM);
        }
        return nullptr;
    }
    if (pLogger) {
        pLogger->log("nullptr sets in function sub of sets", RESULT_CODE::BAD_REFERENCE);
    }
    return nullptr;
}
