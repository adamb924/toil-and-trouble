#include "preferredstemcriterion.h"

using namespace TT;

PreferredStemCriterion::PreferredStemCriterion(const ME::LexicalStem &preferred, const ME::LexicalStem &dispreferred) : mPreferred(preferred), mDispreferred(dispreferred)
{

}

QString PreferredStemCriterion::summary() const
{
    return QObject::tr("PreferredStemCriterion( Preferred: %1; Dispreferred: %2 )").arg( mPreferred.oneLineSummary() ).arg( mDispreferred.oneLineSummary() );
}

QList<ME::Parsing> PreferredStemCriterion::evaluate(const QList<ME::Parsing> &parsings, bool *decided)
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

QList<ME::Parsing> PreferredStemCriterion::parsingsWithStem(const QList<ME::Parsing> &parsings, const ME::LexicalStem &stem)
{
    QList<ME::Parsing> result;
    QListIterator<ME::Parsing> i(parsings);
    while(i.hasNext())
    {
        ME::Parsing p = i.next();
        if( p.containsStem( stem ) )
        {
            result << p;
        }
    }
    return result;
}

bool PreferredStemCriterion::parsingsContainStem(const QList<ME::Parsing> &parsings, const ME::LexicalStem &stem)
{
    QListIterator<ME::Parsing> i(parsings);
    while(i.hasNext())
    {
        if( i.next().containsStem( stem ) )
        {
            return true;
        }
    }
    return false;
}
