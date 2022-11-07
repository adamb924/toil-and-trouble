#include "preferredstemcriterion.h"

PreferredStemCriterion::PreferredStemCriterion(const LexicalStem &preferred, const LexicalStem &dispreferred) : mPreferred(preferred), mDispreferred(dispreferred)
{

}

QString PreferredStemCriterion::summary() const
{
    return QObject::tr("PreferredStemCriterion( Preferred: %1; Dispreferred: %2 )").arg( mPreferred.oneLineSummary() ).arg( mDispreferred.oneLineSummary() );
}

QList<Parsing> PreferredStemCriterion::evaluate(const QList<Parsing> &parsings, bool *decided)
{
    /// if the only stems in the list have the preferred stem,
    /// then return immediately: there's no decision to be made
    if( ! parsingsContainStem(parsings, mPreferred) )
    {
        *decided = false;
        return parsings;
    }

    /// if some of the stems do not have the preferred stem,
    /// then we need to check if any of them have the dispreferred
    /// stems. If *none* of them do, then this stem criterion
    /// doesn't apply, and we should return the original list.
    if( ! parsingsContainStem(parsings, mDispreferred) )
    {
        *decided = false;
        return parsings;
    }

    *decided = true;
    return parsingsWithStem(parsings, mPreferred);
}

QList<Parsing> PreferredStemCriterion::parsingsWithStem(const QList<Parsing> &parsings, const LexicalStem &stem)
{
    QList<Parsing> result;
    QListIterator<Parsing> i(parsings);
    while(i.hasNext())
    {
        Parsing p = i.next();
        if( p.containsStem( stem ) )
        {
            result << p;
        }
    }
    return result;
}

bool PreferredStemCriterion::parsingsContainStem(const QList<Parsing> &parsings, const LexicalStem &stem)
{
    QListIterator<Parsing> i(parsings);
    while(i.hasNext())
    {
        if( i.next().containsStem( stem ) )
        {
            return true;
        }
    }
    return false;
}
