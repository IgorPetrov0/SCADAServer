#ifndef NEWPORTDIALOG_H
#define NEWPORTDIALOG_H

#include <QDialog>

namespace Ui {
class newPortDialog;
}

class newPortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newPortDialog(QWidget *parent = 0);
    ~newPortDialog();

private:
    Ui::newPortDialog *ui;
};

#endif // NEWPORTDIALOG_H
