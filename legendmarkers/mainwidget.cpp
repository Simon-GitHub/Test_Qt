/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtCore/QDebug>
#include <QtCharts/QLegend>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QXYLegendMarker>
#include <QtCharts/QValueAxis>
#include <QtCore/QtMath>
#include <QThread>
#include <QTimer>
#include <QPushButton>
QT_CHARTS_USE_NAMESPACE

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
		, m_num(0)
{
	//setStyleSheet("QWidget{border: 2px solid red;}");
    // Create chart view with the chart
    m_chart = new QChart();
	// 去除边框
	m_chart->layout()->setContentsMargins(0,0,0,0);
	m_chart->setBackgroundBrush(QColor(0, 0, 0, 255));
    m_chartView = new QChartView(m_chart, this);

    // Create layout for grid and detached legend
    m_mainLayout = new QGridLayout();
	m_mainLayout->setContentsMargins(0,0,0,0);
	m_mainLayout->setSpacing(0);
	m_startPushButton = new QPushButton(QString::fromLocal8Bit("开始"));
	m_startPushButton->setCheckable(true);
	connect(m_startPushButton, SIGNAL(clicked(bool)), this, SLOT(pushButtonSlot(bool)));
	m_mainLayout->addWidget(m_chartView, 0, 0, 1, 1);
	m_mainLayout->addWidget(m_startPushButton, 1, 0, 1, 1);
    setLayout(m_mainLayout);

    // Add few series
    //addSeries();
    //addSeries();
    //addSeries();
    //addSeries();

    connectMarkers();

    // Set the title and show legend
    m_chart->setTitle(QString::fromLocal8Bit("数据对比曲线"));
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    m_chartView->setRenderHint(QPainter::Antialiasing);
    //m_chartView->setRubberBand(QChartView::RectangleRubberBand);
	m_timer = new QTimer;
	connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));

	QFile file("data.csv");  
	if(!file.open(QIODevice::ReadOnly))  
		qDebug()<<"OPEN FILE FAILED";  
	QTextStream * out = new QTextStream(&file);//文本流  
	m_data = out->readAll().split("\n");//每行以\n区分  
	file.close();//操作完成后记得关闭文件
}

void MainWidget::addSeries()
{
    QLineSeries *series = new QLineSeries();
    m_series.append(series);

    series->setName(QString("line " + QString::number(m_series.count())));

    // Make some sine wave for data
    QList<QPointF> data;
    int offset = m_chart->series().count();
    for (int i = 0; i < 360; i++) {
        qreal x = offset * 20 + i;
        data.append(QPointF(i, qSin(qDegreesToRadians(x))));
    }

    series->append(data);
    m_chart->addSeries(series);

    if (m_series.count() == 1)
        m_chart->createDefaultAxes();
}

void MainWidget::removeSeries()
{
    // Remove last series from chart
    if (m_series.count() > 0) {
        QLineSeries *series = m_series.last();
        m_chart->removeSeries(series);
        m_series.removeLast();
        delete series;
    }
}

void MainWidget::connectMarkers()
{
//![1]
    // Connect all markers to handler
    const QList <QLegendMarker*> markers = m_chart->legend()->markers();
	for (QList <QLegendMarker*>::ConstIterator it = markers.begin(); it != markers.end(); ++it)
	{
		QLegendMarker *marker = *it;
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWidget::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &MainWidget::handleMarkerClicked);
    }
//![1]
}

void MainWidget::disconnectMarkers()
{
//![2]
	const QList <QLegendMarker*> markers = m_chart->legend()->markers();
	for (QList <QLegendMarker*>::ConstIterator it = markers.begin(); it != markers.end(); ++it)
	{
		QLegendMarker *marker = *it;
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWidget::handleMarkerClicked);
    }
//![2]
}

void MainWidget::handleMarkerClicked()
{
//![3]
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
//![3]

//![4]
    switch (marker->type())
//![4]
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
//![5]
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);
//![5]

//![6]
        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible())
            alpha = 0.5;

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

