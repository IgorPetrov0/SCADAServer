#ifndef NEWPORTDIALOG_H
#define NEWPORTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <object.h>

namespace Ui {
class newPortDialog;
}

class newPortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newPortDialog(object *currentObject,QWidget *parent = nullptr);
    ~newPortDialog();
    void setPort(objectPort *port);
    objectPort *getPort();

protected:
    object *currentObject;
    objectPort *currentPort;
    void errorMessage(QString message);

protected slots:
    void okSlot();


private:
    Ui::newPortDialog *ui;
};

#endif // NEWPORTDIALOG_H
