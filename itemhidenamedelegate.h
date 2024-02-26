#ifndef ITEMHIDENAMEDELEGATE_H
#define ITEMHIDENAMEDELEGATE_H

#include <QStyledItemDelegate>

class ItemHideNameDelegate : public QStyledItemDelegate
{
public:
    explicit ItemHideNameDelegate(QObject *parent = nullptr);

    bool displayRoleEnabled = false;

protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // ITEMHIDENAMEDELEGATE_H
