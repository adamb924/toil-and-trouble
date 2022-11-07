#ifndef PREFERREDMORPHEMESEQUENCECRITERION_H
#define PREFERREDMORPHEMESEQUENCECRITERION_H

#include "abstractadjudicationcriterion.h"

#include <QRegularExpression>

class PreferredMorphemeSequenceCriterion : public AbstractAdjudicationCriterion
{
public:
    PreferredMorphemeSequenceCriterion(const QString & preferred, const QString & dispreferred);

    QList<Parsing> evaluate(const QList<Parsing> &parsings, bool * decided) override;

    QString summary() const override;

private:
    QList<Parsing> parsingsWithSequence(const QList<Parsing> &parsings, const QRegularExpression & sequence);
    QString escapeBrackets(QString str) const;

    QRegularExpression mPreferred, mDispreferred;
};

#endif // PREFERREDMORPHEMESEQUENCECRITERION_H
