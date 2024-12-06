#ifndef PARSINGADJUDICATOR_H
#define PARSINGADJUDICATOR_H

#include <QList>
#include "datatypes/parsing.h"

#include "toil-and-trouble_global.h"


namespace ME {
class Morphology;
}

namespace TT {

class AbstractAdjudicationCriterion;

class TOILANDTROUBLE_EXPORT ParsingAdjudicator
{
public:
    explicit ParsingAdjudicator(const ME::Morphology *morphology);

    /// Go through the adjudication critiera in order. If a criterion
    /// adjudicates between the parsings, that parsings is returned
    /// immediately. If none of the criteria adjudicate, the first
    /// parsing is returned.
    QList<ME::Parsing> adjudicate(const QList<ME::Parsing> & parsings) const;

    void readCriteria(QXmlStreamReader &in);

    static QString XML_PARSING_ADJUDICATOR;
    static QString XML_STEM_PREFERENCE;
    static QString XML_STEM_ID_PREFERENCE;
    static QString XML_MORPHEME_PREFERENCE;

private:
    const ME::Morphology *mMorphology;
    QList<AbstractAdjudicationCriterion*> mCriteria;
};

} // namespace TT

#endif // PARSINGADJUDICATOR_H
