#include "../include/ICompact.h"
#include "../src/IVector.h"
#include <cmath>
#include <set>
#include <iostream>


namespace {
	class MyICompact : public ICompact {
	public:

        /* returns a step, end, begin with which you can iterate over all domains of compact*/
        virtual IVector* getBegin() const;
        virtual IVector* getEnd() const;

        virtual iterator* end(IVector const* const step);
        virtual iterator* begin(IVector const* const step);

        virtual RESULT_CODE isContains(IVector const* const vec, bool& result) const;
        virtual RESULT_CODE isSubSet(ICompact const* const other, bool& result) const;
        virtual RESULT_CODE isIntersects(ICompact const* const other, bool& result) const;

        virtual size_t getDim() const;
        virtual ICompact* clone() const;

        ~MyICompact();
        MyICompact(IVector const* const begin, IVector const* const end,ILogger* pLogger);

        class MyIterator : public iterator
        {
        public:
            //adds step to current value in iterator
            //+step
            virtual RESULT_CODE doStep(){
                if ((dir_ != nullptr) && (step_ !=nullptr)) {
                    double curr_prior = 0;
                    while (true) {
                        size_t index = -1;
                        for (size_t i = 0; i < dir_->getDim(); i++) {
                            if (curr_prior == dir_->getCoord(i)) {
                                index = i;
                            }
                        }
                        if (index == -1) {
                            break;
                        }
                        if (step_->getCoord(index) == 0) {
                            curr_prior++;
                            continue;
                        }
                        if (step_->getCoord(index) > 0) {
                            if (fabs(cur_place->getCoord(index) + step_->getCoord(index) - end_p->getCoord(index)) <= 1e-9 || cur_place->getCoord(index) + step_->getCoord(index) < end_p->getCoord(index)) {
                                double data = cur_place->getCoord(index) + step_->getCoord(index);
                                cur_place->setCoord(index, data);
                                return RESULT_CODE::SUCCESS;
                            }
                            else {
                                int count = 0;
                                for (size_t i = 0; i < step_->getDim(); i++) {
                                    if (fabs(cur_place->getCoord(i) - end_p->getCoord(i)) <= 1e-9) {
                                        count++;
                                    }
                                }
                                if (count == step_->getDim()) {
                                    return RESULT_CODE::OUT_OF_BOUNDS;
                                }
                                curr_prior++;
                                cur_place->setCoord(index, start_p->getCoord(index));
                            }
                        }
                        else {
                            if (cur_place->getCoord(index) + step_->getCoord(index) > end_p->getCoord(index) || fabs(cur_place->getCoord(index) + step_->getCoord(index) - end_p->getCoord(index)) <= 1e-9) {
                                double data = cur_place->getCoord(index) + step_->getCoord(index);
                                cur_place->setCoord(index, data);
                                return RESULT_CODE::SUCCESS;
                            }
                            else {
                                int count = 0;
                                for (size_t i = 0; i < step_->getDim(); i++) {
                                    if (fabs(cur_place->getCoord(i) - end_p->getCoord(i)) >= 1e-9) {
                                        count++;
                                    }
                                }
                                if (count == step_->getDim()) {
                                    return RESULT_CODE::OUT_OF_BOUNDS;
                                }
                                curr_prior++;
                                cur_place->setCoord(index, start_p->getCoord(index));
                            }
                        }
                    }
                    return RESULT_CODE::OUT_OF_BOUNDS;
                }
                else {
                    return RESULT_CODE::BAD_REFERENCE;
                }
            };

            virtual IVector* getPoint() const {
                return cur_place->clone();
            };

            //change order of step
            virtual RESULT_CODE setDirection(IVector const* const dir) {
                if (dir != nullptr) {
                    if (dir->getDim() == step_->getDim()) { 
                        for (size_t i = 0; i < dir->getDim() - 1; i++)
                        {
                            for (size_t j = i + 1; j < dir->getDim(); j++) {
                                if (dir->getCoord(i) == dir->getCoord(j)) {
                                    return RESULT_CODE::MULTIPLE_DEFINITION;
                                }
                            }
                        }
                        delete dir_;
                        dir_ = dir->clone();
                        delete cur_place;
                        cur_place = start_p->clone();
                        return RESULT_CODE::SUCCESS;
                    }
                    else {
                        return RESULT_CODE::WRONG_DIM;
                    }
                }
                else {
                    return RESULT_CODE::WRONG_ARGUMENT;
                }
            }

            /*dtor*/
            virtual ~MyIterator() {
                delete start_p;
                delete end_p;
                delete cur_place;
                delete step_;
                delete dir_;
            }

