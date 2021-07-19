#include "entryshortener.hpp"

EntryShortener::EntryShortener(const QString& data)
    : m_wholeData(data)
{
    QStringList splitted = data.split("\n");
    m_shortenedData = splitted[0];
}

QString EntryShortener::getShortenedData() 
{
    return m_shortenedData;
}
