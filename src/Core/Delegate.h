#pragma once

#include <vector>
#include <functional>

using namespace std;

/*
	Custom delegate that accept multiple bindings.
	a generica class that store a vector of functions

	@author Fernando del Molino
*/
template<class... Args>
class MultipleDelegate
{

private:
    vector<function<void(Args...)>> _functions;

public:
    MultipleDelegate() {

    }

    ~MultipleDelegate() {
        _functions.clear();
    }

    void operator()(Args... args) {
        for (auto& func : _functions) {
            func(args...);
        }
    }

    /* Bind the function */
    void Bind(const function<void(Args...)>& f) {
        _functions.push_back(f);
    }

    /* Unbind a concret function */
    void Unbind(const function<void(Args...)>& f) {
        //_functions.erase(f);
    }

    /* Invoke all the functions */
    void Invoke(Args... args) {
        for (auto& func : _functions) {
            func(args...);
        }
    }

    /* Clear all the functions */
    void Clear() {
        _functions.clear();
    }
};

