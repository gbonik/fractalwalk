#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "ui/ui_main_window.h"

#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

private slots:
    void thumb_0_clicked();
    void thumb_1_clicked();
    void thumb_2_clicked();
    void btn_up_clicked();
    void spin_p_changed(double value);
    void btn_prev_clicked();
    void btn_next_clicked();
    void btn_jump_to_clicked();
    void check_show_parent_changed(int state);

private:
    Ui::MainWindow      ui_;
    double              p_;
    std::vector<int>    path_;

    void thumb_clicked(GraphWidget* thumb);
    void update_thumbnails();
    void update_all();
    void update_level();
    bool path_is_constant();
};

#endif

