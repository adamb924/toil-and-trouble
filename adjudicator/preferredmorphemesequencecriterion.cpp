#include "preferredmorphemesequencecriterion.h"

#include "datatypes/parsing.h"

PreferredMorphemeSequenceCriterion::PreferredMorphemeSequenceCriterion(const QString & preferred, const QString & dispreferred)
{
    mPreferred = QRegularExpression( escapeBrackets(preferred) );
    mDispreferred = QRegularExpression( escapeBrackets(dispreferred) );
}

QList<Parsing> PreferredMorphemeSequenceCriterion::evaluate(const QList<Parsing> &parsings, bool *decided)
{
    QList<Parsing> stemsWithPreferred = parsingsWithSequence(parsings, mPreferred );

    /// optimization
    if( stemsWithPreferred.isEmpty() )
    {
        *decided = false;
        return parsings;
    }

    QList<Parsing> stemsWithDispreferred = parsingsWithSequence(parsings, mDispreferred );
    /// optimization
    if( stemsWithDispreferred.isEmpty() )
    {
        *decided = false;
        return parsings;
    }

    *decided = true;

    /// this handles situations where the preferred sequence is a subsequence of the dispreferred sequence
    /// e.g., [a] is preferred to [a][b]
    QList<Parsing> putativelyPreferred = stemsWithPreferred;
    foreach( Parsing p, stemsWithDispreferred )
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

QList<Parsing> PreferredMorphemeSequenceCriterion::parsingsWithSequence(const QList<Parsing> &parsings, const QRegularExpression &sequence)
{
    QList<Parsing> result;
    QListIterator<Parsing> i(parsings);
    while(i.hasNext())
    {
        Parsing p = i.next();
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
