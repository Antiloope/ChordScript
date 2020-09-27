#include "soundvisualizer.h"
#include <QPainter>
#include "UI/uidefinitions.h"
#include "UI/toolbox/plotsettingsdialog.h"
#include <mutex>

using namespace CS::UI;
using namespace QtCharts;

mutex bufferMutex;

bool ploting = true;
bool newBuffer = false;

CS::AudioBuffer localBuffer;

namespace {

const QString SETTINGS_ICON_RESOURCE = QString::fromUtf8(":/icons/resources/settings.svg");

const int CHART_PEN_SIZE = 2;

enum Channel
{
    Right = 0,
    Left,
};

enum ChartBuffer
{
    First = 0,
    Second = 2,
} currentSplineBuffer = First;

}

SoundVisualizer::SoundVisualizer(QWidget *parent) : QChartView(parent) {
    UiDefinitions* def = UiDefinitions::getInstance();

    setBackgroundBrush(QBrush(def->getColor(ColorId::Background)));

    QPen penRight(def->getColor(ColorId::Primary));
    penRight.setWidth(CHART_PEN_SIZE);
    QPen penLeft(def->getColor(ColorId::Secondary));
    penLeft.setWidth(CHART_PEN_SIZE);

    _splineBuffer[Channel::Right + ChartBuffer::First] = new QLineSeries(this);
    _splineBuffer[Channel::Right + ChartBuffer::First]->setName("Left Channel");
    _splineBuffer[Channel::Right + ChartBuffer::First]->setPen(penRight);
    _splineBuffer[Channel::Left + ChartBuffer::First] = new QLineSeries(this);
    _splineBuffer[Channel::Left + ChartBuffer::First]->setName("Right Channel");
    _splineBuffer[Channel::Left + ChartBuffer::First]->setPen(penLeft);
    _splineBuffer[Channel::Right + ChartBuffer::Second] = new QLineSeries(this);
    _splineBuffer[Channel::Right + ChartBuffer::Second]->setName("Left Channel");
    _splineBuffer[Channel::Right + ChartBuffer::Second]->setPen(penRight);
    _splineBuffer[Channel::Right + ChartBuffer::Second]->hide();
    _splineBuffer[Channel::Left + ChartBuffer::Second] = new QLineSeries(this);
    _splineBuffer[Channel::Left + ChartBuffer::Second]->setName("Right Channel");
    _splineBuffer[Channel::Left + ChartBuffer::Second]->setPen(penLeft);
    _splineBuffer[Channel::Left + ChartBuffer::Second]->hide();

    _chart = new QChart();
    _chart->setBackgroundVisible(false);
    _chart->setBackgroundRoundness(1);
    _chart->setMargins(QMargins(10,10,10,10));
    _chart->addSeries(_splineBuffer[Channel::Right + ChartBuffer::First]);
    _chart->addSeries(_splineBuffer[Channel::Left + ChartBuffer::First]);
    _chart->addSeries(_splineBuffer[Channel::Right + ChartBuffer::Second]);
    _chart->addSeries(_splineBuffer[Channel::Left + ChartBuffer::Second]);
    _chart->legend()->markers()[Channel::Right + ChartBuffer::First]->setPen(penRight);
    _chart->legend()->markers()[Channel::Right + ChartBuffer::First]->setShape(QLegend::MarkerShapeCircle);
    _chart->legend()->markers()[Channel::Left + ChartBuffer::First]->setPen(penLeft);
    _chart->legend()->markers()[Channel::Left + ChartBuffer::First]->setShape(QLegend::MarkerShapeCircle);
    _chart->legend()->markers()[Channel::Right + ChartBuffer::Second]->setPen(penRight);
    _chart->legend()->markers()[Channel::Right + ChartBuffer::Second]->setShape(QLegend::MarkerShapeCircle);
    _chart->legend()->markers()[Channel::Left + ChartBuffer::Second]->setPen(penLeft);
    _chart->legend()->markers()[Channel::Left + ChartBuffer::Second]->setShape(QLegend::MarkerShapeCircle);
    _chart->createDefaultAxes();

    for( auto axe : _chart->axes(Qt::Vertical) )
    {
        axe->setRange(-1, 1);
        axe->hide();
    }
    for( auto axe : _chart->axes(Qt::Horizontal) )
    {
        axe->setRange(0, _chartSamples);
        axe->hide();
    }

    setChart(_chart);
    setRenderHint(QPainter::Antialiasing, false);

    QPushButton* settingsButton = new QPushButton(this);
    QIcon icon1;
    icon1.addFile(SETTINGS_ICON_RESOURCE, QSize(), QIcon::Normal, QIcon::On);
    settingsButton->setIcon(icon1);
    settingsButton->setIconSize(QSize(25,25));
    settingsButton->setStyleSheet(
        "QPushButton {"
            "border: none;"
            "padding: 1px;"
            "background-color: transparent;"
        "}"
        "QPushButton:hover {"
            "background-color: " + def->getColorRGB(ColorId::Light) + ";"
        "}");

    connect(settingsButton,SIGNAL(clicked()),this,SLOT(showSettings()));

    using namespace std::placeholders;
    ExecutorInterface::addObserver(bind(&SoundVisualizer::draw,this,_1));

    localBuffer.resize(2);
    localBuffer[Channel::Right].resize(_chartSamples);
    localBuffer[Channel::Left].resize(_chartSamples);

    _drawThread = new std::thread(bind(&SoundVisualizer::plot,this));
}

