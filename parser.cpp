#include "parser.h"

#include "abstracttextadapter.h"
#include "abstractparserlog.h"
#include "abstractparsedform.h"
#include "morphology.h"
#include "abstracttextsegment.h"

#include "adjudicator/parsingadjudicator.h"

using namespace TT;

Parser::Parser(const ME::Morphology * morphology, std::function<ME::Form (ME::Form)> normalizationFunction) : KE::AbstractTextParser(normalizationFunction), mMorphology(morphology), mAdjudicator( new ParsingAdjudicator(mMorphology) )
{

}

Parser::~Parser()
{
    delete mAdjudicator;
}

void Parser::parseText(const ME::Morphology *morphology, KE::AbstractTextAdapter *text, KE::WhichForm which, KE::AbstractParserLog *log) const
{
    Q_UNUSED(morphology)
    text->foreachTextItem( [this,which,log](KE::AbstractTextItem * item) {
        parseTextItem(item, which, log);
    } );
}

void Parser::parseTextSegment(KE::AbstractTextSegment *segment, KE::WhichForm which, KE::AbstractParserLog *log) const
{
    segment->foreachTextItem( [this,which,log](KE::AbstractTextItem * item) {
        parseTextItem(item, which, log);
    } );
}

void Parser::readCriteria(QXmlStreamReader &in)
{
    mAdjudicator->readCriteria(in);
}

void Parser::parseTextItem(KE::AbstractTextItem *item, KE::WhichForm which, KE::AbstractParserLog *log) const
{
    /// 1. Get a list of parsings for the input
    QSet<ME::Parsing> parsings;
    ME::Form formToParse = (*(item).*which)().form();

    /// TODO perhaps this should be customizeable
    if( formToParse.isWhitespaceAndNonWordCharacters() )
    {
        (*(item).*which)().setWellformedness( KE::AbstractTextItem::Ignored );
        return;
    }
    parsings = mMorphology->uniqueParsings( mNormalizationFunction( formToParse ) );

    /// 2. Convert to list (necessary for various functions)
    QList<ME::Parsing> parsingList(parsings.begin(),parsings.end());

    /// 3. (Maybe) Warn if there are no available parsings; though this could be irritating
    if( parsingList.count() == 0 )
    {
        (*(item).*which)().setWellformedness( KE::AbstractTextItem::NotWellformed );
//        qWarning() << "Unable to parse:" << item->input().summary();
    }
    /// 4. If there are multiple parsings available, we try to choose which one is the best
    else if( parsingList.count() > 1 )
    {
        /// 4b. Get the best parsings from the Adjudicator
        QList<ME::Parsing> adjudicated = mAdjudicator->adjudicate( parsingList );
        (*(item).*which)().setParsing( adjudicated.first() );
        (*(item).*which)().setWellformedness( KE::AbstractTextItem::WellformedMultiple );

        /// 4c. If adjudication didn't produce a result, warn the user
        if( log != nullptr && adjudicated.count() > 1 )
        {
            log->addAmbiguousParsing( adjudicated );
        }

    }
    /// 5. Otherwise there is only one parsing; that's the easy case
    else
    {
        (*(item).*which)().setParsing( parsingList.first() ); /// this will set well-formedness as well
        (*(item).*which)().setWellformedness( KE::AbstractTextItem::WellformedSingle );
    }
}
