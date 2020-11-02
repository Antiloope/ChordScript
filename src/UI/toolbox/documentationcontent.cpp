#include "documentationcontent.h"
#include <QPushButton>
#include "UI/uidefinitions.h"
#include <QStackedLayout>
#include <QLabel>
#include <QScrollBar>
#include <QTabWidget>

using namespace CS::UI;

DocumentationContent::DocumentationContent(QWidget *parent) :
    QFrame(parent) {

    _layout = new QBoxLayout(QBoxLayout::TopToBottom,this);
    setLayout(_layout);
    _layout->setContentsMargins(0,0,0,0);

    for(unsigned i = 0; i < NUMBER_OF_SECTIONS; i++)
    {
        _sections[i] = new QFrame(this);
        initSection(i);
        _layout->addWidget(_sections[i]);
    }
    _sections[_currentSection]->show();
    update();
}

void DocumentationContent::initSection(unsigned section) {
    UiDefinitions* def = UiDefinitions::getInstance();

    QString styles =
        "<style>"
            ".link {"
                "color: " + def->getColorRGB(ColorId::Primary) + ";"
                "text-decoration: none;"
                "font-weight: bold;"
            "}"
            ".cs {"
                "color: " + def->getColorRGB(ColorId::Darkest) + ";"
            "}"
            ".code {"
                "color: " + def->getColorRGB(ColorId::Darkest) + ";"
                "background-color: #eeeeee;"
                "font-family:monospace;"
                "border-radius: 5px;"
            "}"
            "li {"
                "margin-bottom: 15px;"
                "margin-top: 15px;"
            "}"
        "</style>";
    switch( section )
    {
    case 0:
    {
        QVBoxLayout* layout = new QVBoxLayout(_sections[section]);
        _sections[section]->setLayout(layout);
        layout->setContentsMargins(0,8,0,8);

        QScrollArea* intro = new QScrollArea(_sections[section]);

        QGridLayout* grid = new QGridLayout(intro);
        intro->setLayout(grid);

        QLabel* titleLbl = new QLabel("<h1>Introduction</h1>",intro);
        grid->addWidget(titleLbl,0,0,Qt::AlignJustify);

        QLabel* spaceLbl = new QLabel("<h6></h6>",intro);
        grid->addWidget(spaceLbl,1,0,Qt::AlignJustify);

        QLabel* introLbl = new QLabel(
            styles +
            "<p><b>Welcome to ChordScript!</b> This documentation is a detailed "
            "description of the main "
            "syntax and characteristics of the language with examples "
            "on each section. If you are new with <i class=\"cs\">ChordScript</i>, you should "
            "start with the <a class=\"link\" href=\"https://antiloope.github.io/ChordScriptPage/tutorial\">tutorial</a>. This documentation is useful for "
            "searching the specific description of some functionality.</p>",intro);

        introLbl->setOpenExternalLinks(true);
        introLbl->setWordWrap(true);
        introLbl->setStyleSheet(
            "QLabel {font-size:15px;}");
        grid->addWidget(introLbl,2,0,Qt::AlignTop);
        grid->setRowStretch(2,2);

        layout->addWidget(intro);
    }
        break;
    case 1:
    {
        QVBoxLayout* basicsSectionLayout = new QVBoxLayout(_sections[section]);
        _sections[section]->setLayout(basicsSectionLayout);

        basicsSectionLayout->setContentsMargins(0,8,0,8);

        QScrollArea* basicsContent = new QScrollArea(_sections[section]);

        QGridLayout* mainGrid = new QGridLayout(basicsContent);
        basicsContent->setLayout(mainGrid);
        mainGrid->setContentsMargins(0,0,0,0);

        QLabel* titleLbl = new QLabel("<h1>Language basics</h1>",basicsContent);
        mainGrid->addWidget(titleLbl,0,0,Qt::AlignJustify);

        QLabel* spaceLbl = new QLabel("<h6></h6>",basicsContent);
        mainGrid->addWidget(spaceLbl,1,0,Qt::AlignJustify);

        QTabWidget* chapters = new QTabWidget(basicsContent);
        mainGrid->addWidget(chapters,2,0,Qt::AlignJustify);

        chapters->setStyleSheet(
            "QTabBar::tab:!selected {"
                "background: "+ def->getColorRGB(ColorId::Light) + ";"
                "color: " + def->getColorRGB(ColorId::TextPrimary) + ";"
                "padding: 7px;"
                "border: 3px solid " + def->getColorRGB(ColorId::Dark) + ";"
            "}"
            "QTabWidget::tab-bar {"
                "alignment: center;"
            "}"
            "QTabWidget::pane {"
                "border-top: 2px solid " + def->getColorRGB(ColorId::Dark) + ";"
                "position: absolute;"
                "top: -0.8em;"
                "padding-top: 0.8em;"
            "}"
            "QTabBar::tab:selected {"
                "padding: 10px;"
                "background: "+ def->getColorRGB(ColorId::Dark) + ";"
                "color: " + def->getColorRGB(ColorId::Lightest) + ";"
            "}"
            );

        QFrame* chapter1 = new QFrame(chapters);
        QFrame* chapter2 = new QFrame(chapters);
        QFrame* chapter3 = new QFrame(chapters);

        QGridLayout* ch1Grid = new QGridLayout(chapter1);
        chapter1->setLayout(ch1Grid);

        QGridLayout* ch2Grid = new QGridLayout(chapter2);
        chapter2->setLayout(ch2Grid);

        QGridLayout* ch3Grid = new QGridLayout(chapter3);
        chapter3->setLayout(ch3Grid);

        chapters->addTab(chapter1,"What CS is?");
        chapters->addTab(chapter2,"General syntax");
        chapters->addTab(chapter3,"Demo examples");

        QLabel* ch1TitleLbl = new QLabel("<h2>What CS is?</h2>",chapter1);
        ch1Grid->addWidget(ch1TitleLbl,0,0,Qt::AlignLeft);

        QLabel* ch1ContentLbl = new QLabel(
            styles +
            "<p><i class=\"cs\">ChordScript</i> is a <a class=\"link\" "
            "href=\"https://en.wikipedia.org/wiki/Dynamic_programming_language\">dynamic</a>, "
            "<a class=\"link\" href=\"https://en.wikipedia.org/wiki/Strong_and_weak_typing\">strongly typed</a> "
            "and <a class=\"link\" href=\"https://en.wikipedia.org/wiki/Interpreted_language\">interpreted</a> "
            "language</b>, that executes code to generate sounds. In order "
            "to execute sounds on different operative systems, it needs "
            "to \"connect\" with different sound systems and drivers.</p>"
            "<p>To simplify this communication, <i class=\"cs\">ChordScript</i> "
            "uses <a class=\"link\" href=\"https://jackaudio.org/\">Jack "
            "Audio Connection Kit</a>. It works as a server mounted over "
            "current OS sound diver, and <i class=\"cs\">ChordScript</i> "
            "connects to it "
            "as a client. For this reason, when you install CS you "
            "also install Jack. This architecture, does not only makes "
            "<i class=\"cs\">ChordScript</i> able to run on different OS, "
            "but also offers the possibility to work with "
            "<a class=\"link\" href=\"https://en.wikipedia.org/wiki/Latency_(audio)\">low latency</a> "
            "audio generation.</p>"
            "<p>So, <i class=\"cs\">CS</i> allows you to trigger pre-recorded "
            "samples, "
            "synthesize sounds using basic functions and apply some "
            "effects on this sounds, using programming as interface.</p>"
            "<p>For this reasons, <i class=\"cs\">ChordScript</i> "
            "can be a good alternative "
            "for <a class=\"link\" href=\"https://en.wikipedia.org/wiki/Live_coding\">live coding</a>, "
            "<a class=\"link\" href=\"https://en.wikipedia.org/wiki/Creative_coding\">creative coding</a>, "
            "<a class=\"link\" href=\"https://en.wikipedia.org/wiki/Algorithmic_composition\">algorithmic composition</a> "
            "or for any musician/programmer who wants to use programming "
            "as a powerful instrument for composition, giving also the "
            "possibility to record and store it as a .wav file.</p>",chapter1);

        ch1ContentLbl->setWordWrap(true);
        ch1ContentLbl->setStyleSheet(
            "QLabel {font-size:15px;}");
        ch1Grid->addWidget(ch1ContentLbl,1,0);
        ch1Grid->setRowStretch(1,2);

        QLabel* ch2TitleLbl = new QLabel("<h2>General Syntax</h2>",chapter2);
        ch2Grid->addWidget(ch2TitleLbl,0,0,Qt::AlignLeft);

        QLabel* ch2ContentLbl = new QLabel(
            styles +
            "<p><i class=\"cs\">ChordScript</i> takes some syntax rules "
            "from other popular "
            "languages. This decision was made to take it easy to learn "
            "other languages if you start your programming path here. "
            "It takes the strong rules of "
            "<a class=\"link\" href=\"https://en.wikipedia.org/wiki/C_(programming_language)\">C/C++</a> "
            "and the dynamics of "
            "<a class=\"link\" href=\"https://en.wikipedia.org/wiki/Python_(programming_language)\">Python</a>.</p>"
            "<p>Here is a list with the basic rules:</p>"
            "<ul>"
                "<li><b>The type of a variable must be specified in assignation:</b></li>"
                "<span class=\"code\">number myVariable = 10;</span>"
            "</ul>"
            "<p>See data types section for a list of all valid data types</p>"
            "<ul>"
                "<li><b>Assignations and executions lines, must end with a semicolon:</b></li>"
                "<span class=\"code\">string name = \"my name\";</span>"
                "<li><b>Variable names must start with a letter and can continue with more letters, numbers or the character underscore '_' :</b></li>"
                "<span class=\"code\"> MyVariable_3 </span> Valid name.<br>"
                "<span class=\"code\"> 4_aVariable </span> Invalid name."
            "</ul>",chapter2);

        ch2ContentLbl->setWordWrap(true);
        ch2ContentLbl->setStyleSheet(
            "QLabel {font-size:15px;}");
        ch2Grid->addWidget(ch2ContentLbl,1,0,Qt::AlignTop);
        ch2Grid->setRowStretch(1,2);

        QLabel* ch3TitleLbl = new QLabel("<h2>Demo Examples</h2>",chapter3);
        ch3Grid->addWidget(ch3TitleLbl,0,0,Qt::AlignLeft);

        QLabel* ch3ContentLbl = new QLabel(
            styles +
                "<p>This demo, shows a lot of capabilities of the language. "
                "In order to read simpler examples continue to specific sections.</p>"
                "<h4>Base with simple chords</h4>"
            ,chapter3);

        ch3ContentLbl->setWordWrap(true);
        ch3ContentLbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;}");
        ch3Grid->addWidget(ch3ContentLbl,1,0,Qt::AlignTop);

        QPushButton* ch3Demo1 = new QPushButton("Open demo",chapter3);
        ch3Demo1->setObjectName("1");
        ch3Grid->addWidget(ch3Demo1,2,0,Qt::AlignTop);

        connect(ch3Demo1,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* ch3Content2Lbl = new QLabel(
            styles +
                "<h4>Synthesize using loops</h4>"
            ,chapter3);

        ch3Content2Lbl->setWordWrap(true);
        ch3Content2Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;margin-top:15px;}");
        ch3Grid->addWidget(ch3Content2Lbl,3,0,Qt::AlignTop);

        QPushButton* ch3Demo2 = new QPushButton("Open demo",chapter3);
        ch3Demo2->setObjectName("2");
        ch3Grid->addWidget(ch3Demo2,4,0,Qt::AlignTop);

        connect(ch3Demo2,SIGNAL(clicked()),this,SLOT(showDemo()));

        ch3Grid->setRowStretch(4,2);

        basicsSectionLayout->addWidget(basicsContent);
        break;
    }
    case 2:

        break;
    case 3:

        break;
    case 4:

        break;
    case 5:

        break;
    default:
        return;
        break;
    }
    _sections[section]->hide();
}

