#ifndef MULTIPARTDECODER_H
#define MULTIPARTDECODER_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QMap>

typedef QMap<QString, QString> Headers;

class MultiPartDecoder : public QObject
{
    Q_OBJECT
public:
    explicit MultiPartDecoder(QObject *parent = 0);

    ~MultiPartDecoder();

    void setBoundary(QString a_boundary);

    void decode(QByteArray a_data);

signals:
    void decodedChunk(Headers a_headers, QByteArray a_data);

public slots:

private:
    void processHeader(QByteArray& a_data, size_t& offset);
    void processData(QByteArray& a_data, size_t& offset);

    QString    m_boundary;
    size_t     m_boundaryOffset;
    QByteArray m_buffer;

    Headers    m_headers;
    QString    m_headerKey;
    QString    m_headerValue;

    enum State {
        STATE_BEGIN, // at first we use this state
        STATE_HEADER_KEY, // we are reading the header key
        STATE_HEADER_VALUE, // we are reading the header value
        STATE_DATA // finally we get into data, and eventually loop from STATE_HEADER
    } m_state;
};

#endif // MULTIPARTDECODER_H
