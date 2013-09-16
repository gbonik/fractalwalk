#include "jump_to_dialog.h"
#include <QPushButton>
#include "../fractal_laplacian.h"


JumpToDialog::JumpToDialog(int eig)
{
    ui_.setupUi(this);
    QPushButton* btn = new QPushButton("Jump");
    btn->setDefault(true);
    ui_.buttonBox->addButton(btn, QDialogButtonBox::AcceptRole);
    ui_.spin_eig->setValue(eig);

    connect(ui_.spin_eig, SIGNAL(valueChanged(int)), SLOT(spin_changed(int)));
    spin_changed(eig);
}


int JumpToDialog::get_number()
{
    return ui_.spin_eig->value();
}


void JumpToDialog::spin_changed(int val)
{
    std::vector<int> path = eigenvalue_number_to_path(val);

    QString label = "";
    for (size_t i = 0; i < path.size(); ++i)
    {
        if (i) label += ", ";
        label += QString("%1").arg(path[i]);
    }

    ui_.lbl_path->setText(label);
}

