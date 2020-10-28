#ifndef CONSTMODIFIER_H
#define CONSTMODIFIER_H

#include "modifier.h"

namespace CS {

class ConstModifier : public Modifier
{
public:
    ConstModifier(double);
    ~ConstModifier();
    double getValue(double) override;
    double getPositiveValue(double) override;
    bool setAmplitudeModulation(Sound) override;
    Modifier * clone() override;
private:
    double _value;
};

}

#endif // CONSTMODIFIER_H
