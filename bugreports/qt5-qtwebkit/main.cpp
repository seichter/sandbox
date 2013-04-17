/************************************************************************/
/*                                                                      */
/************************************************************************/
#include <QtWidgets/QtWidgets>
#include <QtWebKit/QtWebKit>
#include <QtCore/QDebug>

#include <QWebView>
#include <QWebFrame>
#include <QWebSettings>

#include "webwidget.h"



//////////////////////////////////////////////////////////////////////////

class MyWebView : public QWebView {

	Q_OBJECT

public:

	MyWebView()
		: QWebView()
	{
		// launching from a local resource
		QString launchPath, launchDir;

#if defined(__APPLE__)
		launchDir = "/../Resources/";
#elif defined(__linux)
		launchDir = "/../share/";
#elif defined(_WIN32)
		launchDir = "/../share/";
#endif

		// load from an internal path
		QDir dir;
		launchPath = dir.absoluteFilePath(QApplication::applicationDirPath() + launchDir);
		QUrl launchURL = QUrl::fromLocalFile(launchPath + "index.html");

		connect(this->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
			this, SLOT(impl_javaScriptWindowObjectCleared()));

		QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

		this->load(launchURL);
	}

	Q_SLOT void impl_javaScriptWindowObjectCleared() {
		this->page()->mainFrame()->addToJavaScriptWindowObject("engine",new MyScriptSystem(this));
	}


};

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	MyWebView* webview = new MyWebView();

	webview->setWindowTitle("Bounce");

	webview->show();

	return a.exec();
}

#include "main.moc"
