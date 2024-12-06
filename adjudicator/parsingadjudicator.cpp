#include "parsingadjudicator.h"

#include <stdexcept>

#include "abstractadjudicationcriterion.h"
#include "morphology.h"

#include "preferredstemcriterion.h"
#include "preferredmorphemesequencecriterion.h"
#include "preferredstemidcriterion.h"

#include "toilandtrouble.h"

#include <QXmlStreamReader>

using namespace TT;

QString ParsingAdjudicator::XML_PARSING_ADJUDICATOR = "parsing-adjudicator";
QString ParsingAdjudicator::XML_STEM_PREFERENCE = "stem-preference";
QString ParsingAdjudicator::XML_STEM_ID_PREFERENCE = "stem-id-preference";
QString ParsingAdjudicator::XML_MORPHEME_PREFERENCE = "morpheme-preference";

ParsingAdjudicator::ParsingAdjudicator(const ME::Morphology *morphology) : mMorphology(morphology)
{

}

QList<ME::Parsing> ParsingAdjudicator::adjudicate(const QList<ME::Parsing> &parsings) const
{
    if( parsings.count() < 2 )
    {
        return parsings;
    }

    QList<ME::Parsing> adjudicatedParsings = parsings;
    int currentCount = adjudicatedParsings.count();

    if( ToilAndTrouble::DebugOutput )
    {
        qInfo() << "Starting with" << adjudicatedParsings.count() << ": " << ME::Parsing::parsingListSummary( adjudicatedParsings );
    }

    QListIterator<AbstractAdjudicationCriterion*> i(mCriteria);
    while(i.hasNext())
    {
        AbstractAdjudicationCriterion* criterion = i.next();
        bool decided = false;
        adjudicatedParsings = criterion->evaluate( adjudicatedParsings, &decided );
        if( ToilAndTrouble::DebugOutput )
        {
            if( adjudicatedParsings.count() < currentCount )
            {
                qInfo() << "Reduced to" << adjudicatedParsings.count() << ": " << ME::Parsing::parsingListSummary( adjudicatedParsings ) << "by" << criterion->summary();
                currentCount = adjudicatedParsings.count();
            }
            else
            {
                qInfo() << "No effect:" << criterion->summary();
            }
        }
        if( adjudicatedParsings.length() == 1 )
        {
            return adjudicatedParsings;
        }
    }
    Q_ASSERT(adjudicatedParsings.count() > 0);
    return adjudicatedParsings;
}

void ParsingAdjudicator::readCriteria(QXmlStreamReader &in)
{
    Q_ASSERT( in.isStartElement() && in.name() == XML_PARSING_ADJUDICATOR );

    bool isOk = true;

    while(!in.atEnd()
          && !( in.isEndElement() && in.name() == XML_PARSING_ADJUDICATOR ))
    {
        in.readNext();

        if( in.tokenType() == QXmlStreamReader::StartElement )
        {
            if( in.name() == XML_STEM_PREFERENCE )
            {
                ME::WritingSystem ws = mMorphology->writingSystem( in.attributes().value("lang").toString() );
                QString preferred = in.attributes().value("prefer").toString();
                QString dispreferred = in.attributes().value("over").toString();

                try {
                    ME::LexicalStem * preferredStem = mMorphology->uniqueLexicalStem( ME::Form( ws, preferred ) );
                    ME::LexicalStem * dispreferredStem = mMorphology->uniqueLexicalStem( ME::Form( ws, dispreferred ) );

                    if( preferredStem != nullptr && dispreferredStem != nullptr )
                    {
                        mCriteria << new PreferredStemCriterion( *preferredStem, *dispreferredStem );
                    }
                } catch (const std::runtime_error& e) {
                    isOk = false;
                    qCritical() << e.what() << "(Line " << in.lineNumber() << ")";
                }
            }
            if( in.name() == XML_STEM_ID_PREFERENCE )
            {
                const qlonglong preferred = in.attributes().value("prefer").toLongLong();
                const qlonglong dispreferred = in.attributes().value("over").toLongLong();
                mCriteria << new PreferredStemIdCriterion( preferred, dispreferred );
            }
            else if( in.name() == XML_MORPHEME_PREFERENCE )
            {
                QString preferred = in.attributes().value("prefer").toString();
                QString dispreferred = in.attributes().value("over").toString();
                mCriteria << new PreferredMorphemeSequenceCriterion( preferred, dispreferred );
            }
        }
    }

    if( !isOk )
    {
        throw std::runtime_error( "Sound and Fury had read errors (described above)." );
    }

    Q_ASSERT( in.isEndElement() && in.name() == XML_PARSING_ADJUDICATOR );
}
