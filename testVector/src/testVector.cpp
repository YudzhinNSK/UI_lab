#include "ILogger.h"
#include "IVector.h"
#include <cmath>
#include <iostream>

bool CreateTest(ILogger* pLogger) {

    double* data1 = new double[3]{ 0,1,2 };
    double* data3 = new double[3]{ NAN,NAN,NAN };

    IVector* vec1 = IVector::createVector(3, data1, pLogger); // SUCCESS

    IVector* vec2 = IVector::createVector(0, data1, pLogger); // WRONG_DIM -> nullptr

    IVector* vec3 = IVector::createVector(3, nullptr, pLogger); // BAD_REFERENCE -> nullptr

    IVector* vec4 = IVector::createVector(3, data3, pLogger); // NAN_VALUE -> nullptr

    if (vec1 && !vec2 && !vec3 && !vec4) {
        delete[] data1;
        delete[] data3;
        delete vec1;
        delete vec2;
        delete vec3;
        delete vec4;
        return true;
    }
    else {
        delete[] data1;
        delete[] data3;
        delete vec1;
        delete vec2;
        delete vec3;
        delete vec4;
        return false;
    }
}

bool AddTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 0,3,4 };
    double* data3 = new double[4]{ 2,1,3,0 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(4, data3, pLogger);

    IVector* add_success = IVector::add(vec1, vec2, pLogger); // SUCCESS
    IVector* add_wrong_dim = IVector::add(vec1, vec3, pLogger); // nullptr wrong_dim
    IVector* add_nullptr = IVector::add(vec1, nullptr, pLogger); // nullptr

    bool test_success = true;
    for (size_t i = 0; i < 3; i++) {
        if (vec1->getCoord(i) + vec2->getCoord(i) != add_success->getCoord(i)) {
            test_success = false;
            break;
        }
    }


    if (test_success && (add_success != nullptr) && !add_wrong_dim && !add_nullptr) {
        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete vec1;
        delete vec2;
        delete vec3;
        delete add_success;
        delete add_wrong_dim;
        delete add_nullptr;
        return true;
    }
    else {
        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete vec1;
        delete vec2;
        delete vec3;
        delete add_success;
        delete add_wrong_dim;
        delete add_nullptr;
        return false;
    }
}

bool SubTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 0,3,4 };
    double* data3 = new double[4]{ 2,1,3,0 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(4, data3, pLogger);

    IVector* sub_success = IVector::sub(vec1, vec2, pLogger); // SUCCESS
    IVector* sub_wrong_dim = IVector::sub(vec1, vec3, pLogger); // nullptr wrong_dim
    IVector* sub_nullptr = IVector::sub(vec1, nullptr, pLogger); // nullptr

    bool test_success = true;
    for (size_t i = 0; i < 3; i++) {
        if (vec1->getCoord(i) - vec2->getCoord(i) != sub_success->getCoord(i)) {
            test_success = false;
            break;
        }
    }


    if (test_success && (sub_success != nullptr) && !sub_wrong_dim  && !sub_nullptr) {
        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete vec1;
        delete vec2;
        delete vec3;
        delete sub_success;
        delete sub_wrong_dim;
        delete sub_nullptr;
        return true;
    }
    else {
        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete vec1;
        delete vec2;
        delete vec3;
        delete sub_success;
        delete sub_wrong_dim;
        delete sub_nullptr;
        return false;
    }
}

bool ScalarTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };
    double* data2 = new double[3]{ 0,3,4 };
    double* data3 = new double[4]{ 2,1,3,0 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec2 = IVector::createVector(3, data2, pLogger);
    IVector* vec3 = IVector::createVector(4, data3, pLogger);

    double mul_success = IVector::mul(vec1, vec2, pLogger); // SUCCESS
    double mul_wrong_dim = IVector::mul(vec1, vec3, pLogger); // NAN wrong_dim
    double mul_nullptr = IVector::mul(vec1, nullptr, pLogger); // NAN

    bool test_success = true;
    if (mul_success != 11) {
        test_success = false;
    }


    if (test_success && std::isnan(mul_wrong_dim) && std::isnan(mul_nullptr)) {
        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete vec1;
        delete vec2;
        delete vec3;
        return true;
    }
    else {
        delete[] data1;
        delete[] data2;
        delete[] data3;
        delete vec1;
        delete vec2;
        delete vec3;
        return false;
    }
}

bool MulTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);

    double param = 3;

    IVector* mul_success = IVector::mul(vec1, param, pLogger); // SUCCESS
    IVector* mul_wrong_dim = IVector::mul(vec1, NAN, pLogger); // nullptr wrong_dim
    IVector* mul_nullptr = IVector::mul(nullptr, param, pLogger); // nullptr

    bool test_success = true;
    for (size_t i = 0; i < 3; i++) {
        if (vec1->getCoord(i) * param != mul_success->getCoord(i)) {
            test_success = false;
            break;
        }
    }


    if (test_success && (mul_success != nullptr) && !mul_wrong_dim && !mul_nullptr) {
        delete[] data1;
        delete vec1;
        delete mul_success;
        delete mul_wrong_dim;
        delete mul_nullptr;
        return true;
    }
    else {
        delete[] data1;
        delete vec1;
        delete mul_success;
        delete mul_wrong_dim;
        delete mul_nullptr;
        return false;
    }
}

