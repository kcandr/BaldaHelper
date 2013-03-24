#ifndef CELLDELEGATE_H
#define CELLDELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>

class CellDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        if (lineEdit) {
            lineEdit->setAlignment(Qt::AlignCenter);
            lineEdit->setInputMask(">A;_");
        }
        return lineEdit;
    }
};

#endif // CELLDELEGATE_H