            MyIterator(IVector const* const start, IVector const* const end, IVector const* const step) {
                start_p = start->clone();
                end_p = end->clone();
                step_ = step->clone();
                cur_place = start->clone();
                size_t i = end->getDim();
                double* p = new double[i];
                for (size_t k = 0; k < i; k++) {
                    p[k] = (double)k;
                }
                dir_ = IVector::createVector(i, p, nullptr);
                delete[] p;
            };


        private:
            /*non default copyable*/
            IVector* start_p = nullptr;
            IVector* end_p = nullptr;
            IVector* cur_place = nullptr;
            IVector* step_ = nullptr;
            IVector* dir_ = nullptr;
            MyIterator(const MyIterator& other) = delete;
            void operator=(const MyIterator& other) = delete;
        };

    private:
        ILogger* log_ = nullptr;
        IVector* lb = nullptr;
        IVector* rt = nullptr;
        MyICompact(const MyICompact& other) = delete;
        void operator=(const MyICompact& other) = delete;
	};
}

MyICompact::iterator* MyICompact::begin(IVector const* const step) {
    if (step != nullptr) {
        size_t dim = step->getDim();
        if (dim == lb->getDim()) {
            for (size_t i = 0; i < dim; i++) {
                if (step->getCoord(i) <= 0) {
                    if (log_) {
                        log_->log("All coeff-s of step vector must more than 0 if you use BEGIN method", RESULT_CODE::WRONG_ARGUMENT);
                    }
                    return nullptr;;
                }
            }
            return new MyIterator(lb, rt, step);
        }
        else {
            if (log_) {
                log_->log("Wrong dim of step vector", RESULT_CODE::WRONG_DIM);
            }
            return nullptr;
        }
    }
    else {
        if (log_) {
            log_->log("Step is NULL vector, please check data", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }
}

MyICompact::iterator* MyICompact::end(IVector const* const step) {
    if (step != nullptr) {
        size_t dim = step->getDim();
        if (dim == lb->getDim()) {
            for (size_t i = 0; i < dim; i++) {
                if (step->getCoord(i) >= 0) {
                    if (log_) {
                        log_->log("All coeff-s of step vector must less than 0 if you use END method", RESULT_CODE::WRONG_ARGUMENT);
                    }
                    return nullptr;;
                }
            }
            return new MyIterator(rt, lb, step);
        }
        else {
            if (log_) {
                log_->log("Wrong dim of step vector", RESULT_CODE::WRONG_DIM);
            }
            return nullptr;
        }
    }
    else {
        if (log_) {
            log_->log("Step is NULL vector, please check data", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }
}

MyICompact::MyICompact(IVector const* const begin, IVector const* const end, ILogger* pLogger) {
    lb = const_cast<IVector*>(begin);
    rt = const_cast<IVector*>(end);
    log_ = pLogger;
}

MyICompact::~MyICompact(){
    delete lb;
    delete rt;
    log_ = nullptr;
}

ICompact::~ICompact() {}

size_t MyICompact::getDim() const {
    size_t dim = lb->getDim();
    return dim;
}

ICompact* MyICompact::clone()const {
    return new MyICompact(lb->clone(), rt->clone(), log_);
}

RESULT_CODE MyICompact::isContains(IVector const* const vec, bool& result) const {
    if (vec != nullptr) {
        if (vec->getDim() == lb->getDim()) {
            size_t dim = lb->getDim();
                for (size_t i = 0; i < dim; i++) {
                    if (!((vec->getCoord(i) >= lb->getCoord(i)) && (vec->getCoord(i) <= rt->getCoord(i)))) {
                        result = false;
                            return RESULT_CODE::SUCCESS;
                    }
                }
            result = true;
            return RESULT_CODE::SUCCESS;
        }
        else {
            result = false;
            if (log_) {
                log_->log("wrong dim of input vector!", RESULT_CODE::WRONG_DIM);
            }
            return RESULT_CODE::WRONG_DIM;
        }
    }
    else {
        result = false;
        if (log_) {
            log_->log("BAD DATA CHECK THE INPUT VECTOR", RESULT_CODE::BAD_REFERENCE);
        }
        return RESULT_CODE::BAD_REFERENCE;
    }
}

RESULT_CODE MyICompact::isSubSet(ICompact const* const other, bool& result)const {
    if (other != nullptr) {
        if (other->getDim() == lb->getDim()) {
            size_t dim = lb->getDim();
            IVector* second_beg = other->getBegin();
            IVector* second_end = other->getEnd();
            for (size_t i = 0; i < dim; i++) {
                if (!((second_beg->getCoord(i) >= lb->getCoord(i)) && (second_beg->getCoord(i) <= rt->getCoord(i)))) {
                    result = false;
                    delete second_beg;
                    delete second_end;
                    return RESULT_CODE::SUCCESS;
                }
            }
            for (size_t i = 0; i < dim; i++) {
                if (!((second_end->getCoord(i) >= lb->getCoord(i)) && (second_end->getCoord(i) <= rt->getCoord(i)))) {
                    result = false;
                    delete second_beg;
                    delete second_end;
                    return RESULT_CODE::SUCCESS;
                }
            }
            result = true;
            delete second_beg;
            delete second_end;
            return RESULT_CODE::SUCCESS;
        }
        else {
            result = false;
            if (log_) {
                log_->log("BAD DIM CHECK THE INPUT COMPACT IN SUBSET", RESULT_CODE::WRONG_DIM);
            }
            return RESULT_CODE::WRONG_DIM;
        }
    }
    else {
        result = false;
        if (log_) {
            log_->log("BAD DATA CHECK THE INPUT COMPACT IN SUBSET", RESULT_CODE::BAD_REFERENCE);
        }
        return RESULT_CODE::BAD_REFERENCE;
    }
}

RESULT_CODE MyICompact::isIntersects(ICompact const* const other, bool& result) const {
    if (other != nullptr) {
        if (lb->getDim() == other->getDim()) {
            ICompact* copy_this_comp = new MyICompact(lb->clone(), rt->clone(), log_);
            ICompact* inter = ICompact::intersection(copy_this_comp, other, log_);
            if (inter) {
                result = true;
                delete copy_this_comp;
                copy_this_comp = nullptr;
                delete inter;
                inter = nullptr;
                return RESULT_CODE::SUCCESS;
            }
            else {
                result = false;
                delete copy_this_comp;
                copy_this_comp = nullptr;
                delete inter;
                inter = nullptr;
                return RESULT_CODE::SUCCESS;
            }
        }
        else {
            result = false;
            if (log_) {
                log_->log("wrong dim in isIntersects", RESULT_CODE::WRONG_DIM);
            }
            return RESULT_CODE::WRONG_DIM;
        }
    }
    else {
        result = false;
        if (log_) {
            log_->log("BAD REFERENCE in isIntersect", RESULT_CODE::BAD_REFERENCE);
        }
        return RESULT_CODE::BAD_REFERENCE;
    }
}

ICompact* ICompact::createCompact(IVector const* const begin, IVector const* const end, ILogger* logger){
    if ((begin != nullptr) && (end != nullptr)) {
        if (begin->getDim() == end->getDim()) {
            IVector* left = begin->clone();
            IVector* right = end->clone();
            size_t dim = begin->getDim();
            for (size_t i = 0; i < dim; i++) {
                if (left->getCoord(i) > right->getCoord(i)) {
                    if (logger) {
                        logger->log("WRONG PLACES VECTORS", RESULT_CODE::WRONG_ARGUMENT);
                    }
                    delete left;
                    delete right;
                    return nullptr;
                }                
            }
            return new MyICompact(left, right, logger);
        }
        else {
            if (logger != nullptr) {
                logger->log("Check vector's dim in create compact", RESULT_CODE::WRONG_DIM);
            }
            return nullptr;
        }
    }
    else {
        if (logger != nullptr) {
            logger->log("Check vector's in create compact", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }
}

IVector* MyICompact::getBegin() const{
    return lb->clone();
}

IVector* MyICompact::getEnd() const {
    return rt->clone();
}

ICompact* ICompact::intersection(ICompact const* const left, ICompact const* const right, ILogger* logger){
    if ((left != nullptr) && (right != nullptr)) {
        size_t dim = left->getDim();
        if (left->getDim() == right->getDim()) {
            bool isSubSet = false;
            left->isSubSet(right,isSubSet);
            if (isSubSet) {
                return right->clone();
            }

            right->isSubSet(left, isSubSet);
            if (isSubSet) {
                return left->clone();
            }

            IVector* leftcompact_lb = left->getBegin();
            IVector* rightcompact_lb = right->getBegin();

            IVector* leftcompact_rt = left->getEnd();
            IVector* rightcompact_rt = right->getEnd();

            left->isContains(rightcompact_lb, isSubSet);

            if (!isSubSet) {
                left->isContains(rightcompact_rt,isSubSet);
            }

            if (!isSubSet) {
                delete leftcompact_lb;
                delete leftcompact_rt;

                delete rightcompact_lb;
                delete rightcompact_rt;
                return nullptr;
            }

            double* data1 = new double[(int)dim];
            double* data2 = new double[(int)dim];

            for (int i = 0; i < dim; i++) {
                if (leftcompact_lb->getCoord(i) < rightcompact_lb->getCoord(i)) {
                    data1[i] = rightcompact_lb->getCoord(i);
                }
                else {
                    data1[i] = leftcompact_lb->getCoord(i);
                }

                if (leftcompact_rt->getCoord(i) < rightcompact_rt->getCoord(i)) {
                    data2[i] = leftcompact_rt->getCoord(i);
                }
                else {
                    data2[i] = rightcompact_rt->getCoord(i);
                }
            }

            delete leftcompact_lb;
            delete leftcompact_rt;

            delete rightcompact_lb;
            delete rightcompact_rt;
            
            IVector* left = IVector::createVector(dim, data1, logger);
            IVector* right = IVector::createVector(dim, data2, logger);

            delete[] data1;
            delete[] data2;

            return new MyICompact(left, right,logger);

        }
        else {
            if (logger != nullptr) {
                logger->log("Check compacts dim in intersect", RESULT_CODE::WRONG_DIM);
            }
            return nullptr;
        }
    }
    else {
        if (logger != nullptr) {
            logger->log("Check compacts in intersect", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }
}

ICompact* ICompact::add(ICompact const* const left, ICompact const* const right, ILogger* logger) {
    if ((left != nullptr) && (right != nullptr)) {
        size_t dim = left->getDim();
        if (dim == right->getDim()) {
            bool isSubset = false;
            
            left->isSubSet(right, isSubset);
            if (isSubset) {
                return left->clone();
            }

            right->isSubSet(left, isSubset);
            if (isSubset) {
                return right->clone();
            }

            int nedeed_eq = 0;
            IVector* leftCompact_lb = left->getBegin();
            IVector* rightCompact_lb = right->getBegin();

            if (leftCompact_lb->getCoord(0) > rightCompact_lb->getCoord(0)) {
                delete leftCompact_lb;

                delete rightCompact_lb;
                return ICompact::add(right, left, logger);
            }

            IVector* res_lb = IVector::sub(leftCompact_lb, rightCompact_lb, logger);
            
            for (int i = 0; i < dim; i++) {
                if (res_lb->getCoord(i) == 0) {
                    nedeed_eq++;
                }
            }
            
            delete rightCompact_lb;

            if (nedeed_eq == dim - 1) {
                IVector* leftCompact_rt = left->getEnd();
                IVector* rightCompact_rt = right->getEnd();

                IVector* res_rt = IVector::sub(leftCompact_rt, rightCompact_rt, logger);

                for (int i = 0; i < dim; i++) {
                    if ((res_lb->getCoord(i) == 0) &&( res_rt->getCoord(i) == 0)) {
                        nedeed_eq--;
                    }
                }

                delete res_lb;
                delete leftCompact_rt;
                delete res_rt;

                if (nedeed_eq == 0) {
                    return new MyICompact(leftCompact_lb, rightCompact_rt, logger);
                }
                else {
                    delete rightCompact_rt;
                    return nullptr;
                }
            }
            else {
                delete leftCompact_lb;
                delete res_lb;
                return nullptr;
            }
        }
        else {
            if (logger != nullptr) {
                logger->log("Check compacts dim in add", RESULT_CODE::WRONG_DIM);
            }
            return nullptr;
        }
    }
    else {
        if (logger != nullptr) {
            logger->log("Check compacts in add", RESULT_CODE::WRONG_ARGUMENT);
        }
        return nullptr;
    }
}

ICompact* ICompact::makeConvex(ICompact const* const left, ICompact const* const right, ILogger* logger) {
    if ((left != nullptr) && (right != nullptr)) {
        if (left->getDim() == right->getDim()) {
            bool isSubset = false;

            left->isSubSet(right, isSubset);
            if (isSubset) {
                return left->clone();
            }

            right->isSubSet(left, isSubset);
            if (isSubset) {
                return right->clone();
            }
            
            size_t dim = left->getDim();

            double* data1 = new double[dim];
            double* data2 = new double[dim];

            IVector* left_lb = left->getBegin();
            IVector* left_rt = left->getEnd();

            IVector* right_lb = right->getBegin();
            IVector* right_rt = right->getEnd();

            for (int i = 0; i < dim; i++) {
                if (left_lb->getCoord(i) < right_lb->getCoord(i)) {
                    data1[i] = left_lb->getCoord(i);
                }
                else {
                    data1[i] = right_lb->getCoord(i);
                }

                if (left_rt->getCoord(i) > right_rt->getCoord(i)) {
                    data2[i] = left_rt->getCoord(i);
                }
                else {
                    data2[i] = right_rt->getCoord(i);
                }
            }
            delete left_lb;
            delete left_rt;

            delete right_lb;
            delete right_rt;

            IVector* left = IVector::createVector(dim, data1, logger);
            IVector* right = IVector::createVector(dim, data2, logger);

            delete[] data1;
            delete[] data2;

            return new MyICompact(left, right, logger);
        }
        else {
            if (logger != nullptr) {
                logger->log("Check compacts dim in makeConvex", RESULT_CODE::WRONG_DIM);
            }
            return nullptr;
        }
    }
    else {
        if (logger != nullptr) {
            logger->log("Check compacts dim in makeConvex", RESULT_CODE::WRONG_ARGUMENT);
        }                
        return nullptr;
    }
}
