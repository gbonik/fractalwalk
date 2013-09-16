#ifndef JUMP_TO_DIALOG_H
#define JUMP_TO_DIALOG_H

#include <QDialog>
#include "ui/ui_jump_to_dialog.h"

class JumpToDialog : public QDialog
{
    Q_OBJECT
public:
    JumpToDialog(int eig);
    int get_number();

private:
    Ui::JumpToDialog      ui_;

private slots:
    void spin_changed(int i);
};

#endif

