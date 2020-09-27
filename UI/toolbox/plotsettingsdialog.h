#ifndef PLOTSETTINGSDIALOG_H
#define PLOTSETTINGSDIALOG_H

#include <QWidget>
#include <QDialog>

class QComboBox;

namespace CS {
namespace UI {

class PlotSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlotSettingsDialog(int,int,QWidget *parent = nullptr);
private:
    QComboBox* _qualityPicker;
    QComboBox* _stretchFactor;
private slots:
    void save();
};

}
}

#endif // PLOTSETTINGSDIALOG_H
