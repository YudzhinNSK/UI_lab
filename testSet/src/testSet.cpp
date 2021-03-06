#include "ILogger.h"
#include "IVector.h"
#include "ISet.h"
#include <cmath>
#include <iostream>

bool InsertTest(ILogger* pLogger) {

    double* data1 = new double[3]{ 0, 1, 2 };
    double* data2 = new double[4]{ 0, 1, 2, 3 };
    double* data3 = new double[3]{ 0, 2, 4 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(4, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);

    ISet* set_test = ISet::createSet(pLogger);

    bool test_success = true;
    bool test_fail = false;


    RESULT_CODE rc1 = set_test->insert(vec1, IVector::NORM::NORM_1, 0.001); //success
    test_success = rc1 == RESULT_CODE::SUCCESS;

    rc1 = set_test->insert(vec3, IVector::NORM::NORM_1, 0.001); //success
    test_success = rc1 == RESULT_CODE::SUCCESS && set_test->getSize() == 2;

    rc1 = set_test->insert(vec3, IVector::NORM::NORM_1, 0.001); //success (add equal vector size must not change)
    test_success = rc1 == RESULT_CODE::SUCCESS && set_test->getSize() == 2;

    rc1 = set_test->insert(vec2, IVector::NORM::NORM_1, 0.001); //wrong dim
    test_fail = rc1 != RESULT_CODE::SUCCESS && set_test->getSize() == 2;

    rc1 = set_test->insert(nullptr, IVector::NORM::NORM_1, 0.001); //nullptr
    test_fail = rc1 != RESULT_CODE::SUCCESS && set_test->getSize() == 2;

    rc1 = set_test->insert(nullptr, IVector::NORM::NORM_1, NAN); //NAN
    test_fail = rc1 != RESULT_CODE::SUCCESS && set_test->getSize() == 2;

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete vec1;
    delete vec2;
    delete vec3;

    set_test->clear();
    delete set_test;

    return test_fail && test_success;
}

bool GetTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0, 1, 2 };
    double* data2 = new double[3]{ 0, 1, 10};
    double* data3 = new double[3]{ 0, 2, 4 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);

    ISet* set_test = ISet::createSet(pLogger);

    bool test_success = true;
    bool test_fail = false;


    RESULT_CODE rc1 = set_test->insert(vec1, IVector::NORM::NORM_1, 0.001);
    rc1 = set_test->insert(vec2, IVector::NORM::NORM_1, 0.001);
    rc1 = set_test->insert(vec3, IVector::NORM::NORM_1, 0.001);

    delete vec1;

    rc1 = set_test->get(vec1, 2); // succees
    RESULT_CODE rc2 = IVector::equals(vec1, vec3, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);

    test_success = rc1 == RESULT_CODE::SUCCESS && rc2 == RESULT_CODE::SUCCESS; // must be true

    rc1 = set_test->get(vec1, 10); // wrong dim
    test_fail = rc1 != RESULT_CODE::SUCCESS;


    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete vec1;
    delete vec2;
    delete vec3;

    set_test->clear();
    delete set_test;

    return test_fail && test_success;
}

bool CreateTest(ILogger* pLogger) {
    ISet* test = ISet::createSet(pLogger);

    if (test != nullptr) {
        delete test;
        return true;
    }else{
        delete test;
        return false;
    }

}

bool GetDimTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0, 1, 2 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);

    ISet* set_test = ISet::createSet(pLogger);

    bool test_success = true;


    RESULT_CODE rc1 = set_test->insert(vec1, IVector::NORM::NORM_1, 0.001);
    rc1 = set_test->insert(vec1, IVector::NORM::NORM_1, 0.001);

    test_success = rc1 == RESULT_CODE::SUCCESS && set_test->getDim() == 3;


    delete[] data1;
    delete vec1;

    set_test->clear();
    delete set_test;

    return test_success;
}

bool GetSizeTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0, 1, 2 };
    double* data2 = new double[3]{ 0, 1, 10 };
    double* data3 = new double[3]{ 0, 2, 4 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);

    ISet* set_test = ISet::createSet(pLogger);

    bool test_success = true;


    RESULT_CODE rc1 = set_test->insert(vec1, IVector::NORM::NORM_1, 0.001);
    rc1 = set_test->insert(vec2, IVector::NORM::NORM_1, 0.001);

    test_success = rc1 == RESULT_CODE::SUCCESS && set_test->getSize() == 2;

    rc1 = set_test->insert(vec3, IVector::NORM::NORM_1, 0.001);

    test_success = rc1 == RESULT_CODE::SUCCESS && set_test->getSize() == 3;

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete vec1;
    delete vec2;
    delete vec3;

    set_test->clear();
    delete set_test;

    return test_success;
}

