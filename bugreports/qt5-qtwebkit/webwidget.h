#ifndef _WEBWIDGET_H_
#define _WEBWIDGET_H_ 1

#include <QtWidgets/QtWidgets>
#include <QtWebKit/QtWebKit>
#include <QtCore/QDebug>

#include <QWebView>
#include <QWebFrame>
#include <QWebSettings>

class MyScriptSystem : public QObject {

	Q_OBJECT

public:

	MyScriptSystem(QObject* parent = 0);
	virtual ~MyScriptSystem();

	Q_INVOKABLE void setImage(const QImage& image);

	Q_INVOKABLE void setPixmap(const QPixmap& pixmap);
};


#endif