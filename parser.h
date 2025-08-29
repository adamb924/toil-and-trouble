#ifndef PARSER_H
#define PARSER_H

#include <abstracttextparser.h>

#include "toil-and-trouble_global.h"

namespace KE {
class AbstractTextSegment;
}

namespace TT {

class ParsingAdjudicator;

class TOILANDTROUBLE_EXPORT Parser : public KE::AbstractTextParser
{
public:
    Parser(const ME::Morphology *morphology, std::function<ME::Form(ME::Form)> normalizationFunction);
    Parser(const Parser & other) = delete;
    Parser operator=(const Parser & other) = delete;
    ~Parser() override;

    void parseText( const ME::Morphology *morphology, KE::AbstractTextAdapter * text, KE::WhichForm which, KE::AbstractParserLog * log = nullptr) const override;
    void parseTextSegment( KE::AbstractTextSegment * segment, KE::WhichForm which, KE::AbstractParserLog * log = nullptr) const;

    /// pass-through to mAdjudicator
    void readCriteria(QXmlStreamReader &in);

private:
    void parseTextItem(KE::AbstractTextItem * item, KE::WhichForm which, KE::AbstractParserLog * log ) const;

    const ME::Morphology * mMorphology;
    ParsingAdjudicator * mAdjudicator;
};

} // namespace TT

#endif // PARSER_H
