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
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
public:
    explicit VideoStreamView(QQuickItem* a_parent = 0);

    Q_INVOKABLE void open(QString url);

    QSGNode* updatePaintNode(QSGNode *node, UpdatePaintNodeData *);

    QString url() const;
    void setUrl(QString url);

signals:
    void urlChanged();

public slots:

private slots:

private:
    void processHeader(QString a_contentType);

    QString m_url;

    QTimer* m_timer;
    MultiPartDecoder* m_decoder;
    QNetworkReply* m_reply;
    QByteArray m_image;
    QSGTexture *m_texture;

    bool m_processedHeader;
};

#endif // VIDEOSTREAMVIEW_H
