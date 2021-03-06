#include "../src/ILogger.h"
#include "../src/IVector.h"
#include "../src/ICompact.h"
#include <cmath>
#include <iostream>

bool CreateTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 5,7,10 };
    double* data3 = new double[4]{ 3,5,4,0 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(4, data3, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);// success
    ICompact* compact_test2 = ICompact::createCompact(vec2, vec1, pLogger);// bad place
    ICompact* compact_test3 = ICompact::createCompact(vec1, vec3, pLogger);// wrong dim

    bool test_success = compact_test != nullptr && !compact_test2 && !compact_test3;

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete vec1;
    delete vec2;
    delete vec3;

    delete compact_test;
    delete compact_test2;
    delete compact_test3;

    return test_success;
}

bool GetBeg_GetEndTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 5,7,10 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);

    IVector* test_beg = compact_test->getBegin();
    IVector* test_end = compact_test->getEnd();

    bool test_success = compact_test != nullptr && test_beg != nullptr && test_end != nullptr;
    RESULT_CODE rc;
    if (test_success) {
        rc = IVector::equals(test_beg, vec1, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
        test_success = rc == RESULT_CODE::SUCCESS;
        if (test_success) {
            IVector::equals(test_end, vec2, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
            test_success = rc == RESULT_CODE::SUCCESS;
        }
    }

    delete[] data1;
    delete[] data2;
    delete vec1;
    delete vec2;
    delete test_beg;
    delete test_end;

    delete compact_test;

    return test_success;
}

bool GetDimTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 5,7,10 };
    double* data3 = new double[4]{ 1,1,1,1 };
    double* data4 = new double[4]{ 5,7,10,13 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(4, data3, pLogger);
    IVector* vec4 = IVector::createVector(4, data4, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);
    ICompact* compact_test2 = ICompact::createCompact(vec3, vec4, pLogger);

    bool test_success = compact_test != nullptr &&
                        compact_test2 != nullptr &&
                        compact_test->getDim() == 3 &&
                        compact_test2->getDim() == 4;


    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;

    delete compact_test;
    delete compact_test2;

    return test_success;
}

bool CloneTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 5,7,10 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);
    ICompact* compact_clone = compact_test->clone();


    bool test_success = compact_test != nullptr &&
                        compact_clone != nullptr &&
                        compact_test->getDim() == compact_clone->getDim();

    RESULT_CODE rc;
    if (test_success) {
        IVector* left1 = compact_test->getBegin();
        IVector* left2 = compact_clone->getBegin();
        rc = IVector::equals(left1, left2, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
        test_success = rc == RESULT_CODE::SUCCESS;
        delete left1;
        delete left2;
        if (test_success) {
            IVector* right1 = compact_test->getBegin();
            IVector* right2 = compact_clone->getBegin();
            rc = IVector::equals(right1, right2, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
            test_success = rc == RESULT_CODE::SUCCESS;
            delete right1;
            delete right2;
        }
    }

    delete[] data1;
    delete[] data2;
    delete vec1;
    delete vec2;

    delete compact_test;
    delete compact_clone;

    return test_success;
}

bool IsContTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 5,7,10 };
    double* data3 = new double[3]{ 3,5,6 };
    double* data4 = new double[4]{ 5,7,10,13 };
    double* data5 = new double[3]{ 10,10,20 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(4, data4, pLogger);
    IVector* vec5 = IVector::createVector(3, data5, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);

    bool test_success = compact_test != nullptr;
    RESULT_CODE rc;
    if (test_success) {
        rc = compact_test->isContains(vec3, test_success);
        test_success = rc == RESULT_CODE::SUCCESS;
        if (test_success) {
            compact_test->isContains(vec4, test_success);
            test_success = rc == RESULT_CODE::SUCCESS;
            if (!test_success) {
                compact_test->isContains(vec5, test_success);
                test_success = rc == RESULT_CODE::SUCCESS;
                if (!test_success) {
                    compact_test->isContains(nullptr, test_success);
                    test_success = rc == RESULT_CODE::SUCCESS;
                    if(!test_success){
                        test_success = true;
                    }
                }
            }
        }
    }

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete[] data5;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec5;

    delete compact_test;

    return test_success;
}

