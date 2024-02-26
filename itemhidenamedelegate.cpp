#include "itemhidenamedelegate.h"

ItemHideNameDelegate::ItemHideNameDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

void ItemHideNameDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    if (!displayRoleEnabled)
    {
            option->features &= ~QStyleOptionViewItem::HasDisplay;
    }
}

void ItemHideNameDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem newOption(option);
    newOption.text = QString();
    QStyledItemDelegate::paint(painter, newOption, index);
}
