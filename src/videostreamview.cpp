#include <QObject>
#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QQuickWindow>
#include <QRegExp>

#include "videostreamview.h"

VideoStreamView::VideoStreamView(QQuickItem* a_parent) :
    QQuickItem(a_parent), m_timer(new QTimer(this)), m_decoder(new MultiPartDecoder(this)), m_reply(0), m_texture(0), m_processedHeader(false)
{
    m_timer->setSingleShot(false);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
//    m_timer->start(1000);
    setFlag(QQuickItem::ItemHasContents);

    connect(m_decoder, &MultiPartDecoder::decodedChunk, [this](Headers headers, QByteArray data) {
        qDebug() << "Received chunk of" << data.size() << "bytes";
        m_image = data;
        update();
    });

}

QString
VideoStreamView::url() const
{
    return m_url;
}

void
VideoStreamView::setUrl(QString a_url)
{
    if (m_reply) {
        delete m_reply; m_reply = 0;
    }
    m_url = a_url;
    open(m_url);
}

void
VideoStreamView::processHeader(QString a_contentType)
{
    QRegExp regexp("^([^;]*)(?:; (.*))$");
    if (regexp.exactMatch(a_contentType)) {
        QString contentType = regexp.cap(1);
        QString parameter = regexp.cap(2);
        if (parameter.size()) {
            QRegExp paramRegexp("^(.*)=(.*)$");
            if (paramRegexp.exactMatch(parameter)) {
                if (paramRegexp.cap(1) == "boundary") {
                    m_decoder->setBoundary(paramRegexp.cap(2));
                }
            }
        }
    }
}

void
VideoStreamView::open(QString url)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    // connect(manager, SIGNAL(finished(QNetworkReply*)),
    //         this, SLOT(replyFinished(QNetworkReply*)));

    m_reply = manager->get(QNetworkRequest(QUrl(url)));

    connect(m_reply, &QNetworkReply::readyRead, [this]() {
        if (!m_processedHeader) {
            processHeader(m_reply->header(QNetworkRequest::ContentTypeHeader).toString());
            m_processedHeader = true;
        }
        QByteArray bytes = m_reply->readAll();
        m_decoder->decode(bytes);
    });
}

QSGNode*
VideoStreamView::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    delete node;
    delete m_texture;
    if (m_image.size()) {
        QImage image;
        image.loadFromData(m_image);
        m_texture = window()->createTextureFromImage(image);
        QSGSimpleTextureNode* texNode = new QSGSimpleTextureNode();
        texNode->setTexture(m_texture);
        texNode->setRect(boundingRect());
        return texNode;
    } else {
        QSGSimpleRectNode *n = static_cast<QSGSimpleRectNode *>(node);
        if (!n) {
            n = new QSGSimpleRectNode();
        }
        n->setColor(QColor(rand() % 256, rand() % 256, rand() % 256));
        n->setRect(boundingRect());
        return n;
    }
}
