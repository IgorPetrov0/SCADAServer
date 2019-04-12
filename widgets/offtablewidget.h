#ifndef OFFTABLEWIDGET_H
#define OFFTABLEWIDGET_H

#include <QWidget>
#include "logictablewidget.h"

namespace Ui {
class logicTableWidget;
}

class offTableWidget : public logicTableWidget
{
    Q_OBJECT

public:
    offTableWidget(QWidget *parent);
};

#endif // OFFTABLEWIDGET_H
