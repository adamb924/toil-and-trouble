#ifndef PREFERREDSTEMCRITERION_H
#define PREFERREDSTEMCRITERION_H

#include "abstractadjudicationcriterion.h"

class PreferredStemCriterion : public AbstractAdjudicationCriterion
{
public:
    PreferredStemCriterion(const LexicalStem & preferred, const LexicalStem & dispreferred);

    QString summary() const override;

    QList<Parsing> evaluate(const QList<Parsing> &parsings, bool * decided) override;

private:
    static QList<Parsing> parsingsWithStem(const QList<Parsing> &parsings, const LexicalStem & stem);
    static bool parsingsContainStem(const QList<Parsing> &parsings, const LexicalStem & stem);

    LexicalStem mPreferred, mDispreferred;
};

#endif // PREFERREDSTEMCRITERION_H
