#ifndef PREFERREDSTEMIDCRITERION_H
#define PREFERREDSTEMIDCRITERION_H

#include "abstractadjudicationcriterion.h"

namespace TT {

class PreferredStemIdCriterion : public AbstractAdjudicationCriterion
{
public:
    PreferredStemIdCriterion(qlonglong preferred, qlonglong dispreferred);

    QString summary() const override;

    QList<ME::Parsing> evaluate(const QList<ME::Parsing> &parsings, bool * decided) override;

private:
    static QList<ME::Parsing> parsingsWithStem(const QList<ME::Parsing> &parsings, qlonglong stemId, int floor);
    static bool parsingsContainStem(const QList<ME::Parsing> &parsings, qlonglong stemId, int floor);
    static int minimumStemOccurences(const QList<ME::Parsing> &parsings, qlonglong stemId);

    qlonglong mPreferred, mDispreferred;
};

} // namespace TT

#endif // PREFERREDSTEMIDCRITERION_H
