#include "languagedefinitions.h"
#include "context.h"
#include "value.h"
#include "executor/executorinterface.h"
#include <math.h>

#include <string>

using namespace CS;
using namespace std;

double CS::Functions::sin(double x) {
    return std::sin(x);
}

double CS::Functions::square(double x) {
    if( fmod(x,M_PI*2) < M_PI )
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
