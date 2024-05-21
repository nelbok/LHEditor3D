#include <lh/data/Model.hpp>

#include <lh/data/Project.hpp>
#include <lh/io/Json.hpp>

#include "common/Accessors.hpp"

namespace lh {
struct Model::Impl {
	QUrl sourcePath{};
	QString qmlName{};
};

Model::Model(Project* project)
	: Entity(project)
	, _impl{ std::make_unique<Impl>() } {
}

Model::~Model() {}

void Model::reset() {
	Entity::reset();
	setSourcePath({});
	setQmlName({});
}

void Model::copy(const Model& model) {
	Entity::copy(model);
	setSourcePath(model.sourcePath());
	setQmlName(model.qmlName());
}

const QUrl& Model::sourcePath() const {
	return _impl->sourcePath;
}

void Model::setSourcePath(const QUrl& sourcePath) {
	TOOLS_SETTER(Model, sourcePath);
}

const QString& Model::qmlName() const {
	return _impl->qmlName;
}

void Model::setQmlName(const QString& qmlName) {
	TOOLS_SETTER(Model, qmlName);
}

constexpr auto lSourcePath = "sourcePath";
constexpr auto lQmlName = "qmlName";

void Model::load(const QJsonObject& json) {
	Entity::load(json);
	setSourcePath(Json::toUrl(Json::toValue(lSourcePath, json)));
	setQmlName(Json::toString(lQmlName, json));
}

void Model::save(QJsonObject& json) const {
	Entity::save(json);
	json[lSourcePath] = Json::fromUrl(_impl->sourcePath);
	json[lQmlName] = _impl->qmlName;
}
} // namespace lh
