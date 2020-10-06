#ifndef SOUNDANALYZER_H
#define SOUNDANALYZER_H

#include <QWidget>
#include <QtCharts>
#include <QVector>
#include "executor/executorinterface.h"

namespace CS {

namespace UI {

enum Quality
{
    Low = 0,
    Medium,
    High,
};

class SoundVisualizer : public QChartView
{
    Q_OBJECT
public:
    explicit SoundVisualizer(QWidget *parent = nullptr);
    ~SoundVisualizer();
    static void draw(SoundVisualizer*,const AudioBuffer&);
    static void plot(SoundVisualizer*);
    void loadSettings(Quality,int);
private:
    QChart* _chart;
    QLineSeries* _splineBuffer[4];
    int _stretchFactor = 1;
    int _bufferLoadDelay = 0;
    int _chartSamples = 128;
    std::thread* _drawThread;
private slots:
    void showSettings();
};

}
}

#endif // SOUNDANALYZER_H
