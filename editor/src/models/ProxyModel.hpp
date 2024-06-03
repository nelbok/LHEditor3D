#pragma once

#include <QtCore/QSortFilterProxyModel>

namespace lhe {
class ProxyModel : public QSortFilterProxyModel {
	Q_OBJECT
	Q_PROPERTY(QVariantMap filters READ filters WRITE setFilters NOTIFY filtersUpdated)

public:
	ProxyModel(QObject* parent = nullptr);
	virtual ~ProxyModel();

	const QVariantMap& filters() const;
	void setFilters(const QVariantMap& filters);

protected:
	virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

private:
	int getRole(const QByteArray& name) const;

private:
	QVariantMap _filters{};

signals:
	void filtersUpdated();
};

} // namespace lhe
