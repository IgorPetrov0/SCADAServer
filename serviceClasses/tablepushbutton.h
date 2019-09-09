#ifndef TABLEPUSHBUTTON_H
#define TABLEPUSHBUTTON_H

#include <QPushButton>

class tablePushButton : public QPushButton
{
    Q_OBJECT
public:
    tablePushButton(int index, QWidget *parent=nullptr);

protected:
    int index;


protected slots:
    void pushSlot();

signals:
    void pushSignal(bool state, int index);


};

#endif // TABLEPUSHBUTTON_H
