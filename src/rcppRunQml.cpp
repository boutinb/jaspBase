
#include <Rcpp.h>
using namespace Rcpp;

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFileInfo>
#include <QQmlComponent>
#include <QQuickItem>

#include <filesystem>

#include <json/json.h>

static QGuiApplication* application = nullptr;
static QQmlApplicationEngine* engine = nullptr;


#define VALUE(string) #string
#define TO_LITERAL(string) VALUE(string) // Why???? It works...

void init()
{
	if (application) return;
	QString rHome = qgetenv("R_HOME");
	QString jaspBase = rHome + "/library/jaspBase";
	QString pluginsPath = jaspBase + "/PlugIns";
	QString jaspPluginPath = pluginsPath;
	QString qtPluginPath = pluginsPath;

#ifdef QT_HOME
	QString qtHome = TO_LITERAL(QT_HOME);
	QCoreApplication::addLibraryPath(qtHome + "/plugins"); // This is to find the platform in Qt
	qtPluginPath = qtHome + "/qml";
#else
	QCoreApplication::addLibraryPath(pluginsPath);
#endif

#ifdef JASP_PLUGINS_PATH
	jaspPluginPath = TO_LITERAL(JASP_PLUGINS_PATH);
#endif

	qputenv("QT_QPA_PLATFORM", "minimal");

	std::vector<const char*> arguments = {""};

	int		argc = arguments.size();
	char** argvs = new char*[argc];

	for (int i = 0; i < argc; i++)
	{
		argvs[i] = new char[strlen(arguments[i]) + 1];
		memset(argvs[i], '\0',				strlen(arguments[i]) + 1);
		memcpy(argvs[i], arguments[i],		strlen(arguments[i]));
		argvs[i][							strlen(arguments[i])] = '\0';
	}

	application = new QGuiApplication(argc, argvs);
	engine = new QQmlApplicationEngine();

	engine->addImportPath(qtPluginPath);
	if (jaspPluginPath != qtPluginPath)
		engine->addImportPath(jaspPluginPath);
}

// [[Rcpp::export]]

String loadQmlFileAndCheckOptions(String qmlFileName, String options, String data)
{
	init();
	engine->clearComponentCache();

	std::string qmlFileNameStr	= qmlFileName.get_cstring();
	std::string optionsStr		= options.get_cstring();
	std::string dataStr			= data.get_cstring();

	QFileInfo			qmlFile(QString::fromStdString(qmlFileNameStr));
	QString				qmlBaseName(qmlFile.baseName());
	QString				error;
	QQuickItem*			form = nullptr;

	if (!qmlFile.exists())
		error = "File not found: " + qmlFile.absoluteFilePath();

	if (error.isEmpty())
	{
		QUrl urlFile = QUrl::fromLocalFile(qmlFile.absoluteFilePath());
		QQmlComponent	qmlComp( engine, urlFile, QQmlComponent::PreferSynchronous);

		form = qobject_cast<QQuickItem*>(qmlComp.create());

		if (!form)
			error = QString::fromLatin1("QML form %1 not created").arg(QString::fromStdString(qmlFileNameStr));

		for (const auto & qmlError : qmlComp.errors())
			error += QString::fromLatin1("\nError when creating component at %1, %2: %3").arg(qmlError.line()).arg(qmlError.column()).arg(qmlError.description());
	}

	if (!error.isEmpty())
	{
		Json::Value result(Json::objectValue);
		result["error"] = error.toStdString();

		return result.toStyledString();
	}

	application->processEvents();

	QString returnedValue;
	QMetaObject::invokeMethod(form, "parseOptions",
		Q_RETURN_ARG(QString, returnedValue),
		Q_ARG(QString, QString::fromStdString(optionsStr)),
		Q_ARG(QString, QString::fromStdString(dataStr)));

	return returnedValue.toStdString();
}
