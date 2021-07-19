#ifndef ENTRYSHORTENER_HPP
#define ENTRYSHORTENER_HPP

#include <QStringList>
#include <QDebug>

class EntryShortener {
public:
    explicit EntryShortener(const QString& data);

    QString getShortenedData();

private:
    QString m_shortenedData;
    QString m_wholeData;
};

#endif // ENTRYSHORTENER_HPP
