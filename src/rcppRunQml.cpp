
#include <Rcpp.h>
using namespace Rcpp;

#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFileInfo>
#include <QQmlComponent>
#include <QQuickItem>

#include <filesystem>

#ifdef BUILDING_JASP
#include <json/json.h>
#else
#include "json/json.h"
#endif


static bool initialized = false;
static QGuiApplication* application = nullptr;
static QQmlApplicationEngine* engine = nullptr;
static const std::string SOURCE_FOLDER= "/Users/brunoboutin/JASP/source/qmlR/";

void init()
{
	if (initialized) return;
	initialized = true;

	QString rHome = qgetenv("R_HOME");
	QString qmlRFolder = rHome + "/library/qmlR";

	QCoreApplication::addLibraryPath(qmlRFolder + "/plugins");

	int					dummyArgc = 1;
	char				dummyArgv[2];
	dummyArgv[0] = '?';
	dummyArgv[1] = '\0';

	const char* qmlR = SOURCE_FOLDER.c_str();
	const char*	platformArg = "-platform";
	const char*	platformOpt = "cocoa";

	std::vector<const char*> arguments = {qmlR, platformArg, platformOpt};

	int		argc = arguments.size();
	char** argvs = new char*[argc];

	for (int i = 0; i < argc; i++)
	{
		argvs[i] = new char[strlen(arguments[i]) + 1];
		memset(argvs[i], '\0',				strlen(arguments[i]) + 1);
		memcpy(argvs[i], arguments[i],		strlen(arguments[i]));
		argvs[i][							strlen(arguments[i])] = '\0';
	}


	qputenv("QT_QPA_PLATFORM", "cocoa");
	//qputenv("QT_QPA_PLATFORM_PLUGIN_PATH", platformFolder.toStdString().c_str());
	char			*	dummyArgvP = dummyArgv;
	application = new QGuiApplication(argc, argvs);
	engine = new QQmlApplicationEngine();

	engine->addImportPath("/Users/brunoboutin/Qt/6.4.2/macos/qml");
	engine->addImportPath("/Users/brunoboutin/JASP/source/build-jaspQMLComponents-Qt_6_4_2_for_macOS-Debug/components");
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

	if (error.isEmpty())
	{
		application->processEvents();

		QString returnedValue;
		QMetaObject::invokeMethod(form, "parseOptions",
			Q_RETURN_ARG(QString, returnedValue),
			Q_ARG(QString, QString::fromStdString(optionsStr)),
			Q_ARG(QString, QString::fromStdString(dataStr)));
		return returnedValue.toStdString();
	}

	Json::Value result(Json::objectValue);
	result["error"] = error.toStdString();

	return result.toStyledString();
}
