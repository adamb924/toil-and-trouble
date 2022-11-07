#ifndef PARSER_H
#define PARSER_H

#include <abstracttextparser.h>

#include "toil-and-trouble_global.h"

class ParsingAdjudicator;
class AbstractTextSegment;

class TOILANDTROUBLE_EXPORT Parser : public AbstractTextParser
{
public:
    Parser(const Morphology *morphology, std::function<Form(Form)> normalizationFunction);
    Parser(const Parser & other) = delete;
    Parser operator=(const Parser & other) = delete;
    ~Parser() override;

    void parseText( AbstractTextAdapter * text, WhichForm which, AbstractParserLog * log = nullptr) const override;
    void parseTextSegment( AbstractTextSegment * segment, WhichForm which, AbstractParserLog * log = nullptr) const;

    /// pass-through to mAdjudicator
    void readCriteria(QXmlStreamReader &in);

private:
    void parseTextItem(AbstractTextItem * item, WhichForm which, AbstractParserLog * log ) const;

    const Morphology * mMorphology;
    ParsingAdjudicator * mAdjudicator;
};

#endif // PARSER_H
