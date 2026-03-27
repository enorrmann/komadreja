#include "PadModel.h"
#include <QColor>
#include "config.h"

PadModel::PadModel(QObject *parent)
    : QAbstractListModel(parent)
{

    for (int i = 0; i < NUM_PADS; ++i) {
        PadData pad;
        pad.index = i;
        pad.name = QString::number(i + 1);
        pad.hasSample = false;
        // Assign some colorful defaults
        int hue = (i * 15) % 360;
        pad.color = QColor::fromHsv(hue, 150, 200);
        m_pads.append(pad);
    }
}

int PadModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_pads.count();
}

QVariant PadModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_pads.count())
        return QVariant();

    const PadData &pad = m_pads[index.row()];

    switch (role) {
    case IndexRole:
        return pad.index;
    case NameRole:
        return pad.name;
    case HasSampleRole:
        return pad.hasSample;
    case ColorRole:
        return pad.color;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> PadModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "padIndex";
    roles[NameRole] = "padName";
    roles[HasSampleRole] = "hasSample";
    roles[ColorRole] = "padColor";
    return roles;
}

void PadModel::activatePad(int index)
{
    // Trigger audio mapping logic later
    if (index >= 0 && index < m_pads.count()) {
        // Just an Example
    }
}
