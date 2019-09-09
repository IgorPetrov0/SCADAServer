#ifndef PORTSMANUALDIALOG_H
#define PORTSMANUALDIALOG_H

#include <QDialog>
#include "object.h"
#include "serviceClasses/tablepushbutton.h"

namespace Ui {
class portsManualDialog;
}

class portsManualDialog : public QDialog
{
    Q_OBJECT

public:
    explicit portsManualDialog(object *currentObject, QWidget *parent = nullptr);
    void updateContent();
    ~portsManualDialog();

protected:
    object *currentObject;

protected slots:
    void changePortStateSlot(bool state, int index);
    void updateSlot();


private:
    Ui::portsManualDialog *ui;
};

#endif // PORTSMANUALDIALOG_H
