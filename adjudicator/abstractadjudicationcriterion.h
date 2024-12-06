#ifndef ABSTRACTADJUDICATIONCRITERION_H
#define ABSTRACTADJUDICATIONCRITERION_H

#include <QList>
#include "datatypes/parsing.h"

namespace TT {

class AbstractAdjudicationCriterion
{
public:
    AbstractAdjudicationCriterion();
    virtual ~AbstractAdjudicationCriterion();

    virtual QString summary() const = 0;

    virtual QList<ME::Parsing> evaluate(const QList<ME::Parsing> &parsings, bool * decided) = 0;
};

} // namespace TT

#endif // ABSTRACTADJUDICATIONCRITERION_H
