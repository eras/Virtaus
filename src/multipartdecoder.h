#ifndef MULTIPARTDECODER_H
#define MULTIPARTDECODER_H

#include <QObject>
#include <QString>
#include <QByteArray>

class Header: public QObject {
    Q_OBJECT
public:
    Header() {
        // nothing
    }

    Header(const Header& a_other) :
        QObject(0),
        m_key(a_other.m_key),
        m_value(a_other.m_value) {
        // nothing
    }

    Header(QString a_key, QString a_value) : m_key(a_key), m_value(a_value) {
        // nothing
    }

    ~Header() {
        // nothing
    }

    QString getKey() const { return m_key; }
    QString getValue() const { return m_value; }

private:
    QString m_key;
    QString m_value;
};

class MultiPartDecoder : public QObject
{
    Q_OBJECT
public:
    explicit MultiPartDecoder(QObject *parent = 0);

    ~MultiPartDecoder();

    void setBoundary(QString a_boundary);

    void decode(QByteArray a_data);

signals:
    void decodedChunk(QList<Header> header, QByteArray a_data);

public slots:

private:
    void processHeader(QByteArray& a_data, size_t& offset);
    void processData(QByteArray& a_data, size_t& offset);

    QString    m_boundary;
    size_t     m_boundaryOffset;
    QByteArray m_buffer;

    QList<Header> m_header;
    QString    m_headerKey;
    QString    m_headerValue;

    int        m_debugCounter;

    enum State {
        STATE_BEGIN, // at first we use this state
        STATE_HEADER_KEY, // we are reading the header key
        STATE_HEADER_VALUE, // we are reading the header value
        STATE_DATA // finally we get into data, and eventually loop from STATE_HEADER
    } m_state;
};

#endif // MULTIPARTDECODER_H
