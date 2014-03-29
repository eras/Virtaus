#include <QDebug>
#include "multipartdecoder.h"

MultiPartDecoder::MultiPartDecoder(QObject *parent) :
    QObject(parent), m_boundaryOffset(0), m_state(STATE_BEGIN)
{

}

MultiPartDecoder::~MultiPartDecoder()
{

}

void
MultiPartDecoder::setBoundary(QString a_boundary)
{
    bool addDashes = (a_boundary.size() < 2 || a_boundary[0] != '-' || a_boundary[1] != '-');
    m_boundary = QString("\r\n") + (addDashes ? "--" : "") + a_boundary + "\r\n";
    m_boundaryOffset = 2; // skip the first two characters for the first boundary
    m_state = STATE_BEGIN;
}

void
MultiPartDecoder::processHeader(QByteArray& a_data, size_t& offset)
{
    State origState = m_state;
    while (offset < size_t(a_data.size()) && m_state == origState) {
        char ch = a_data[offset];
        switch (m_state) {
        case STATE_HEADER_KEY: {
            if (ch == ':') {
                m_state = STATE_HEADER_VALUE;
            } else if (ch == '\r') {
                // ignore first or \r\n
            } else if (ch == '\n') {
                m_state = STATE_DATA;
            } else {
                m_headerKey += ch;
            }
        } break;
        case STATE_HEADER_VALUE: {
            if (ch == '\r') {
                // ignore
            } else if (ch == ' ' && m_headerValue.size() == 0) {
                // ignore first space
            } else if (ch == '\n') {
                m_headers.insert(m_headerKey, m_headerValue);
                m_headerKey = "";
                m_headerValue = "";
                m_state = STATE_HEADER_KEY;
            } else {
                m_headerValue += ch;
            }
        } break;
        default: ; // ignore other cases
        }
        ++offset;
    }
}

void
MultiPartDecoder::processData(QByteArray& a_data, size_t& offset)
{
    State origState = m_state;
    while (offset < size_t(a_data.size()) && m_state == origState) {
        char ch = a_data[offset];
        if (ch == m_boundary[m_boundaryOffset]) {
            ++m_boundaryOffset;
        } else {
            m_boundaryOffset = 0;
        }
        if (m_boundaryOffset == size_t(m_boundary.size())) {
            if (m_state != STATE_BEGIN) {
                m_buffer.resize(m_buffer.size() - m_boundary.size());
                emit decodedChunk(m_headers, m_buffer);
            }
            m_headers.clear();
            m_buffer.clear();
            m_boundaryOffset = 0;
            m_state = STATE_HEADER_KEY;
        } else {
            m_buffer.append(ch);
        }
        ++offset;
    }
}

void
MultiPartDecoder::decode(QByteArray a_data)
{
    size_t offset = 0;
    while (offset < size_t(a_data.size())) {
        switch (m_state) {
        case STATE_BEGIN:
        case STATE_DATA: {
            processData(a_data, offset);
        } break;
        case STATE_HEADER_KEY:
        case STATE_HEADER_VALUE: {
            processHeader(a_data, offset);
        } break;
        }
    }
}
