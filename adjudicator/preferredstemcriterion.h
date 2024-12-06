#ifndef PREFERREDSTEMCRITERION_H
#define PREFERREDSTEMCRITERION_H

#include "abstractadjudicationcriterion.h"

namespace TT {

class PreferredStemCriterion : public AbstractAdjudicationCriterion
{
public:
    PreferredStemCriterion(const ME::LexicalStem & preferred, const ME::LexicalStem & dispreferred);

    QString summary() const override;

    QList<ME::Parsing> evaluate(const QList<ME::Parsing> &parsings, bool * decided) override;

private:
    static QList<ME::Parsing> parsingsWithStem(const QList<ME::Parsing> &parsings, const ME::LexicalStem & stem);
    static bool parsingsContainStem(const QList<ME::Parsing> &parsings, const ME::LexicalStem & stem);

    ME::LexicalStem mPreferred, mDispreferred;
};

} // namespace TT

#endif // PREFERREDSTEMCRITERION_H
