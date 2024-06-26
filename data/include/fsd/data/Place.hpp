#pragma once

#include <fsd/data/Object.hpp>

namespace fsd {
class Project;

class Place : public Object {
	Q_OBJECT

public:
	Place(Project* project);
	virtual ~Place();

	virtual void reset() override;
	void copy(const Place& place);
	virtual void load(const QJsonObject& json) override;
	virtual void save(QJsonObject& json) const override;

private:
	struct Impl;
	std::unique_ptr<Impl> _impl;
};
} // namespace fsd