SoundVisualizer::~SoundVisualizer() {
    ploting = false;
    delete _chart;
    _drawThread->join();
    delete _drawThread;
}

void SoundVisualizer::showSettings() {
    PlotSettingsDialog settings(_chartSamples,_stretchFactor,this);
    settings.exec();
}

void SoundVisualizer::loadSettings(Quality quality,int stretchFactor) {
    switch( quality )
    {
    case Low:
        _bufferLoadDelay = 1;
        _chartSamples = 64;
        _stretchFactor = stretchFactor;
        for( auto axe : _chart->axes(Qt::Horizontal) )
        {
            axe->setRange(0, _chartSamples);
            axe->hide();
        }
        break;
    case Medium:
        _bufferLoadDelay = 0;
        _chartSamples = 128;
        _stretchFactor = stretchFactor;
        for( auto axe : _chart->axes(Qt::Horizontal) )
        {
            axe->setRange(0, _chartSamples);
            axe->hide();
        }
        break;
    case High:
        _bufferLoadDelay = 0;
        _chartSamples = 256;
        _stretchFactor = stretchFactor;
        for( auto axe : _chart->axes(Qt::Horizontal) )
        {
            axe->setRange(0, _chartSamples);
            axe->hide();
        }
        break;
    }
}

void switchBuffer() {
    if( currentSplineBuffer == ChartBuffer::First )
        currentSplineBuffer = ChartBuffer::Second;
    else
        currentSplineBuffer = ChartBuffer::First;
}

void SoundVisualizer::plot(SoundVisualizer* instance) {
    while( ploting )
    {
        if( newBuffer && instance->isVisible() )
        {
            instance->_splineBuffer[Channel::Right + currentSplineBuffer]->hide();
            instance->_splineBuffer[Channel::Left + currentSplineBuffer]->hide();

            switchBuffer();

            instance->_splineBuffer[Channel::Right + currentSplineBuffer]->show();
            instance->_splineBuffer[Channel::Left + currentSplineBuffer]->show();

            switchBuffer();

            QLineSeries *serieRight = instance->_splineBuffer[Channel::Right + currentSplineBuffer];
            QLineSeries *serieLeft = instance->_splineBuffer[Channel::Left + currentSplineBuffer];

            bufferMutex.lock();

            serieRight->clear();
            serieLeft->clear();

            for( int i = 0; i < instance->_chartSamples; i++ )
            {
                serieLeft->append(i,localBuffer[Channel::Left][i*instance->_stretchFactor]);
                serieRight->append(i,localBuffer[Channel::Right][i*instance->_stretchFactor]);
            }

            switchBuffer();

            newBuffer = false;

            bufferMutex.unlock();
        }
    }
}

void SoundVisualizer::draw(SoundVisualizer* instance,const AudioBuffer& valuesToPrint) {
    static int count = 0;
    if( bufferMutex.try_lock() )
    {
        localBuffer = valuesToPrint;
        if( count < instance->_bufferLoadDelay )
            count++;
        else
        {
            newBuffer = true;
            count = 0;
        }
        bufferMutex.unlock();
    }
}
