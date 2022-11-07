#include "parsingadjudicator.h"

#include <stdexcept>

#include "abstractadjudicationcriterion.h"
#include "morphology.h"

#include "preferredstemcriterion.h"
#include "preferredmorphemesequencecriterion.h"
#include "preferredstemidcriterion.h"

#include "toilandtrouble.h"

#include <QXmlStreamReader>

ParsingAdjudicator::ParsingAdjudicator(const Morphology *morphology) : mMorphology(morphology)
{

}

QList<Parsing> ParsingAdjudicator::adjudicate(const QList<Parsing> &parsings) const
{
    if( parsings.count() < 2 )
    {
        return parsings;
    }

    QList<Parsing> adjudicatedParsings = parsings;
    int currentCount = adjudicatedParsings.count();

    if( ToilAndTrouble::DebugOutput )
    {
        qInfo() << "Starting with" << adjudicatedParsings.count() << ": " << Parsing::parsingListSummary( adjudicatedParsings );
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
                qInfo() << "Reduced to" << adjudicatedParsings.count() << ": " << Parsing::parsingListSummary( adjudicatedParsings ) << "by" << criterion->summary();
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
    Q_ASSERT( in.isStartElement() && in.name() == "parsing-adjudicator" );

    bool isOk = true;

    while(!in.atEnd()
          && !( in.isEndElement() && in.name() == "parsing-adjudicator" ))
    {
        in.readNext();

        if( in.tokenType() == QXmlStreamReader::StartElement )
        {
            if( in.name() == "stem-preference" )
            {
                WritingSystem ws = mMorphology->writingSystem( in.attributes().value("lang").toString() );
                QString preferred = in.attributes().value("prefer").toString();
                QString dispreferred = in.attributes().value("over").toString();

                try {
                    LexicalStem * preferredStem = mMorphology->uniqueLexicalStem( Form( ws, preferred ) );
                    LexicalStem * dispreferredStem = mMorphology->uniqueLexicalStem( Form( ws, dispreferred ) );

                    if( preferredStem != nullptr && dispreferredStem != nullptr )
                    {
                        mCriteria << new PreferredStemCriterion( *preferredStem, *dispreferredStem );
                    }
                } catch (const std::runtime_error& e) {
                    isOk = false;
                    qCritical() << e.what() << "(Line " << in.lineNumber() << ")";
                }
            }
            if( in.name() == "stem-id-preference" )
            {
                const qlonglong preferred = in.attributes().value("prefer").toLongLong();
                const qlonglong dispreferred = in.attributes().value("over").toLongLong();
                mCriteria << new PreferredStemIdCriterion( preferred, dispreferred );
            }
            else if( in.name() == "morpheme-preference" )
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

    Q_ASSERT( in.isEndElement() && in.name() == "parsing-adjudicator" );
}