bool ClearTest(ILogger* pLogger) {

    double* data1 = new double[3]{ 0, 1, 2 };
    double* data2 = new double[3]{ 0, 1, 10 };
    double* data3 = new double[3]{ 10, 9, 8 };
    double* data4 = new double[3]{ 11, 9, 8 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(3, data4, pLogger);

    ISet* set_test = ISet::createSet(pLogger);


    bool test_success = true;

    set_test->insert(vec1, IVector::NORM::NORM_1, 0.001);
    set_test->insert(vec2, IVector::NORM::NORM_1, 0.001);
    set_test->insert(vec3, IVector::NORM::NORM_1, 0.001);
    set_test->insert(vec4, IVector::NORM::NORM_1, 0.001);

    set_test->clear();

    test_success = set_test->getSize() == 0 && set_test->getDim() == 0;

    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;

    delete set_test;

    return test_success;
}

bool Erase_Ind_Test(ILogger* pLogger) {
    double* data1 = new double[3]{ 0, 1, 2 };
    double* data2 = new double[3]{ 0, 1, 10 };
    double* data3 = new double[3]{ 10, 9, 8 };
    double* data4 = new double[3]{ 11, 9, 8 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(3, data4, pLogger);

    ISet* set_test = ISet::createSet(pLogger);


    bool test_success = true;

    set_test->insert(vec1, IVector::NORM::NORM_1, 0.001);
    set_test->insert(vec2, IVector::NORM::NORM_1, 0.001);
    set_test->insert(vec3, IVector::NORM::NORM_1, 0.001);
    set_test->insert(vec4, IVector::NORM::NORM_1, 0.001);

    set_test->erase(1);

    RESULT_CODE rc = set_test->get(vec1, vec2, IVector::NORM::NORM_1, 0.001);

    test_success = set_test->getSize() == 3 && rc != RESULT_CODE::SUCCESS;

    rc = set_test->erase(10);

    test_success = rc != RESULT_CODE::SUCCESS;


    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;

    delete set_test;

    return test_success;
}

bool Erase_Norm_Test(ILogger* pLogger) {
    double* data1 = new double[3]{ 0, 1, 2 };
    double* data2 = new double[3]{ 0, 0, 3 };
    double* data3 = new double[3]{ 1, 1, 1 };
    double* data4 = new double[3]{ 11, 9, 8 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(3, data4, pLogger);
    IVector* test = nullptr;

    ISet* set_test = ISet::createSet(pLogger);


    bool test_success = true;

    set_test->insert(vec1, IVector::NORM::NORM_2, 0.001);
    set_test->insert(vec2, IVector::NORM::NORM_2, 0.001);
    set_test->insert(vec3, IVector::NORM::NORM_2, 0.001);
    set_test->insert(vec4, IVector::NORM::NORM_2, 0.001);

    set_test->erase(vec1, IVector::NORM::NORM_1, 0.001);

    RESULT_CODE rc = set_test->get(test, vec1, IVector::NORM::NORM_1, 0.001);

    RESULT_CODE rc2 = set_test->get(test, vec2, IVector::NORM::NORM_1, 0.001);
    delete test;
    RESULT_CODE rc3 = set_test->get(test, vec3, IVector::NORM::NORM_1, 0.001);
    delete test;

    test_success = set_test->getSize() == 1 && rc != RESULT_CODE::SUCCESS && rc2 != RESULT_CODE::SUCCESS && rc3 != RESULT_CODE::SUCCESS;

    rc = set_test->erase(vec1, IVector::NORM::NORM_1, NAN);

    test_success = rc != RESULT_CODE::SUCCESS;


    delete[] data1;
    delete[] data2;
    delete[] data3;
    delete[] data4;
    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;

    set_test->clear();
    delete set_test;

    return test_success;
}

bool CloneTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0, 1, 2 };
    double* data2 = new double[3]{ 0, 1, 10 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);

    ISet* set_test = ISet::createSet(pLogger);

    bool test_size = true;
    bool test_eq = true;


    RESULT_CODE rc1 = set_test->insert(vec1, IVector::NORM::NORM_1, 0.001);
    rc1 = set_test->insert(vec2, IVector::NORM::NORM_1, 0.001);

    ISet* set_test_clone = set_test->clone();

    test_size = set_test->getSize() == set_test_clone->getSize();
    if (test_size) {
        for (size_t i = 0; i < set_test->getSize(); i++) {
            IVector* vec3;
            set_test->get(vec3, i);
            IVector* vec4;
            set_test_clone->get(vec4, i);
            rc1 = IVector::equals(vec3, vec4, IVector::NORM::NORM_1, 0.001, &test_eq, pLogger);
            test_eq = rc1 == RESULT_CODE::SUCCESS;
            if (!test_eq) {
                break;
            }
            delete vec3;
            delete vec4;
        }
    }

    delete[] data1;
    delete[] data2;
    delete vec1;
    delete vec2;

    set_test->clear();
    delete set_test;

    set_test_clone->clear();
    delete set_test_clone;

    return test_size && test_eq;
}

bool AddTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0, 1, 2 };
    double* data2 = new double[3]{ 0, 1, 10 };
    double* data5 = new double[4]{ 0, 100, 10, 11 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec5 = IVector::createVector(4, data5, pLogger);

    ISet* set_test = ISet::createSet(pLogger); // dim 3
    ISet* set_test3 = ISet::createSet(pLogger); // dim 3
    ISet* set_test4 = ISet::createSet(pLogger); // dim 4


    bool test_success = true;
    bool test_eq = false;


    set_test->insert(vec1, IVector::NORM::NORM_1, 0.001);
    set_test->insert(vec2, IVector::NORM::NORM_1, 0.001);

    set_test3->insert(vec2, IVector::NORM::NORM_1, 0.001);

    set_test4->insert(vec5, IVector::NORM::NORM_1, 0.001);

    ISet* set_add_success = ISet::add(set_test, set_test3, IVector::NORM::NORM_1, 0.001, pLogger);
    ISet* set_add_wrong_dim = ISet::add(set_test, set_test4, IVector::NORM::NORM_1, 0.001, pLogger);
    ISet* set_add_nullptr = ISet::add(set_test, nullptr, IVector::NORM::NORM_1, 0.001, pLogger);
    ISet* set_add_nan = ISet::add(set_test, set_test3, IVector::NORM::NORM_1, NAN, pLogger);

    test_success = !set_add_wrong_dim && !set_add_nullptr && !set_add_nan && set_add_success->getSize() == 2;
    RESULT_CODE rc;
    if (test_success) {
        for (size_t i = 0; i < 2; i++) {
            IVector* vec6;
            set_add_success->get(vec6, i);
            rc = IVector::equals(vec2, vec6, IVector::NORM::NORM_1, 0.001, &test_eq, pLogger);
            test_eq = rc == RESULT_CODE::SUCCESS;
            delete vec6;
            if (test_eq) {
                break;
            }
        }
        if (test_eq) {
            for (size_t i = 0; i < 2; i++) {
                IVector* vec6;
                set_add_success->get(vec6, i);
                rc = IVector::equals(vec1, vec6, IVector::NORM::NORM_1, 0.001, &test_eq, pLogger);
                test_eq = rc == RESULT_CODE::SUCCESS;
                delete vec6;
                if (test_eq) {
                    break;
                }
            }
        }


    }

    delete[] data1;
    delete[] data2;
    delete[] data5;
    delete vec1;
    delete vec2;
    delete vec5;

    set_test->clear();
    delete set_test;

    set_test3->clear();
    delete set_test3;

    set_test4->clear();
    delete set_test4;

    set_add_success->clear();
    delete set_add_success;

    return test_eq && test_success;
}