//![6]
        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}

void MainWidget::pushButtonSlot(bool isClicked)
{
	if (isClicked)
	{
		m_timer->start(1000);
		m_startPushButton->setText(QString::fromLocal8Bit("暂停"));
	}
	else
	{
		m_timer->stop();
		m_startPushButton->setText(QString::fromLocal8Bit("继续"));
	}
}

void MainWidget::timeoutSlot()
{
	if (m_num < m_data.size())
	{
		QStringList tempbar = m_data.at(m_num).split(",");//一行中的单元格以，区分  
		++m_num;
		if(tempbar.size() != 20) return;
		m_chart->removeAllSeries();
		QList<QAbstractAxis *> listAxis = m_chart->axes();
		for (int k = 0; k < listAxis.size(); ++k)
		{
			m_chart->removeAxis(listAxis[k]);
		}
		m_series.clear();
		QList<QPointF> listPointF;
		QList<double> listValue;
		for (int j = 0; j < 10; ++j)
		{
			listPointF.push_back(QPointF(j+1, tempbar[j].toDouble()));
			listValue.push_back(tempbar[j].toDouble());
		}
		qSort(listValue.begin(), listValue.end());
		addSeries(listPointF, *listValue.begin(), *(--listValue.end()));
		listPointF.clear();
		listValue.clear();
		for (int j = 10; j < 20; ++j)
		{
			listPointF.push_back(QPointF(j-9, tempbar[j].toDouble()));
			listValue.push_back(tempbar[j].toDouble());
		}
		qSort(listValue.begin(), listValue.end());
		addSeries(listPointF, *listValue.begin(), *(--listValue.end()));
	}
	else
	{
		m_timer->stop();
		m_num = 0;
		m_startPushButton->setText(QString::fromLocal8Bit("开始"));
	}
}

void MainWidget::addSeries(const QList<QPointF>& listPointF, double min, double max)
{
	QScatterSeries* scatterSeries = new QScatterSeries();//创建散点
	scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);//设置散点样式
	scatterSeries->setMarkerSize(6);//设置散点大小
	QLineSeries *series = new QLineSeries();
	m_series.append(series);

	series->setName(QString("line " + QString::number(m_series.count())));

	series->append(listPointF);
	scatterSeries->append(listPointF);
	m_chart->addSeries(series);
	m_chart->addSeries(scatterSeries);

	QList<QAbstractAxis *> listAxis = m_chart->axes();
	if (listAxis.empty())
	{
		QValueAxis *axisX = new QValueAxis;
		axisX->setTitleText("X");
		axisX->setTickCount(10);
		axisX->setLabelFormat("%i");
		m_chart->addAxis(axisX, Qt::AlignBottom);
		series->attachAxis(axisX);
		scatterSeries->attachAxis(axisX);

		QValueAxis *axisY = new QValueAxis;
		axisY->setTitleText("Y");
		axisY->setTickCount(10);
		axisY->setLabelFormat("%i");
		m_chart->addAxis(axisY, Qt::AlignLeft);
		series->attachAxis(axisY);
		scatterSeries->attachAxis(axisY);
	}
	else
	{
		QValueAxis* axisX = dynamic_cast<QValueAxis*>(m_chart->axisX());
		if (axisX)
		{
			series->attachAxis(axisX);
			scatterSeries->attachAxis(axisX);
			m_chart->removeAxis(axisX);
			m_chart->addAxis(axisX, Qt::AlignBottom);
		}
		QValueAxis* axisY = dynamic_cast<QValueAxis*>(m_chart->axisY());
		if (axisY)
		{
			qreal yMin = axisY->min();
			qreal yMax = axisY->max();
			if (min < yMin)
			{
				axisY->setMin(min);
			}
			if (max > yMax)
			{
				axisY->setMax(max);
			}
			series->attachAxis(axisY);
			scatterSeries->attachAxis(axisY);
			m_chart->removeAxis(axisY);
			m_chart->addAxis(axisY, Qt::AlignLeft);
		}
	}
}