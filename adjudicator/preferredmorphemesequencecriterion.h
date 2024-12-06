#ifndef PREFERREDMORPHEMESEQUENCECRITERION_H
#define PREFERREDMORPHEMESEQUENCECRITERION_H

#include "abstractadjudicationcriterion.h"

#include <QRegularExpression>

namespace TT {

class PreferredMorphemeSequenceCriterion : public AbstractAdjudicationCriterion
{
public:
    PreferredMorphemeSequenceCriterion(const QString & preferred, const QString & dispreferred);

    QList<ME::Parsing> evaluate(const QList<ME::Parsing> &parsings, bool * decided) override;

    QString summary() const override;

private:
    QList<ME::Parsing> parsingsWithSequence(const QList<ME::Parsing> &parsings, const QRegularExpression & sequence);
    QString escapeBrackets(QString str) const;

    QRegularExpression mPreferred, mDispreferred;
};

} // namespace TT

#endif // PREFERREDMORPHEMESEQUENCECRITERION_H