bool IntersectTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0, 1, 2 };
    double* data2 = new double[3]{ 0, 1, 10 };
    double* data3 = new double[3]{ 10, 9, 8 };
    double* data4 = new double[3]{ 11, 9, 8 };
    double* data5 = new double[4]{ 0, 100, 10, 11 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(3, data4, pLogger);
    IVector* vec5 = IVector::createVector(4, data5, pLogger);

    ISet* set_test = ISet::createSet(pLogger); // dim 3
    ISet* set_test3 = ISet::createSet(pLogger); // dim 3
    ISet* set_test4 = ISet::createSet(pLogger); // dim 4


    bool test_success = true;
    bool test_eq = false;


    set_test->insert(vec1, IVector::NORM::NORM_1, 0.001);
    set_test->insert(vec2, IVector::NORM::NORM_1, 0.001);
    set_test->insert(vec3, IVector::NORM::NORM_1, 0.001);

    set_test3->insert(vec2, IVector::NORM::NORM_1, 0.001);
    set_test3->insert(vec3, IVector::NORM::NORM_1, 0.001);
    set_test3->insert(vec4, IVector::NORM::NORM_1, 0.001);

    set_test4->insert(vec5, IVector::NORM::NORM_1, 0.001);

    ISet* set_intersect_success = ISet::intersect(set_test, set_test3, IVector::NORM::NORM_1, 0.001, pLogger);
    ISet* set_intersect_wrong_dim = ISet::intersect(set_test, set_test4, IVector::NORM::NORM_1, 0.001, pLogger);
    ISet* set_intersect_nullptr = ISet::intersect(set_test, nullptr, IVector::NORM::NORM_1, 0.001, pLogger);
    ISet* set_intersect_nan = ISet::intersect(set_test, set_test3, IVector::NORM::NORM_1, NAN, pLogger);

    test_success = !set_intersect_wrong_dim && !set_intersect_nullptr && !set_intersect_nan && set_intersect_success->getSize() == 2;
    RESULT_CODE rc;

    if (test_success) {
        for (size_t i = 0; i < 2; i++) {
            IVector* vec6;
            set_intersect_success->get(vec6, i);
            rc = IVector::equals(vec2, vec6, IVector::NORM::NORM_1, 0.001, &test_eq, pLogger);
            test_eq = rc == RESULT_CODE::SUCCESS;
            delete vec6;
            if (test_eq) {
                break;
            }
        }
        if (test_eq) {
            for (size_t i = 0; i < 2; i++) {
                IVector* vec6;
                set_intersect_success->get(vec6, i);
                rc = IVector::equals(vec3, vec6, IVector::NORM::NORM_1, 0.001, &test_eq, pLogger);
                test_eq = rc == RESULT_CODE::SUCCESS;
                delete vec6;
                if (test_eq) {
                    break;
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

    set_test->clear();
    delete set_test;

    set_test3->clear();
    delete set_test3;

    set_test4->clear();
    delete set_test4;

    set_intersect_success->clear();
    delete set_intersect_success;

    return test_eq && test_success;
}

bool Get_Norm_Test(ILogger* pLogger) {
    double* data1 = new double[3]{ 0, 1, 2 };
    double* data2 = new double[3]{ 0, 1, 10 };
    double* data3 = new double[3]{ 0, 2, 4 };
    double* data4 = new double[3]{ 0, 100, 10 };
    double* data5 = new double[4]{ 0, 100, 10, 11};

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(3, data3, pLogger);
    IVector* vec4 = IVector::createVector(3, data4, pLogger);
    IVector* vec5 = IVector::createVector(4, data5, pLogger);

    ISet* set_test = ISet::createSet(pLogger);

    bool test_success = true;
    bool test_fail = false;


    RESULT_CODE rc1 = set_test->insert(vec1, IVector::NORM::NORM_1, 0.001);
    rc1 = set_test->insert(vec2, IVector::NORM::NORM_1, 0.001);
    rc1 = set_test->insert(vec3, IVector::NORM::NORM_1, 0.001);

    delete vec1;

    rc1 = set_test->get(vec1,vec3,IVector::NORM::NORM_1, 0.001); // succees
    RESULT_CODE rc2 = IVector::equals(vec1, vec3, IVector::NORM::NORM_1, 0.001, &test_success, pLogger);

    test_success = rc1 == RESULT_CODE::SUCCESS && rc2 == RESULT_CODE::SUCCESS; // must be true

    rc1 = set_test->get(vec1, nullptr, IVector::NORM::NORM_1, 0.001); // nullptr
    test_fail = rc1 != RESULT_CODE::SUCCESS;

    rc1 = set_test->get(vec1, vec5, IVector::NORM::NORM_1, 0.001); // wrong dim
    test_fail = rc1 != RESULT_CODE::SUCCESS;

    rc1 = set_test->get(vec1, vec4, IVector::NORM::NORM_1, 0.001); // not found
    test_fail = rc1 != RESULT_CODE::SUCCESS;

    rc1 = set_test->get(vec1, vec3, IVector::NORM::NORM_1, NAN); // NAN
    test_fail = rc1 != RESULT_CODE::SUCCESS;



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

    set_test->clear();
    delete set_test;

    return test_fail && test_success;
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

    std::cout << "INSERT TEST | ";
    if (InsertTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "GET TEST | ";
    if (GetTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "GET_NORM TEST | ";
    if (Get_Norm_Test(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "GET DIM TEST | ";
    if (GetDimTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "GET SIZE TEST | ";
    if (GetSizeTest(pLogger)) {
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

    std::cout << "ADD TEST | ";
    if (AddTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "INTERSECT TEST | ";
    if (IntersectTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "CLEAR TEST | ";
    if (ClearTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "ERASE_IND TEST | ";
    if (Erase_Ind_Test(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "ERASE_NORM TEST | ";
    if (Erase_Norm_Test(pLogger)) {
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
