#ifndef PREFERREDSTEMIDCRITERION_H
#define PREFERREDSTEMIDCRITERION_H

#include "abstractadjudicationcriterion.h"

class PreferredStemIdCriterion : public AbstractAdjudicationCriterion
{
public:
    PreferredStemIdCriterion(qlonglong preferred, qlonglong dispreferred);

    QString summary() const override;

    QList<Parsing> evaluate(const QList<Parsing> &parsings, bool * decided) override;

private:
    static QList<Parsing> parsingsWithStem(const QList<Parsing> &parsings, qlonglong stemId, int floor);
    static bool parsingsContainStem(const QList<Parsing> &parsings, qlonglong stemId, int floor);
    static int minimumStemOccurences(const QList<Parsing> &parsings, qlonglong stemId);

    qlonglong mPreferred, mDispreferred;
};

#endif // PREFERREDSTEMIDCRITERION_H
