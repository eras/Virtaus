#include <QObject>
#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QQuickWindow>

#include "videostreamview.h"

VideoStreamView::VideoStreamView(QQuickItem* a_parent) :
    QQuickItem(a_parent), m_timer(new QTimer(this)), m_decoder(new MultiPartDecoder(this)), m_texture(0)
{
    m_timer->setSingleShot(false);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
//    m_timer->start(1000);
    setFlag(QQuickItem::ItemHasContents);

    connect(m_decoder, &MultiPartDecoder::decodedChunk, [this](QList<Header> headers, QByteArray data) {
        qDebug() << "Received chunk of" << data.size() << "bytes";
        m_image = data;
        update();
    });

    open("http://webcam.modeemi.fi/mjpg/video.mjpg");
}

void
VideoStreamView::open(QString url)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    m_decoder->setBoundary("myboundary");
    m_reply = manager->get(QNetworkRequest(QUrl(url)));

    qDebug() << "Started with" << m_reply;
    m_decoder->decode(m_reply->readAll());
    connect(m_reply, &QNetworkReply::readyRead, [=]() {
        QByteArray bytes = m_reply->readAll();
        //qDebug() << "Received some data:" << bytes.size();
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
