#ifndef VIDEOSTREAMVIEW_H
#define VIDEOSTREAMVIEW_H

#include <QObject>
#include <QTimer>
#include <QQuickItem>
#include <QNetworkReply>
#include <QByteArray>
#include <QSGTexture>

#include "multipartdecoder.h"

class VideoStreamView : public QQuickItem
{
    Q_OBJECT
public:
    explicit VideoStreamView(QQuickItem* a_parent = 0);

    Q_INVOKABLE void open(QString url);

    QSGNode* updatePaintNode(QSGNode *node, UpdatePaintNodeData *);

signals:

public slots:

private slots:

private:
    void processHeader(QString a_contentType);


    QTimer* m_timer;
    MultiPartDecoder* m_decoder;
    QNetworkReply* m_reply;
    QByteArray m_image;
    QSGTexture *m_texture;

    bool m_processedHeader;
};

#endif // VIDEOSTREAMVIEW_H