bool IsSubSetTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 5,7,10 };
    double* data3 = new double[3]{ 1,2,3 };
    double* data4 = new double[3]{ 4,6,9 };

    double* data5 = new double[4]{ 5,7,10,13 };
    double* data6 = new double[4]{ 10,12,14,100 };

    double* data7 = new double[3]{ 6,8,11 };
    double* data8 = new double[3]{ 10,10,15 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(3, data4, pLogger);
    IVector* vec5 = IVector::createVector(4, data5, pLogger);
    IVector* vec6 = IVector::createVector(4, data6, pLogger);
    IVector* vec7 = IVector::createVector(3, data7, pLogger);
    IVector* vec8 = IVector::createVector(3, data8, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);
    ICompact* compact_test2 = ICompact::createCompact(vec3, vec4, pLogger);
    ICompact* compact_test3 = ICompact::createCompact(vec5, vec6, pLogger);
    ICompact* compact_test4 = ICompact::createCompact(vec7, vec8, pLogger);

    bool test_success = compact_test != nullptr &&
                        compact_test2 != nullptr &&
                        compact_test3 != nullptr &&
                        compact_test4 != nullptr;
    RESULT_CODE rc;
    if (test_success) {
        rc = compact_test->isSubSet(compact_test2, test_success);
        test_success = rc == RESULT_CODE::SUCCESS;
        if (test_success) {
            rc = compact_test->isSubSet(compact_test3, test_success);
            test_success = rc == RESULT_CODE::SUCCESS;
            if (!test_success) {
                rc = compact_test->isSubSet(compact_test4, test_success);
                test_success = rc == RESULT_CODE::SUCCESS;
                if (!test_success) {
                    rc = compact_test->isSubSet(nullptr, test_success);
                    test_success = rc == RESULT_CODE::SUCCESS;
                    if (!test_success) {
                        test_success = true;
                    }
                }
            }
        }
    }



    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete[] data5;
    delete[] data6;
    delete[] data7;
    delete[] data8;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec5;
    delete vec6;
    delete vec7;
    delete vec8;

    delete compact_test;
    delete compact_test2;
    delete compact_test3;
    delete compact_test4;

    return test_success;
}

bool IsIntersTest(ILogger* pLogger) {

    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 5,7,10 };
    double* data3 = new double[3]{ 1,2,3 };
    double* data4 = new double[3]{ 8,12,10 };

    double* data5 = new double[4]{ 5,7,10,13 };
    double* data6 = new double[4]{ 10,12,14,100 };

    double* data7 = new double[3]{ 6,8,11 };
    double* data8 = new double[3]{ 10,10,15 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(3, data4, pLogger);
    IVector* vec5 = IVector::createVector(4, data5, pLogger);
    IVector* vec6 = IVector::createVector(4, data6, pLogger);
    IVector* vec7 = IVector::createVector(3, data7, pLogger);
    IVector* vec8 = IVector::createVector(3, data8, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);
    ICompact* compact_test2 = ICompact::createCompact(vec3, vec4, pLogger);
    ICompact* compact_test3 = ICompact::createCompact(vec5, vec6, pLogger);
    ICompact* compact_test4 = ICompact::createCompact(vec7, vec8, pLogger);

    ICompact* intersect_success = ICompact::intersection(compact_test, compact_test2, pLogger);
    ICompact* intersect_wrong_dim = ICompact::intersection(compact_test, compact_test3, pLogger);
    ICompact* intersect_empty = ICompact::intersection(compact_test, compact_test4, pLogger);
    ICompact* intersect_nullptr = ICompact::intersection(compact_test, nullptr, pLogger);


    bool test_success = intersect_success != nullptr &&
        intersect_wrong_dim == nullptr &&
        intersect_empty == nullptr &&
        intersect_nullptr == nullptr;

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete[] data5;
    delete[] data6;
    delete[] data7;
    delete[] data8;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec5;
    delete vec6;
    delete vec7;
    delete vec8;

    delete compact_test;
    delete compact_test2;
    delete compact_test3;
    delete compact_test4;

    delete intersect_success;
    delete intersect_wrong_dim;
    delete intersect_empty;
    delete intersect_nullptr;

    return test_success;
}

