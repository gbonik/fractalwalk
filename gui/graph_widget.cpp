#include "graph_widget.h"
#include <QPainter>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

GraphWidget::GraphWidget(QWidget* parent) : QWidget(parent), graph_color_(0, 0, 200),
    draw_parent_(false)
{
}


void GraphWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    p.fillRect(rect(), Qt::white);

    const double vert_scale = 0.95;

    if (data_.size())
    {
        double max_y = 0.01;
        for (size_t i = 0; i < data_.size(); ++i)
            max_y = max(fabs(data_[i]), max_y);


        if (0.5 / max_y * (height() * vert_scale) / 2 > 8)
        {
            QPen grid_pen;
            for (double val = floor(-max_y * 1.2); val < max_y * 1.2; val += 0.1)
            {
                bool minor = false;
                grid_pen.setColor(QColor(220, 220, 220));
                if (fabs(round(val*2) - val*2) < 1e-8)
                {
                    grid_pen.setStyle(Qt::SolidLine);
                    if (fabs(val) < 1e-8)
                        grid_pen.setColor(QColor(150, 150, 150));
                }
                else
                {
                    grid_pen.setStyle(Qt::DashLine);
                    minor = true;
                }
                if (!minor || 0.1 / max_y * (height() * vert_scale) / 2 > 15)
                {
                    p.setPen(grid_pen);
                    int y = round(height() / 2 - (val / max_y * (height() * vert_scale) / 2));
                    p.drawLine(0, y, width(), y);
                }
            }
        }

        QPen graph_pen;
        graph_pen.setColor(graph_color_);

        if (int(data_.size()) <= width() * 5)
        {
            if (int(data_.size()) <= width() / 2)
                graph_pen.setWidth(2);
            else
                graph_pen.setWidth(1);

            p.setPen(graph_pen);
            QPainterPath path;
            for (size_t i = 0; i < data_.size(); ++i)
            {
                int x = i / double(data_.size() - 1) * width();
                int y = round(height() / 2 - (data_[i] / max_y * (height() * vert_scale) / 2));
                if (!i)
                    path.moveTo(x, y);
                else
                    path.lineTo(x, y);
            }
            p.drawPath(path);

            if (draw_parent_)
            {
                for (size_t i = 0; i < data_.size(); i += 3)
                {
                    int x = i / double(data_.size() - 1) * width();
                    int y = round(height() / 2 - (data_[i] / max_y * (height() * vert_scale) / 2));
                    p.fillRect(x - 2, y - 2, 5, 5, QColor(220, 0, 0));
                }
            }
        }
        else
        {
            p.setPen(graph_pen);
            for (int i = 0; i < width(); ++i)
            {
                int a = floor((i / double(width() - 1) * (data_.size() - 1)));
                a = max(a, 0);
                int b = ceil((i + 1) / double(width() - 1) * (data_.size() - 1));
                b = min(b, int(data_.size()) - 1);
                double min_val = 1e40, max_val = -1e40;
                for (int j = a; j <= b; ++j)
                {
                    min_val = min(data_[j], min_val);
                    max_val = max(data_[j], max_val);
                }

                int y1 = round(height() / 2 - min_val / max_y * height() * vert_scale / 2);
                int y2 = round(height() / 2 - max_val / max_y * height() * vert_scale / 2);

                if (y1 != y2)
                    p.drawLine(i, y1, i, y2);
                else
                    p.drawPoint(i, y1);
            }

        }

        QPen text_pen;
        p.setPen(text_pen);
        p.drawText(rect(), QString("λ = %1").arg(lambda_));
    }
    else
    {
        p.drawText(rect(), Qt::AlignCenter, message_);
    }
}


void GraphWidget::mouseReleaseEvent(QMouseEvent* )
{
    emit clicked();
}


void GraphWidget::set_data(const double* data, int sz)
{
    data_.resize(sz);
    if (data)
        memcpy(&data_[0], data, sizeof(double) * sz);
    update();
}


void GraphWidget::set_lambda(double lambda)
{
    lambda_ = lambda;
    update();
}


void GraphWidget::set_message(QString message)
{
    message_ = message;
    update();
}


void GraphWidget::set_draw_parent(bool draw)
{
    draw_parent_ = draw;
    update();
}

