#include "itemhidenamedelegate.h"

/**
 * @class ItemHideNameDelegate
 * @brief The ItemHideNameDelegate class provides a delegate for hiding item names in views.
 *
 * This class extends QStyledItemDelegate to provide custom painting behavior for item names in views, allowing them to be hidden.
 */


ItemHideNameDelegate::ItemHideNameDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{}

/**
 * @brief Initializes the style option option with the values from the item model index.
 *
 * @param option The style option to initialize.
 * @param index The model index of the item.
 */
void ItemHideNameDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    if (!displayRoleEnabled)
    {
            option->features &= ~QStyleOptionViewItem::HasDisplay;
    }
}

/**
 * @brief Paints the item specified by the given index and style option with the provided painter.
 *
 * This function overrides the default paint behavior to hide the text of the item.
 *
 * @param painter The painter used to draw the item.
 * @param option The style options for the item.
 * @param index The model index of the item to paint.
 */
void ItemHideNameDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem newOption(option);
    newOption.text = QString();
    QStyledItemDelegate::paint(painter, newOption, index);
}