bool EQTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,0,3 };

    double* data3 = new double[4]{ 0,0,2,0 };

    double* data5 = new double[2]{ 0,10 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec3 = IVector::createVector(4, data3, pLogger);
    IVector* vec5 = IVector::createVector(2, data5, pLogger);

    double tolerance = 0.01;

    bool test_success = true;
    bool test_fail = true;

    RESULT_CODE eq_success_norm1 = IVector::equals(vec1, vec1, IVector::NORM::NORM_1, tolerance, &test_success, pLogger); //
    RESULT_CODE eq_success_norm2 = IVector::equals(vec3, vec3, IVector::NORM::NORM_2, tolerance, &test_success, pLogger); // SUCCESS
    RESULT_CODE eq_success_norm3 = IVector::equals(vec5, vec5, IVector::NORM::NORM_INF, tolerance, &test_success, pLogger); //

    RESULT_CODE eq_wrong_dim = IVector::equals(vec5, vec1, IVector::NORM::NORM_INF, tolerance, &test_fail, pLogger); // FALSE wrong_dim
    RESULT_CODE eq_nan = IVector::equals(vec5, vec1, IVector::NORM::NORM_INF, NAN, &test_fail, pLogger); // FALSE NAN tolerance
    RESULT_CODE eq_nullptr = IVector::equals(vec5, nullptr, IVector::NORM::NORM_INF, tolerance, &test_fail, pLogger); // FALSE nullptr

    test_success = eq_success_norm1 == RESULT_CODE::SUCCESS && eq_success_norm2 == RESULT_CODE::SUCCESS && eq_success_norm3 == RESULT_CODE::SUCCESS;
    test_fail = eq_wrong_dim != RESULT_CODE::SUCCESS && eq_nan != RESULT_CODE::SUCCESS && eq_nullptr != RESULT_CODE::SUCCESS;

    if (test_success && test_fail) {
        delete[] data1;
        delete[] data3;
        delete[] data5;
        delete vec1;
        delete vec3;
        delete vec5;
        return true;
    }
    else {
        delete[] data1;
        delete[] data3;
        delete[] data5;
        delete vec1;
        delete vec3;
        delete vec5;
        return false;
    }
}

bool GetCordTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);

    int param = 2;

    double getCoord_success = vec1->getCoord(param); // SUCCESS
    double getCoord_out_of_bounds = vec1->getCoord(10); // NAN OUT_OF_BOUNDS


    if ((getCoord_success == 2.0) && std::isnan(getCoord_out_of_bounds)) {
        delete[] data1;
        delete vec1;
        return true;
    }
    else {
        delete[] data1;

    delete vec1;
        return false;
    }
}

bool GetDimTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);

    size_t param =	vec1->getDim();

    if (param == 3) {
        delete[] data1;
        delete vec1;
        return true;
    }
    else {
        delete[] data1;

        delete vec1;
        return false;
    }
}

bool SetCordTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 0,1,2 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);

    int param = 2;
    vec1->setCoord(param, 2.5);
    double getCoord_success = vec1->getCoord(param); // SUCCESS
    RESULT_CODE getCoord_out_of_bounds = vec1->setCoord(10, 4); // OUT_OF_BOUNDS


    if ((getCoord_success == 2.5) && getCoord_out_of_bounds != RESULT_CODE::SUCCESS) {
        delete[] data1;
        delete vec1;
        return true;
    }
    else {
        delete[] data1;

        delete vec1;
        return false;
    }
}

bool NormTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 5,1,3 };

    double* data3 = new double[4]{ 0,0,2,0 };

    double* data5 = new double[2]{ 100,10 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec3 = IVector::createVector(4, data3, pLogger);
    IVector* vec5 = IVector::createVector(2, data5, pLogger);

    double norm1 = vec1->norm(IVector::NORM::NORM_1); //
    double norm2 = vec3->norm(IVector::NORM::NORM_2); // SUCCESS
    double norm3 = vec5->norm(IVector::NORM::NORM_INF);//


    if ((norm1 - 9.0) <= 1e-5 && (norm2 - 4.0) <= 1e-5 && (norm3 - 100.0) <= 1e-5) {
        delete[] data1;
        delete[] data3;
        delete[] data5;
        delete vec1;
        delete vec3;
        delete vec5;
        return true;
    }
    else {
        delete[] data1;
        delete[] data3;
        delete[] data5;
        delete vec1;
        delete vec3;
        delete vec5;
        return false;
    }
}

bool CloneTest(ILogger* pLogger) {
    double* data1 = new double[3]{ 5,1,3 };

    IVector* vec1 = IVector::createVector(3, data1, pLogger);
    IVector* vec3 = vec1->clone();

    double tolerance = 0.01;

    bool test_success = true;

    RESULT_CODE rc = IVector::equals(vec1, vec3, IVector::NORM::NORM_1,tolerance, &test_success, pLogger);

    test_success = rc == RESULT_CODE::SUCCESS;

    if (test_success) {
        delete[] data1;
        delete vec1;
        delete vec3;
        return true;
    }
    else {
        delete[] data1;
        delete vec1;
        delete vec3;
        return false;
    }
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

    std::cout << "ADD TEST | ";
    if (AddTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "SUB TEST | ";
    if (SubTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "SCALAR_MUL TEST | ";
    if (ScalarTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "MUL TEST | ";
    if (MulTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "EQUALS TEST | ";
    if (EQTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "GETCOORD TEST | ";
    if (GetCordTest(pLogger)) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "SETCOORD TEST | ";
    if (SetCordTest(pLogger)) {
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

    std::cout << "Norm TEST | ";
    if (NormTest(pLogger)) {
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
