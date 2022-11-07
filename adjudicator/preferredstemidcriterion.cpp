#include "preferredstemidcriterion.h"

PreferredStemIdCriterion::PreferredStemIdCriterion(qlonglong preferred, qlonglong dispreferred)
    : mPreferred(preferred),
      mDispreferred(dispreferred)
{

}

QString PreferredStemIdCriterion::summary() const
{
    return QObject::tr("PreferredStemIdCriterion( Preferred: %1; Dispreferred: %2 )").arg( mPreferred ).arg( mDispreferred );

}

QList<Parsing> PreferredStemIdCriterion::evaluate(const QList<Parsing> &parsings, bool *decided)
{
    const int preferredFloor = minimumStemOccurences(parsings, mPreferred);
    const int dispreferredFloor = minimumStemOccurences(parsings, mDispreferred);

    /// if the only stems in the list have the preferred stem,
    /// then return immediately: there's no decision to be made
    if( ! parsingsContainStem(parsings, mPreferred, preferredFloor) )
    {
        *decided = false;
        return parsings;
    }

    /// if some of the stems do not have the preferred stem,
    /// then we need to check if any of them have the dispreferred
    /// stems. If *none* of them do, then this stem criterion
    /// doesn't apply, and we should return the original list.
    if( ! parsingsContainStem(parsings, mDispreferred, dispreferredFloor) )
    {
        *decided = false;
        return parsings;
    }

    *decided = true;
    return parsingsWithStem(parsings, mPreferred, preferredFloor);
}

QList<Parsing> PreferredStemIdCriterion::parsingsWithStem(const QList<Parsing> &parsings, qlonglong stemId, int floor)
{
    QList<Parsing> result;
    QListIterator<Parsing> i(parsings);
    while(i.hasNext())
    {
        const Parsing p = i.next();
        if( p.numberOfInstancesOfStem(stemId) > floor )
        {
            result << p;
        }
    }
    return result;
}

bool PreferredStemIdCriterion::parsingsContainStem(const QList<Parsing> &parsings, qlonglong stemId, int floor)
{
    QListIterator<Parsing> i(parsings);
    while(i.hasNext())
    {
        if( i.next().numberOfInstancesOfStem(stemId) > floor )
        {
            return true;
        }
    }
    return false;
}

int PreferredStemIdCriterion::minimumStemOccurences(const QList<Parsing> &parsings, qlonglong stemId)
{
    QListIterator<Parsing> i(parsings);
    int fewestOccurences = INT_MAX;
    while(i.hasNext())
    {
        const int numberOfOccurences = i.next().numberOfInstancesOfStem( stemId );
        if( numberOfOccurences < fewestOccurences )
        {
            fewestOccurences = numberOfOccurences;
        }
        /// this will generally trigger, so it's a worthwhile optimizations
        if( fewestOccurences == 0 )
        {
            return 0;
        }
    }
    return fewestOccurences;
}
