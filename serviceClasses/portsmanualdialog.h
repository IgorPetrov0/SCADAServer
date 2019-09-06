#ifndef PORTSMANUALDIALOG_H
#define PORTSMANUALDIALOG_H

#include <QDialog>

namespace Ui {
class portsManualDialog;
}

class portsManualDialog : public QDialog
{
    Q_OBJECT

public:
    explicit portsManualDialog(QWidget *parent = nullptr);
    ~portsManualDialog();

private:
    Ui::portsManualDialog *ui;
};

#endif // PORTSMANUALDIALOG_H
