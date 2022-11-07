#ifndef ABSTRACTADJUDICATIONCRITERION_H
#define ABSTRACTADJUDICATIONCRITERION_H

#include <QList>
#include "datatypes/parsing.h"

class AbstractAdjudicationCriterion
{
public:
    AbstractAdjudicationCriterion();
    virtual ~AbstractAdjudicationCriterion();

    virtual QString summary() const = 0;

    virtual QList<Parsing> evaluate(const QList<Parsing> &parsings, bool * decided) = 0;
};

#endif // ABSTRACTADJUDICATIONCRITERION_H
