#ifndef SOUNDANALYZER_H
#define SOUNDANALYZER_H

#include <QWidget>
#include <QtCharts>
#include <QVector>
#include "executor/executorinterface.h"

namespace CS {

namespace UI {

class SoundAnalyzer : public QChartView
{
    Q_OBJECT
public:
    explicit SoundAnalyzer(QWidget *parent = nullptr);
    ~SoundAnalyzer();
    static void draw(SoundAnalyzer*,const AudioBuffer&);
    static void plot(SoundAnalyzer*);
private:
    QChart* _chart;
    QLineSeries* _splineBuffer[4];
    std::thread* _drawThread;
};

}
}

#endif // SOUNDANALYZER_H
