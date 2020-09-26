#include "soundanalyzer.h"
#include <QPainter>
#include "UI/uidefinitions.h"
#include <mutex>

using namespace CS::UI;
using namespace QtCharts;

mutex bufferMutex;

bool ploting = true;
bool newBuffer = false;

CS::AudioBuffer localBuffer;

int currentSplineBuffer = 0;

const int MAX_SIZE_WIDTH = 256;

SoundAnalyzer::SoundAnalyzer(QWidget *parent) : QChartView(parent){
    UiDefinitions* def = UiDefinitions::getInstance();

    setBackgroundBrush(QBrush(def->getColor(ColorId::Background)));

    QPen pen1(def->getColor(ColorId::Primary));
    pen1.setWidth(2);
    QPen pen2(def->getColor(ColorId::Secondary));
    pen2.setWidth(2);

    _splineBuffer[0] = new QLineSeries(this);
    _splineBuffer[0]->setName("Left Channel");
    _splineBuffer[0]->setPen(pen1);
    _splineBuffer[1] = new QLineSeries(this);
    _splineBuffer[1]->setName("Right Channel");
    _splineBuffer[1]->setPen(pen2);
    _splineBuffer[2] = new QLineSeries(this);
    _splineBuffer[2]->setName("Left Channel");
    _splineBuffer[2]->setPen(pen1);
    _splineBuffer[2]->hide();
    _splineBuffer[3] = new QLineSeries(this);
    _splineBuffer[3]->setName("Right Channel");
    _splineBuffer[3]->setPen(pen2);
    _splineBuffer[3]->hide();

    _chart = new QChart();
    _chart->setBackgroundVisible(false);
    _chart->setBackgroundRoundness(1);
    _chart->setMargins(QMargins(10,10,10,10));
//    _chart->legend()->hide();
    _chart->addSeries(_splineBuffer[0]);
    _chart->addSeries(_splineBuffer[1]);
    _chart->addSeries(_splineBuffer[2]);
    _chart->addSeries(_splineBuffer[3]);
    _chart->createDefaultAxes();
    for( auto axe : _chart->axes(Qt::Vertical) )
    {
        axe->setRange(-1, 1);
        axe->hide();
    }
    for( auto axe : _chart->axes(Qt::Horizontal) )
    {
        axe->setRange(0, MAX_SIZE_WIDTH);
        axe->hide();
    }

    setChart(_chart);
    setRenderHint(QPainter::Antialiasing,false);

    using namespace std::placeholders;
    ExecutorInterface::addObserver(bind(&SoundAnalyzer::draw,this,_1));

    localBuffer.resize(2);
    localBuffer[0].resize(MAX_SIZE_WIDTH);
    localBuffer[1].resize(MAX_SIZE_WIDTH);

    _drawThread = new std::thread(bind(&SoundAnalyzer::plot,this));
}

SoundAnalyzer::~SoundAnalyzer() {
    ploting = false;
    delete _chart;
    _drawThread->join();
    delete _drawThread;
}

void switchBuffer() {
    if( currentSplineBuffer == 0 )
        currentSplineBuffer = 2;
    else
        currentSplineBuffer = 0;
}

void SoundAnalyzer::plot(SoundAnalyzer* instance) {
    while( ploting )
    {
        if( newBuffer )
        {
            instance->_splineBuffer[0 + currentSplineBuffer]->hide();
            instance->_splineBuffer[1 + currentSplineBuffer]->hide();

            switchBuffer();

            instance->_splineBuffer[0 + currentSplineBuffer]->show();
            instance->_splineBuffer[1 + currentSplineBuffer]->show();

            switchBuffer();

            QLineSeries *serieRight = instance->_splineBuffer[0 + currentSplineBuffer];
            QLineSeries *serieLeft = instance->_splineBuffer[1 + currentSplineBuffer];

            bufferMutex.lock();

            serieRight->clear();
            serieLeft->clear();

            for( size_t i = 0; i < localBuffer[0].size()/40; i++ )
            {
                serieLeft->append(i,localBuffer[1][i*2]);
                serieRight->append(i,localBuffer[0][i*2]);
            }

            switchBuffer();

            newBuffer = false;

            bufferMutex.unlock();
        }
    }
}

void SoundAnalyzer::draw(SoundAnalyzer*,const AudioBuffer& valuesToPrint) {
    static int count = 0;
    if( bufferMutex.try_lock() )
    {
        localBuffer = valuesToPrint;
        if( count < 1 )
            count++;
        else
        {
            newBuffer = true;
            count = 0;
        }
        bufferMutex.unlock();
    }
}
