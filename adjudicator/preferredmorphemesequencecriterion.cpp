#include "preferredmorphemesequencecriterion.h"

#include "datatypes/parsing.h"

using namespace TT;

PreferredMorphemeSequenceCriterion::PreferredMorphemeSequenceCriterion(const QString & preferred, const QString & dispreferred)
{
    mPreferred = QRegularExpression( escapeBrackets(preferred) );
    mDispreferred = QRegularExpression( escapeBrackets(dispreferred) );
}

QList<ME::Parsing> PreferredMorphemeSequenceCriterion::evaluate(const QList<ME::Parsing> &parsings, bool *decided)
{
    QList<ME::Parsing> stemsWithPreferred = parsingsWithSequence(parsings, mPreferred );

    /// optimization
    if( stemsWithPreferred.isEmpty() )
    {
        *decided = false;
        return parsings;
    }

    QList<ME::Parsing> stemsWithDispreferred = parsingsWithSequence(parsings, mDispreferred );
    /// optimization
    if( stemsWithDispreferred.isEmpty() )
    {
        *decided = false;
        return parsings;
    }

    *decided = true;

    /// this handles situations where the preferred sequence is a subsequence of the dispreferred sequence
    /// e.g., [a] is preferred to [a][b]
    QList<ME::Parsing> putativelyPreferred = stemsWithPreferred;
    foreach( ME::Parsing p, stemsWithDispreferred )
    {
        putativelyPreferred.removeOne( p );
    }

    if( putativelyPreferred.count() > 0 )
    {
        *decided = true;
        return putativelyPreferred;
    }
    else
    {
        *decided = false;
        return stemsWithPreferred;
    }
}

QString PreferredMorphemeSequenceCriterion::summary() const
{
    return QObject::tr("PreferredMorphemeSequenceCriterion( Preferred: %1; Dispreferred: %2 )").arg( mPreferred.pattern() ).arg( mDispreferred.pattern() );
}

QList<ME::Parsing> PreferredMorphemeSequenceCriterion::parsingsWithSequence(const QList<ME::Parsing> &parsings, const QRegularExpression &sequence)
{
    QList<ME::Parsing> result;
    QListIterator<ME::Parsing> i(parsings);
    while(i.hasNext())
    {
        ME::Parsing p = i.next();
        if( p.labelSummary().contains( sequence ) )
        {
            result << p;
        }
    }
    return result;
}

QString PreferredMorphemeSequenceCriterion::escapeBrackets(QString str) const
{
    return str.replace("[","\\[").replace("]","\\]");
}
