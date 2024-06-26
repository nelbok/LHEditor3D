#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUuid>

namespace fsd {
class Entity : public QObject {
	Q_OBJECT
	Q_PROPERTY(QUuid uuid READ uuid WRITE setUuid NOTIFY uuidUpdated)
	Q_PROPERTY(bool isAlive READ isAlive WRITE setIsAlive NOTIFY isAliveUpdated)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameUpdated)
	Q_PROPERTY(bool hasRef READ hasRef NOTIFY hasRefUpdated)

public:
	Entity(QObject* parent = nullptr);
	virtual ~Entity();

	virtual void reset();
	void copy(const Entity& entity);

	const QUuid& uuid() const;
	void setUuid(const QUuid& uuid);

	bool isAlive() const;
	void setIsAlive(bool isAlive);

	const QString& name() const;
	void setName(const QString& name);

	const QList<Entity*>& refs() const;
	void addRef(Entity* ref);
	void removeRef(Entity* ref);
	bool hasRef();

	virtual void load(const QJsonObject& json);
	virtual void save(QJsonObject& json) const;

private:
	struct Impl;
	std::unique_ptr<Impl> _impl;

signals:
	void uuidUpdated();
	void isAliveUpdated();
	void nameUpdated();
	void hasRefUpdated();
};
} // namespace fsd