void DocumentationContent::showDemo() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    int btn = button->objectName().toInt();
    switch( btn )
    {
    case 1:
    {
        QString code =
            "sound mySound = S_SIN * 0.3;\n\n"

            "number quarter = 2; # in seconds\n"
            "number eighth = quarter / 2; # in seconds\n\n"

            "array scale = [C5,D5,E5,F5,G5,A5,B5,C6,D6];\n\n"

            "# Definition of some chords\n"
            "array c = [C5,E5,G5];\n"
            "array a = [C5,E5,A5];\n"
            "# Another chord but defined using the scale\n"
            "array d = [scale.at(0),scale.at(3),scale.at(5)];\n\n"

            "argument song = (); # This argument holds all the song chords\n\n"

            "song.push((c,quarter));\n"
            "song.push((a,quarter));\n"
            "song.push((d,quarter));\n"
            "song.push((c,quarter));\n\n"

            "mySound.play(\n"
            "\t song\n"
            ");\n\n"

            "# Define a lead sound\n"
            "sound myLead = S_SQUARE.freqModulation(S_SIN(5) * 0.7);\n\n"

            "argument melody = ();\n\n"

            "# Play a scale\n"
            "for(number i = 0; i < 8; i++) {\n"
            "\t melody.push(([scale.at(i)],eighth));\n"
            "}\n\n"

            "myLead.play(melody);";
        emit demo(code);
    }
        break;
    case 2:
    {
        QString code =
            "sound mySound = S_SIN * S_SIN.freqModulation(S_SIN(4)) * S_SIN.freqFactor(2);\n\n"

            "mySound.play([A4]);\n\n"

            "mySound.loop(\n"
            "\t([D5],1),\n"
            "\t([Ds5],0.4)\n"
            ");\n\n"

            "mySound.loop(\n"
            "\t([0],0.9),\n"
            "\t([Cs5],0.4)\n"
            ");";
        emit demo(code);
    }
        break;
    default:
        break;
    }
}

void DocumentationContent::display() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    _sections[_currentSection]->hide();
    int section = button->text().split(".")[0].toInt() - 1;
    _sections[section]->show();
    update();
    _currentSection = section;
}
