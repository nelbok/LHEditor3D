#include "TranslationsManager.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>

namespace fse {

struct TranslationsManager::Impl {
	QTranslator translator;
	QString current{ "en" };

	void installTranslator() {
		if (translator.load(QLocale(current), "app", "_", ":/translations")) {
			QCoreApplication::installTranslator(&translator);
		} else {
			assert(!"Translation not found");
		}
	}
};

TranslationsManager::TranslationsManager(QObject* parent)
	: QObject(parent)
	, _impl{ std::make_unique<Impl>() } {}

TranslationsManager::~TranslationsManager() {}

void TranslationsManager::init() {
	_impl->installTranslator();
}

const QString& TranslationsManager::current() const {
	return _impl->current;
}

void TranslationsManager::setCurrent(const QString& current) {
	if (_impl->current != current) {
		_impl->current = current;
		_impl->installTranslator();
		emit currentUpdated();
	}
}
} // namespace fse
