#include <fsd/data/Project.hpp>

#include <stdexcept>

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>

#include <fsd/data/Character.hpp>
#include <fsd/data/Link.hpp>
#include <fsd/data/Model.hpp>
#include <fsd/data/Place.hpp>
#include <fsd/data/UuidPointer.hpp>
#include <fsd/io/Json.hpp>

#include "common/Accessors.hpp"

namespace fsd {

struct Project::Impl {
	UuidPointer<Place>* defaultPlace{ nullptr };
	QUrl path{};
	bool isTemp{ true };
	QList<Character*> characters{};
	QList<Link*> links{};
	QList<Model*> models{};
	QList<Place*> places{};

	bool isInterruptionRequested(const Project* project) const {
		auto* thread = project->thread();
		if (thread != QCoreApplication::instance()->thread()) {
			return thread->isInterruptionRequested();
		}
		return false;
	}

	template<class TClass>
	void loadList(Project* project, QList<TClass*>& list, const QString& key, const QJsonObject& json, void (Project::*signal)()) {
		list.clear();
		const auto& jsonArray = Json::toArray(key, json);
		for (const auto& jsonEntity : jsonArray) {
			if (!jsonEntity.isObject()) {
				throw std::runtime_error("JSON isn't an object");
			}
			auto* entity = new TClass(project);
			entity->load(jsonEntity.toObject());
			list.append(entity);
			if (isInterruptionRequested(project)) {
				return;
			}
		}
		emit(project->*signal)();
	}

	template<class TClass>
	void saveList(const Project* project, const QList<TClass*>& list, const QString& key, QJsonObject& json) const {
		QJsonArray jsonArray;
		for (auto* entity : list) {
			if (!entity->isAlive())
				continue;

			QJsonObject jsonEntity;
			entity->save(jsonEntity);
			jsonArray.append(jsonEntity);
			if (isInterruptionRequested(project)) {
				return;
			}
		}
		json[key] = jsonArray;
	}
};

Project::Project(QObject* parent)
	: Entity(parent)
	, _impl{ std::make_unique<Impl>() } {
	_impl->defaultPlace = makePlacePointer(this, this);
}

Project::~Project() {}

void Project::reset() {
	Entity::reset();

	// Reset datas
	setDefaultPlace(nullptr);
	cleanCharacters();
	cleanLinks();
	cleanModels();
	cleanPlaces();
}

Place* Project::defaultPlace() const {
	return (_impl->defaultPlace->valid()) ? _impl->defaultPlace->get() : nullptr;
}

void Project::setDefaultPlace(Place* defaultPlace) {
	if (_impl->defaultPlace->set(defaultPlace)) {
		emit defaultPlaceUpdated();
	}
}

const QList<Character*>& Project::characters() const {
	return _impl->characters;
}

void Project::setCharacters(const QList<Character*>& characters) {
	TOOLS_SETTER(Project, characters);
}

Character* Project::createCharacter() {
	TOOLS_CREATE_ENTITY(Project, characters);
}

void Project::removeCharacter(Character* character) {
	TOOLS_REMOVE_ENTITY(Project, character);
}

Character* Project::duplicateCharacter(Character* character) {
	TOOLS_DUPLICATE_ENTITY(Project, character);
}

void Project::cleanCharacters() {
	TOOLS_CLEAN_ENTITIES(Project, characters);
}

const QList<Link*>& Project::links() const {
	return _impl->links;
}

void Project::setLinks(const QList<Link*>& links) {
	TOOLS_SETTER(Project, links);
}

Link* Project::createLink() {
	TOOLS_CREATE_ENTITY(Project, links);
}

void Project::removeLink(Link* link) {
	TOOLS_REMOVE_ENTITY(Project, link);
}

Link* Project::duplicateLink(Link* link) {
	TOOLS_DUPLICATE_ENTITY(Project, link);
}

void Project::cleanLinks() {
	TOOLS_CLEAN_ENTITIES(Project, links);
}

const QList<Model*>& Project::models() const {
	return _impl->models;
}

void Project::setModels(const QList<Model*>& models) {
	TOOLS_SETTER(Project, models);
}

Model* Project::createModel() {
	TOOLS_CREATE_ENTITY(Project, models);
}

void Project::removeModel(Model* model) {
	TOOLS_REMOVE_ENTITY(Project, model);
}

Model* Project::duplicateModel(Model* model) {
	TOOLS_DUPLICATE_ENTITY(Project, model);
}

void Project::cleanModels() {
	TOOLS_CLEAN_ENTITIES(Project, models);
}

const QList<Place*>& Project::places() const {
	return _impl->places;
}

void Project::setPlaces(const QList<Place*>& places) {
	TOOLS_SETTER(Project, places);
}

Place* Project::createPlace() {
	TOOLS_CREATE_ENTITY(Project, places);
}

void Project::removePlace(Place* place) {
	TOOLS_REMOVE_ENTITY(Project, place);
}

Place* Project::duplicatePlace(Place* place) {
	TOOLS_DUPLICATE_ENTITY(Project, place);
}

void Project::cleanPlaces() {
	TOOLS_CLEAN_ENTITIES(Project, places);
}

constexpr auto lDefaultPlaces = "defaultPlace";
constexpr auto lCharacters = "characters";
constexpr auto lLinks = "links";
constexpr auto lModels = "models";
constexpr auto lPlaces = "places";

void Project::load(const QJsonObject& json) {
	reset();
	Entity::load(json);

	_impl->loadList(this, _impl->characters, lCharacters, json, &Project::charactersUpdated);
	_impl->loadList(this, _impl->links, lLinks, json, &Project::linksUpdated);
	_impl->loadList(this, _impl->models, lModels, json, &Project::modelsUpdated);
	_impl->loadList(this, _impl->places, lPlaces, json, &Project::placesUpdated);

	_impl->defaultPlace->setUuid(Json::toUuid(Json::toValue(lDefaultPlaces, json)));
	emit defaultPlaceUpdated();
}

void Project::save(QJsonObject& json) const {
	Entity::save(json);
	_impl->saveList(this, _impl->characters, lCharacters, json);
	_impl->saveList(this, _impl->links, lLinks, json);
	_impl->saveList(this, _impl->models, lModels, json);
	_impl->saveList(this, _impl->places, lPlaces, json);
	json[lDefaultPlaces] = Json::fromUuid(_impl->defaultPlace->uuid());
}
} // namespace fsd
