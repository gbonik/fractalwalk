#ifndef GRAPH_WIDGET_H
#define GRAPH_WIDGET_H

#include <QWidget>
#include <vector>

class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    GraphWidget(QWidget* parent = 0);

    const double* data() const { return &data_[0]; }
    int data_size() const { return data_.size(); }

    void set_data(const double* data, int sz);
    void set_lambda(double lambda);
    void set_message(QString message);
    void set_draw_parent(bool draw);

    double lambda() const { return lambda_; }

signals:
    void clicked();

private:
    virtual void paintEvent(QPaintEvent* ev);
    virtual void mouseReleaseEvent(QMouseEvent* );

    std::vector<double> data_;
    double              lambda_;
    QString             message_;
    QColor              graph_color_;
    bool                draw_parent_;
};

#endif

