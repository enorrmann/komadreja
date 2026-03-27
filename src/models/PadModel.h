#ifndef PADMODEL_H
#define PADMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QColor>

struct PadData {
    int index;
    QString name;
    bool hasSample;
    QColor color;
};

class PadModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PadRoles {
        IndexRole = Qt::UserRole + 1,
        NameRole,
        HasSampleRole,
        ColorRole
    };

    explicit PadModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void activatePad(int index);

private:
    QVector<PadData> m_pads;
};

#endif // PADMODEL_H
