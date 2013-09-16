#include "main_window.h"
#include "../fractal_laplacian.h"
#include <cmath>
#include <iostream>
#include "jump_to_dialog.h"

enum { MAX_PATH_LENGTH = 15 };

using namespace std;

MainWindow::MainWindow() : p_(0.3)
{
    ui_.setupUi(this);
    update_all();
    connect(ui_.thumbnail_0, SIGNAL(clicked()), SLOT(thumb_0_clicked()));
    connect(ui_.thumbnail_1, SIGNAL(clicked()), SLOT(thumb_1_clicked()));
    connect(ui_.thumbnail_2, SIGNAL(clicked()), SLOT(thumb_2_clicked()));
    connect(ui_.btn_up, SIGNAL(clicked()), SLOT(btn_up_clicked()));
    connect(ui_.spin_p, SIGNAL(valueChanged(double)), SLOT(spin_p_changed(double)));
    connect(ui_.btn_prev, SIGNAL(clicked()), SLOT(btn_prev_clicked()));
    connect(ui_.btn_next, SIGNAL(clicked()), SLOT(btn_next_clicked()));
    connect(ui_.btn_jump_to, SIGNAL(clicked()), SLOT(btn_jump_to_clicked()));
    connect(ui_.check_show_parent, SIGNAL(stateChanged(int)), SLOT(check_show_parent_changed(int)));
    ui_.lbl_maxlevel->hide();
}


void MainWindow::thumb_0_clicked()
{
    path_.push_back(0);
    thumb_clicked(ui_.thumbnail_0);
}


void MainWindow::thumb_1_clicked()
{
    if (!path_is_constant())
        path_.push_back(1);
    thumb_clicked(ui_.thumbnail_1);
}


void MainWindow::thumb_2_clicked()
{
    path_.push_back(2);
    thumb_clicked(ui_.thumbnail_2);
}


void MainWindow::btn_up_clicked()
{
    if (path_.size())
        path_.pop_back();
    update_all();

    if (!path_.size())
        ui_.btn_up->setEnabled(false);
}


void MainWindow::spin_p_changed(double value)
{
    p_ = value;
    update_all();
}


void MainWindow::btn_prev_clicked()
{
    if (path_.size())
    {
        if (path_is_constant())
        {
            if (path_.back() == 2)
                path_.back() = 0;
        }
        else
        {
            int first_diff = 1;
            for (size_t i = 1; i < path_.size(); ++i)
                if (path_[i] != path_[0])
                {
                    first_diff = i;
                    break;
                }

            int i;
            for (i = path_.size() - 1; i > first_diff; --i)
            {
                if (path_[i]-- == 0)
                    path_[i] = 2;
                else
                    break;
            }

            if (i == first_diff)
            {
                if (path_[first_diff] == 0)
                {
                    path_[first_diff] = 2;
                    path_[first_diff - 1] = 0;
                }
                else
                    path_[first_diff] = 0;
            }
        }
        update_all();
    }
}


void MainWindow::btn_next_clicked()
{
    if (path_.size())
    {
        if (path_is_constant())
        {
            if (path_.back() == 0)
                path_.back() = 2;
        }
        else
        {
            int first_diff = 1;
            for (size_t i = 1; i < path_.size(); ++i)
                if (path_[i] != path_[0])
                {
                    first_diff = i;
                    break;
                }

            int i;
            for (i = path_.size() - 1; i > first_diff; --i)
            {
                if (path_[i]++ == 2)
                    path_[i] = 0;
                else
                    break;
            }

            if (i == first_diff)
            {
                if (path_[first_diff] == 0)
                    path_[first_diff] = 2;
                else
                {
                    path_[first_diff] = 0;
                    path_[first_diff - 1] = 2;
                }
            }
        }
        update_all();
    }

}


void MainWindow::btn_jump_to_clicked()
{
    int eig = path_.size() ? path_to_eigenvalue_number(&path_[0], path_.size()) : 0;

    JumpToDialog dialog(eig);
    int r = dialog.exec();
    if (r == QDialog::Accepted)
    {
        int eig = dialog.get_number();
        path_ = eigenvalue_number_to_path(eig);
        update_all();
    }
}


void MainWindow::check_show_parent_changed(int state)
{
    ui_.graph->set_draw_parent(state == Qt::Checked);
}


void MainWindow::thumb_clicked(GraphWidget* thumb)
{
    ui_.graph->set_data(thumb->data(), thumb->data_size());
    ui_.graph->set_lambda(thumb->lambda());
    update_thumbnails();
    update_level();
    ui_.btn_up->setEnabled(true);
}


void MainWindow::update_thumbnails()
{
    if (path_.size() <= MAX_PATH_LENGTH)
    {
        bool all_same = path_is_constant();
        if (all_same)
            ui_.thumbnail_1->hide();
        else
            ui_.thumbnail_1->show();
        ui_.lbl_maxlevel->hide();
        ui_.thumbnail_0->show();
        ui_.thumbnail_2->show();

        int level = path_.size();
        int size = size_for_level(level);
        vector<double> f(size);

        path_.push_back(0);
        double lambda = compute_eigenpair(p_, level, &path_[0], &f[0]);
        ui_.thumbnail_0->set_data(&f[0], size);
        ui_.thumbnail_0->set_lambda(lambda);
        path_.pop_back();

        if (!all_same)
        {
            path_.push_back(1);
            lambda = compute_eigenpair(p_, level, &path_[0], &f[0]);
            ui_.thumbnail_1->set_data(&f[0], size);
            ui_.thumbnail_1->set_lambda(lambda);
            path_.pop_back();
        }

        path_.push_back(2);
        lambda = compute_eigenpair(p_, level, &path_[0], &f[0]);
        ui_.thumbnail_2->set_data(&f[0], size);
        ui_.thumbnail_2->set_lambda(lambda);
        path_.pop_back();
    }
    else
    {
        ui_.thumbnail_0->hide();
        ui_.thumbnail_1->hide();
        ui_.thumbnail_2->hide();
        ui_.lbl_maxlevel->show();
    }
}


void MainWindow::update_all()
{
    update_thumbnails();
    update_level();

    if (path_.size())
    {
        int level = path_.size() - 1;
        int size = size_for_level(level);
        vector<double> f(size);
        double lambda = compute_eigenpair(p_, level, &path_[0], &f[0]);
        ui_.graph->set_data(&f[0], size);
        ui_.graph->set_lambda(lambda);
    }
    else
    {
        ui_.graph->set_data(0, 0);
        ui_.graph->set_message("Select initial eigenfunction below");
    }
}


void MainWindow::update_level()
{
    if (path_.size())
    {
        QString label = QString("Level %1 [").arg(path_.size() - 1);
        for (size_t i = 0; i < path_.size(); ++i)
        {
            label += QString("%1").arg(path_[i]);
        }

        int eig = path_to_eigenvalue_number(&path_[0], path_.size());

        ui_.lbl_path->setText(label + "] #" + QString("%1").arg(eig));

        if (path_is_constant())
        {
            ui_.btn_prev->setEnabled(path_[0] == 2);
            ui_.btn_next->setEnabled(path_[0] == 0);
        }
        else
        {
            ui_.btn_prev->setEnabled(true);
            ui_.btn_next->setEnabled(true);
        }
    }
    else
    {
        ui_.lbl_path->setText("");
        ui_.btn_prev->setEnabled(false);
        ui_.btn_next->setEnabled(false);
    }

}


bool MainWindow::path_is_constant()
{
    for (size_t i = 1; i < path_.size(); ++i)
        if (path_[i] != path_[0])
            return false;
    return true;
}