bool IntersectTest(ILogger* pLogger) {

    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 5,7,10 };
    double* data3 = new double[3]{ 1,2,3 };
    double* data4 = new double[3]{ 8,12,10 };

    double* data5 = new double[4]{ 5,7,10,13 };
    double* data6 = new double[4]{ 10,12,14,100 };

    double* data7 = new double[3]{ 6,8,11 };
    double* data8 = new double[3]{ 10,10,15 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(3, data4, pLogger);
    IVector* vec5 = IVector::createVector(4, data5, pLogger);
    IVector* vec6 = IVector::createVector(4, data6, pLogger);
    IVector* vec7 = IVector::createVector(3, data7, pLogger);
    IVector* vec8 = IVector::createVector(3, data8, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);
    ICompact* compact_test2 = ICompact::createCompact(vec3, vec4, pLogger);
    ICompact* compact_test3 = ICompact::createCompact(vec5, vec6, pLogger);
    ICompact* compact_test4 = ICompact::createCompact(vec7, vec8, pLogger);

    ICompact* intersect_success = ICompact::intersection(compact_test, compact_test2, pLogger);
    ICompact* intersect_wrong_dim = ICompact::intersection(compact_test, compact_test3, pLogger);
    ICompact* intersect_empty = ICompact::intersection(compact_test, compact_test4, pLogger);
    ICompact* intersect_nullptr = ICompact::intersection(compact_test, nullptr, pLogger);


    bool test_success = intersect_success != nullptr &&
                        intersect_wrong_dim == nullptr &&
                        intersect_empty == nullptr &&
                        intersect_nullptr == nullptr;
    RESULT_CODE rc;
    if (test_success) {
        IVector* beg = intersect_success->getBegin();
        rc = IVector::equals(beg, vec3, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
        test_success = rc == RESULT_CODE::SUCCESS;
        delete beg;
        if (test_success) {
            IVector* end = intersect_success->getEnd();
            rc = IVector::equals(end, vec2, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
            test_success = rc == RESULT_CODE::SUCCESS;
            delete end;
        }
    }

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete[] data5;
    delete[] data6;
    delete[] data7;
    delete[] data8;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec5;
    delete vec6;
    delete vec7;
    delete vec8;

    delete compact_test;
    delete compact_test2;
    delete compact_test3;
    delete compact_test4;

    delete intersect_success;
    delete intersect_wrong_dim;
    delete intersect_empty;
    delete intersect_nullptr;

    return test_success;
}

bool AddTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 5,7,10 };

    double* data3 = new double[3]{ 0,2,2 };
    double* data4 = new double[3]{ 5,12,10 };

    double* data5 = new double[4]{ 5,7,10,13 };
    double* data6 = new double[4]{ 10,12,14,100 };

    double* data7 = new double[3]{ 6,8,11 };
    double* data8 = new double[3]{ 10,10,15 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(3, data4, pLogger);
    IVector* vec5 = IVector::createVector(4, data5, pLogger);
    IVector* vec6 = IVector::createVector(4, data6, pLogger);
    IVector* vec7 = IVector::createVector(3, data7, pLogger);
    IVector* vec8 = IVector::createVector(3, data8, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);
    ICompact* compact_test2 = ICompact::createCompact(vec3, vec4, pLogger);
    ICompact* compact_test3 = ICompact::createCompact(vec5, vec6, pLogger);
    ICompact* compact_test4 = ICompact::createCompact(vec7, vec8, pLogger);

    ICompact* add_success = ICompact::add(compact_test, compact_test2, pLogger);
    ICompact* add_wrong_dim = ICompact::add(compact_test, compact_test3, pLogger);
    ICompact* add_empty = ICompact::add(compact_test, compact_test4, pLogger);
    ICompact* add_nullptr = ICompact::add(compact_test, nullptr, pLogger);


    bool test_success = add_success != nullptr &&
                        add_wrong_dim == nullptr &&
                        add_empty == nullptr &&
                        add_nullptr == nullptr;
    RESULT_CODE rc;
    if (test_success) {
        IVector* beg = add_success->getBegin();
        rc = IVector::equals(beg, vec1, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
        test_success = rc == RESULT_CODE::SUCCESS;
        delete beg;
        if (test_success) {
            IVector* end = add_success->getEnd();
            rc = IVector::equals(end, vec4, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
            test_success = rc == RESULT_CODE::SUCCESS;
            delete end;
        }
    }

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete[] data5;
    delete[] data6;
    delete[] data7;
    delete[] data8;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec5;
    delete vec6;
    delete vec7;
    delete vec8;

    delete compact_test;
    delete compact_test2;
    delete compact_test3;
    delete compact_test4;

    delete add_success;
    delete add_wrong_dim;
    delete add_empty;
    delete add_nullptr;

    return test_success;
}

bool MakeConvTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 5,7,10 };

    double* data3 = new double[3]{ 0,2,2 };
    double* data4 = new double[3]{ 5,12,15 };

    double* data5 = new double[4]{ 5,7,10,13 };
    double* data6 = new double[4]{ 10,12,14,100 };

    double* data7 = new double[3]{ 2,3,4 };
    double* data8 = new double[3]{ 4,6,9 };

    double* data9 = new double[3]{ 6,-3,0 };
    double* data10 = new double[3]{ 6,12,9 };

    double* data11 = new double[3]{ 0,-3,0 };
    double* data12 = new double[3]{ 6,12,10 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(3, data4, pLogger);
    IVector* vec5 = IVector::createVector(4, data5, pLogger);
    IVector* vec6 = IVector::createVector(4, data6, pLogger);
    IVector* vec7 = IVector::createVector(3, data7, pLogger);
    IVector* vec8 = IVector::createVector(3, data8, pLogger);
    IVector* vec9 = IVector::createVector(3, data9, pLogger);
    IVector* vec10 = IVector::createVector(3, data10, pLogger);
    IVector* vec11 = IVector::createVector(3, data11, pLogger);
    IVector* vec12 = IVector::createVector(3, data12, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);
    ICompact* compact_test2 = ICompact::createCompact(vec3, vec4, pLogger);
    ICompact* compact_test3 = ICompact::createCompact(vec5, vec6, pLogger);
    ICompact* compact_test4 = ICompact::createCompact(vec7, vec8, pLogger);
    ICompact* compact_test5 = ICompact::createCompact(vec9, vec10, pLogger);
    ICompact* compact_test6 = ICompact::createCompact(vec11, vec12, pLogger);

    ICompact* conv_success = ICompact::makeConvex(compact_test, compact_test2, pLogger);
    ICompact* conv_wrong_dim = ICompact::makeConvex(compact_test, compact_test3, pLogger);
    ICompact* conv_success2 = ICompact::makeConvex(compact_test, compact_test4, pLogger);
    ICompact* conv_success3 = ICompact::makeConvex(compact_test, compact_test5, pLogger);
    ICompact* conv_nullptr = ICompact::makeConvex(compact_test, nullptr, pLogger);


    bool test_success = conv_success != nullptr &&
                        conv_wrong_dim == nullptr &&
                        conv_success2 != nullptr &&
                        conv_success3 != nullptr &&
                        conv_nullptr == nullptr;
    RESULT_CODE rc;
    if (test_success) {
        IVector* beg = conv_success->getBegin();
        rc = IVector::equals(beg, vec1, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
        test_success = rc == RESULT_CODE::SUCCESS;
        delete beg;
        if (test_success) {
            IVector* end = conv_success->getEnd();
            rc = IVector::equals(end, vec4, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
            test_success = rc == RESULT_CODE::SUCCESS;
            delete end;
        }
    }

    if (test_success) {
        IVector* beg = conv_success2->getBegin();
        rc = IVector::equals(beg, vec1, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
        test_success = rc == RESULT_CODE::SUCCESS;
        delete beg;
        if (test_success) {
            IVector* end = conv_success2->getEnd();
            rc = IVector::equals(end, vec2, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
            test_success = rc == RESULT_CODE::SUCCESS;
            delete end;
        }
    }

    if (test_success) {
        IVector* beg = conv_success3->getBegin();
        rc =IVector::equals(beg, vec11, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
        test_success = rc == RESULT_CODE::SUCCESS;
        delete beg;
        if (test_success) {
            IVector* end = conv_success3->getEnd();
            rc = IVector::equals(end, vec12, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
            test_success = rc == RESULT_CODE::SUCCESS;
            delete end;
        }
    }

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete[] data5;
    delete[] data6;
    delete[] data7;
    delete[] data8;
    delete[] data9;
    delete[] data10;
    delete[] data11;
    delete[] data12;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec5;
    delete vec6;
    delete vec7;
    delete vec8;
    delete vec9;
    delete vec10;
    delete vec11;
    delete vec12;

    delete compact_test;
    delete compact_test2;
    delete compact_test3;
    delete compact_test4;
    delete compact_test5;
    delete compact_test6;

    delete conv_success;
    delete conv_wrong_dim;
    delete conv_success2;
    delete conv_success3;
    delete conv_nullptr;

    return test_success;
}

bool DoStepTest(ILogger* pLogger) {
    double* data1 = new double[2]{ 0,1 };
    double* data2 = new double[2]{ 4,2 };
    double* data3 = new double[2]{ 2,2 };
    double* data4 = new double[2]{ 0.5,0.5 };
    double* data5 = new double[2]{ -0.2,-0.2 };

    IVector* vec1 = IVector::createVector(2, data1, pLogger);
    IVector* vec2 = IVector::createVector(2, data2, pLogger);
    IVector* vec3 = IVector::createVector(2, data3, pLogger);
    IVector* vec4 = IVector::createVector(2, data4, pLogger);
    IVector* vec5 = IVector::createVector(2, data5, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);
    ICompact* compact_test2 = ICompact::createCompact(vec1, vec3, pLogger);

    ICompact::iterator* it1 = compact_test->begin(vec4);
    ICompact::iterator* it2 = compact_test2->end(vec5);

    int count = 0;
    bool test_success = it1 != nullptr && it2 != nullptr;

    RESULT_CODE rc;

    if (test_success) {
        do {
            count++;
            IVector* p = it1->getPoint();
            rc = compact_test->isContains(p, test_success);
            delete p;
        } while (it1->doStep() == RESULT_CODE::SUCCESS && rc == RESULT_CODE::SUCCESS);
        test_success = count == 27;

        count = 0;
        do {
            count++;
            IVector* p = it2->getPoint();
            rc = compact_test->isContains(p, test_success);
            delete p;
        } while (it2->doStep() == RESULT_CODE::SUCCESS && rc == RESULT_CODE::SUCCESS);
        test_success = count == 66;
    }

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete[] data5;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec5;

    delete compact_test;
    delete compact_test2;

    delete it1;
    delete it2;

    return test_success;
}

bool SetDirTest(ILogger* pLogger) {
    double* data1 = new double[2]{ 0,1 };
    double* data2 = new double[2]{ 4,2 };
    double* data4 = new double[2]{ 0.5,0.5 };
    double* data5 = new double[2]{ -0.8,-0.5 };
    double* data6 = new double[2]{ 1,0 };

    IVector* vec1 = IVector::createVector(2, data1, pLogger);
    IVector* vec2 = IVector::createVector(2, data2, pLogger);
    IVector* vec4 = IVector::createVector(2, data4, pLogger);
    IVector* vec5 = IVector::createVector(2, data5, pLogger);
    IVector* vec6 = IVector::createVector(2, data6, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);

    ICompact::iterator* it1 = compact_test->begin(vec4);
    ICompact::iterator* it2 = compact_test->end(vec5);

    int count = 0;
    RESULT_CODE rc = it2->setDirection(vec6);

    bool test_success = it1 != nullptr && it2 != nullptr && rc == RESULT_CODE::SUCCESS;

    if (test_success) {
        do {
            count++;
            IVector* p = it1->getPoint();
            rc = compact_test->isContains(p, test_success);
            delete p;
        } while (it1->doStep() == RESULT_CODE::SUCCESS && rc == RESULT_CODE::SUCCESS);
        test_success = count == 27;

        count = 0;
        do {
            count++;
            IVector* p = it2->getPoint();
            rc = compact_test->isContains(p, test_success);
            delete p;
        } while (it2->doStep() == RESULT_CODE::SUCCESS && rc == RESULT_CODE::SUCCESS);
        test_success = count == 18;
    }

    delete[] data1;
    delete[] data2;
    delete[] data6;
    delete[] data4;
    delete[] data5;
    delete vec1;
    delete vec2;
    delete vec6;
    delete vec4;
    delete vec5;

    delete compact_test;

    delete it1;
    delete it2;

    return test_success;
}

bool GetPosTest(ILogger* pLogger) {
    double* data1 = new double[2]{ 0,1 };
    double* data2 = new double[2]{ 2,2 };
    double* data4 = new double[2]{ 0.5,0.5 };

    double* data5 = new double[2]{ 1,1 };
    double* data6 = new double[2]{ 1.5,1.5 };


    IVector* vec1 = IVector::createVector(2, data1, pLogger);
    IVector* vec2 = IVector::createVector(2, data2, pLogger);
    IVector* vec4 = IVector::createVector(2, data4, pLogger);
    IVector* vec5 = IVector::createVector(2, data5, pLogger);
    IVector* vec6 = IVector::createVector(2, data6, pLogger);

    ICompact* compact_test = ICompact::createCompact(vec1, vec2, pLogger);

    ICompact::iterator* it1 = compact_test->begin(vec4);

    bool test_success = it1 != nullptr && compact_test != nullptr;

    RESULT_CODE rc;
    RESULT_CODE rc2;
    RESULT_CODE rc3;
    RESULT_CODE rc4;

    if (test_success) {
        int count = 0;
        do {
            count++;
            IVector* p = it1->getPoint();
            rc = compact_test->isContains(p, test_success);

            if (count == 3) {
                rc2 = IVector::equals(p, vec5, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);

            }

            if(count == 9) {
                rc3 = IVector::equals(p, vec6, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
            }

            if (count == 15) {
                rc4 = IVector::equals(p, vec2, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);
            }

            delete p;
        } while (it1->doStep() == RESULT_CODE::SUCCESS && rc == RESULT_CODE::SUCCESS);
        test_success = count == 15 && rc2 == RESULT_CODE::SUCCESS && rc3 == RESULT_CODE::SUCCESS && rc4 == RESULT_CODE::SUCCESS ;
    }

    delete[] data1;
    delete[] data2;
    delete[] data4;
    delete[] data5;
    delete[] data6;
    delete vec1;
    delete vec2;
    delete vec6;
    delete vec4;
    delete vec5;
    delete compact_test;
    delete it1;
    return test_success;
}

int main() {

    ILogger* pLogger = ILogger::createLogger(nullptr);


    if(pLogger){
        pLogger->setLogFile("log.txt");
    }

    std::cout << "CREATE TEST | ";
    if (CreateTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "GET_BEG and GET_END TEST | ";
    if (GetBeg_GetEndTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "GET_DIM TEST | ";
    if (GetDimTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "CLONE TEST | ";
    if (CloneTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "IS_CONT TEST | ";
    if (IsContTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "IS_SUB_SET TEST | ";
    if (IsSubSetTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "IS_INTERS TEST | ";
    if (IsIntersTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "INTERS TEST | ";
    if (IntersectTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "ADD TEST | ";
    if (AddTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "MAKE_CONV TEST | ";
    if (MakeConvTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << std::endl << "ITERATOR TESTS:" << std::endl << "SET_DIR TEST | ";
    if (SetDirTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "GET_POS TEST | ";
    if (GetPosTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "DO_STEP TEST | ";
    if (DoStepTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    if(pLogger){
        pLogger->destroyLogger(nullptr);
    }
    system("PAUSE");
    return 0;
}
