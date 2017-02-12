/**
 * @file   test.cpp
 * @author Sébastien Rouault <sebmsg@free.fr>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version. Please see https://gnu.org/licenses/gpl.html
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * @section DESCRIPTION
 *
 * Implementation tests.
**/

// ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▁ Declarations ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔

// External headers
#include <functional>
#include <iostream>

// Internal headers
#include <anyfunction.hpp>

// ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――

using namespace AnyFunction;

// ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔ Declarations ▔
// ▁ Tests ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔

/** Standalone function.
 * @param x
 * @return x + 2
**/
static float standalone(float x) {
    return x + 2;
}

/** Standalone function manipulation.
**/
static void test_standalone() {
    ::std::cout << "Standalone function:" << ::std::endl;
    { // Copy
        Function<float(float)> funcA = standalone;
        Function<float(float)> funcB = funcA;
        ::std::cout << "- [copy] standalone -> standalone: " << funcA(3) << ", " << funcB(3) << ::std::endl;
        if (funcA) {
            ::std::cout << "- source still valid" << ::std::endl;
        } else {
            ::std::cout << "- source not valid anymore" << ::std::endl;
        }
    }
    { // Move
        Function<float(float), 64> funcA = standalone;
        auto r = funcA(3);
        Function<float(float), 64> funcB = ::std::move(funcA);
        ::std::cout << "- [move] standalone -> standalone: " << r << ", " << funcB(3) << ::std::endl;
        if (funcA) {
            ::std::cout << "- source still valid" << ::std::endl;
        } else {
            ::std::cout << "- source not valid anymore" << ::std::endl;
        }
    }
}

// ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――

/** Lambda (= functor) manipulation.
**/
static void test_lambda() {
    float a = 1;
    float b = 2;
    auto lambda = [&](float x) -> float {
        return a * x + b;
    };
    ::std::cout << "Lambda functor:" << ::std::endl;
    { // Copy from local to local
        Function<float(float), 64> funcA = lambda;
        Function<float(float), 64> funcB = funcA;
        ::std::cout << "- [copy] local -> local: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Copy from local to remote
        Function<float(float), 64> funcA = lambda;
        Function<float(float), 0> funcB = funcA;
        ::std::cout << "- [copy] local -> remote: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Copy from remote to local
        Function<float(float), 0> funcA = lambda;
        Function<float(float), 64> funcB = funcA;
        ::std::cout << "- [copy] remote -> local: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Copy from remote to remote
        Function<float(float), 0> funcA = lambda;
        Function<float(float), 0> funcB = funcA;
        ::std::cout << "- [copy] remote -> remote: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Move from local to local
        Function<float(float), 64> funcA = lambda;
        auto r = funcA(3);
        Function<float(float), 64> funcB = ::std::move(funcA);
        ::std::cout << "- [move] local -> local: " << r << ", " << funcB(3) << ::std::endl;
    }
    { // Move from local to remote
        Function<float(float), 64> funcA = lambda;
        auto r = funcA(3);
        Function<float(float), 0> funcB = ::std::move(funcA);
        ::std::cout << "- [move] local -> remote: " << r << ", " << funcB(3) << ::std::endl;
    }
    { // Move from remote to local
        Function<float(float), 0> funcA = lambda;
        auto r = funcA(3);
        Function<float(float), 64> funcB = ::std::move(funcA);
        ::std::cout << "- [move] remote -> local: " << r << ", " << funcB(3) << ::std::endl;
    }
    { // Move from remote to remote
        Function<float(float), 0> funcA = lambda;
        auto r = funcA(3);
        Function<float(float), 0> funcB = ::std::move(funcA);
        ::std::cout << "- [move] remote -> remote: " << r << ", " << funcB(3) << ::std::endl;
    }
}

// ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――

/** Bind (= functor) manipulation.
**/
static void test_bind() {
    float a = 1;
    auto lambda = [&](float x, float b) -> float {
        return a * x + b;
    };
    auto bind = ::std::bind(lambda, ::std::placeholders::_1, 2);
    ::std::cout << "Bind functor:" << ::std::endl;
    { // Copy from local to local
        Function<float(float), 64> funcA = bind;
        Function<float(float), 64> funcB = funcA;
        ::std::cout << "- [copy] local -> local: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Copy from local to remote
        Function<float(float), 64> funcA = bind;
        Function<float(float), 0> funcB = funcA;
        ::std::cout << "- [copy] local -> remote: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Copy from remote to local
        Function<float(float), 0> funcA = bind;
        Function<float(float), 64> funcB = funcA;
        ::std::cout << "- [copy] remote -> local: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Copy from remote to remote
        Function<float(float), 0> funcA = bind;
        Function<float(float), 0> funcB = funcA;
        ::std::cout << "- [copy] remote -> remote: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Move from local to local
        Function<float(float), 64> funcA = bind;
        auto r = funcA(3);
        Function<float(float), 64> funcB = ::std::move(funcA);
        ::std::cout << "- [move] local -> local: " << r << ", " << funcB(3) << ::std::endl;
    }
    { // Move from local to remote
        Function<float(float), 64> funcA = bind;
        auto r = funcA(3);
        Function<float(float), 0> funcB = ::std::move(funcA);
        ::std::cout << "- [move] local -> remote: " << r << ", " << funcB(3) << ::std::endl;
    }
    { // Move from remote to local
        Function<float(float), 0> funcA = bind;
        auto r = funcA(3);
        Function<float(float), 64> funcB = ::std::move(funcA);
        ::std::cout << "- [move] remote -> local: " << r << ", " << funcB(3) << ::std::endl;
    }
    { // Move from remote to remote
        Function<float(float), 0> funcA = bind;
        auto r = funcA(3);
        Function<float(float), 0> funcB = ::std::move(funcA);
        ::std::cout << "- [move] remote -> remote: " << r << ", " << funcB(3) << ::std::endl;
    }
}

// ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――

/** Other functor manipulation.
**/
static void test_functor() {
    /** Test functor.
    **/
    class Test final {
    private:
        float a;
        float b;
    public:
        /** Value constructor.
        **/
        Test(float a, float b): a(a), b(b) {
            ::std::cout << "  - trace: value constructing " << this << ::std::endl;
        }
        /** Copy constructor/assignment.
         * @param test Instance to copy
         * @return Current instance
        **/
        Test(Test const& test): a(test.a), b(test.b) {
            ::std::cout << "  - trace: copy constructing " << this << " with " << ::std::addressof(test) << ::std::endl;
        }
        Test& operator=(Test const& test) {
            a = test.a;
            b = test.b;
            ::std::cout << "  - trace: copy assigning " << this << " with " << ::std::addressof(test) << ::std::endl;
            return *this;
        }
        /** Move constructor/assignment.
         * @param test Instance to move
         * @return Current instance
        **/
        Test(Test&& test): a(test.a), b(test.b) {
            ::std::cout << "  - trace: move constructing " << this << " with " << ::std::addressof(test) << ::std::endl;
        }
        Test& operator=(Test&& test) {
            a = test.a;
            b = test.b;
            ::std::cout << "  - trace: move assigning " << this << " with " << ::std::addressof(test) << ::std::endl;
            return *this;
        }
        /** Destructor.
        **/
        ~Test() {
            ::std::cout << "  - trace: destroying " << this << ::std::endl;
        }
    public:
        /** Computes y = a * x + b.
         * @param x
         * @return y
        **/
        float operator()(float x) const {
            return a * x + b;
        }
    };
    auto test = Test(1, 2);
    ::std::cout << "Other functor:" << ::std::endl;
    { // Copy from local to local
        Function<float(float), 64> funcA = test;
        Function<float(float), 64> funcB = funcA;
        ::std::cout << "- [copy] local -> local: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Copy from local to remote
        Function<float(float), 64> funcA = test;
        Function<float(float), 0> funcB = funcA;
        ::std::cout << "- [copy] local -> remote: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Copy from remote to local
        Function<float(float), 0> funcA = test;
        Function<float(float), 64> funcB = funcA;
        ::std::cout << "- [copy] remote -> local: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Copy from remote to remote
        Function<float(float), 0> funcA = test;
        Function<float(float), 0> funcB = funcA;
        ::std::cout << "- [copy] remote -> remote: " << funcA(3) << ", " << funcB(3) << ::std::endl;
    }
    { // Move from local to local
        Function<float(float), 64> funcA = test;
        auto r = funcA(3);
        Function<float(float), 64> funcB = ::std::move(funcA);
        ::std::cout << "- [move] local -> local: " << r << ", " << funcB(3) << ::std::endl;
    }
    { // Move from local to remote
        Function<float(float), 64> funcA = test;
        auto r = funcA(3);
        Function<float(float), 0> funcB = ::std::move(funcA);
        ::std::cout << "- [move] local -> remote: " << r << ", " << funcB(3) << ::std::endl;
    }
    { // Move from remote to local
        Function<float(float), 0> funcA = test;
        auto r = funcA(3);
        Function<float(float), 64> funcB = ::std::move(funcA);
        ::std::cout << "- [move] remote -> local: " << r << ", " << funcB(3) << ::std::endl;
    }
    { // Move from remote to remote
        Function<float(float), 0> funcA = test;
        auto r = funcA(3);
        Function<float(float), 0> funcB = ::std::move(funcA);
        ::std::cout << "- [move] remote -> remote: " << r << ", " << funcB(3) << ::std::endl;
    }
}

// ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔ Tests ▔
// ▁ Entry point ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔

/** Program entry point.
 * @param argc Number of arguments
 * @param argv Arguments
 * @return Return code
**/
int main(int argc, char** argv) {
    try {
        test_standalone();
        test_lambda();
        test_bind();
        test_functor();
    } catch (Exception::Any const& err) {
        ::std::cout << err.what() << ::std::endl;
    }
    return 0;
}

// ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔ Entry point ▔
// ▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔
