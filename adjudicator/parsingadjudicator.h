#ifndef PARSINGADJUDICATOR_H
#define PARSINGADJUDICATOR_H

#include <QList>
#include "datatypes/parsing.h"

#include "toil-and-trouble_global.h"

class AbstractAdjudicationCriterion;
class MortalEngineTranslationSource;
class Morphology;

class TOILANDTROUBLE_EXPORT ParsingAdjudicator
{
public:
    explicit ParsingAdjudicator(const Morphology *morphology);

    /// Go through the adjudication critiera in order. If a criterion
    /// adjudicates between the parsings, that parsings is returned
    /// immediately. If none of the criteria adjudicate, the first
    /// parsing is returned.
    QList<Parsing> adjudicate(const QList<Parsing> & parsings) const;

    void readCriteria(QXmlStreamReader &in);

private:
    const Morphology *mMorphology;
    QList<AbstractAdjudicationCriterion*> mCriteria;
};

#endif // PARSINGADJUDICATOR_H
