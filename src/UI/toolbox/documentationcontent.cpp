#include "documentationcontent.h"
#include <QPushButton>
#include "UI/uidefinitions.h"
#include <QStackedLayout>
#include <QLabel>
#include <QScrollBar>
#include <QTabWidget>

using namespace CS::UI;

namespace  {

const QString DEMO_1 = "1";
const QString DEMO_2 = "2";
const QString ARGUMENT_DEMO = "3";
const QString ARGUMENT_PUSH_DEMO = "4";
const QString ARGUMENT_POP_DEMO = "5";
const QString ARRAY_DEMO = "6";
const QString ARRAY_SIZE_DEMO = "7";
const QString ARRAY_AT_DEMO = "8";
const QString ARRAY_PUSH_DEMO = "9";
const QString ARRAY_INSERT_DEMO = "10";
const QString ARRAY_REMOVE_DEMO = "11";
const QString ARRAY_POP_DEMO = "12";
const QString SOUND_PLAY_DEMO = "13";
const QString SOUND_PLAY2_DEMO = "14";
const QString SOUND_STOP_DEMO = "15";
const QString SOUND_LOOP_DEMO = "16";
const QString SOUND_SET_PANNING_DEMO = "17";
const QString SOUND_SET_PANNING_DEMO2 = "18";
const QString SOUND_CONSTANT_FREQ_DEMO = "19";
const QString SOUND_CONSTANT_FREQ_DEMO2 = "20";
const QString SOUND_FREQ_FACTOR_DEMO = "21";
const QString SOUND_FREQ_MODULATION_DEMO = "22";
const QString SOUND_AMP_FACTOR_DEMO = "23";
const QString SOUND_AMP_FACTOR_DEMO2 = "24";
const QString SAMPLE_PLAY_DEMO = "25";
const QString SAMPLE_PLAY_DEMO2 = "26";
const QString SAMPLE_STOP_DEMO = "27";
const QString SAMPLE_LOOP_DEMO = "28";
const QString SAMPLE_SET_PANNING_DEMO = "29";
const QString SAMPLE_SET_PANNING_DEMO2 = "30";
const QString IF_DEMO = "31";
const QString IF_DEMO2 = "32";
const QString FOR_DEMO = "33";
const QString FUNCTIONS_DEMO = "34";

}

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
        ch1ContentLbl->setOpenExternalLinks(true);
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
        ch2ContentLbl->setOpenExternalLinks(true);
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
        ch3Demo1->setObjectName(DEMO_1);
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
        ch3Demo2->setObjectName(DEMO_2);
        ch3Grid->addWidget(ch3Demo2,4,0,Qt::AlignTop);

        connect(ch3Demo2,SIGNAL(clicked()),this,SLOT(showDemo()));

        ch3Grid->setRowStretch(4,2);

        basicsSectionLayout->addWidget(basicsContent);
    }
    break;
    case 2:
    {
        QVBoxLayout* dataTypesSectionLayout = new QVBoxLayout(_sections[section]);
        _sections[section]->setLayout(dataTypesSectionLayout);

        dataTypesSectionLayout->setContentsMargins(0,8,0,8);

        QScrollArea* dataTypesContent = new QScrollArea(_sections[section]);

        QGridLayout* mainGrid = new QGridLayout(dataTypesContent);
        dataTypesContent->setLayout(mainGrid);
        mainGrid->setContentsMargins(0,0,0,0);

        QLabel* titleLbl = new QLabel("<h1>Data types and methods</h1>",dataTypesContent);
        mainGrid->addWidget(titleLbl,0,0,Qt::AlignJustify);

        QLabel* spaceLbl = new QLabel("<h6></h6>",dataTypesContent);
        mainGrid->addWidget(spaceLbl,1,0,Qt::AlignJustify);

        QTabWidget* chapters = new QTabWidget(dataTypesContent);
        mainGrid->addWidget(chapters,2,0);

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
                "border-top: 0px solid white;"
                "position: absolute;"
                "padding-top: 0.2em;"
            "}"
            "QTabBar::tab:selected {"
                "padding: 10px;"
                "background: "+ def->getColorRGB(ColorId::Dark) + ";"
                "color: " + def->getColorRGB(ColorId::Lightest) + ";"
            "}"
            );

        QFrame* basicTypes = new QFrame(chapters);
        QFrame* playables = new QFrame(chapters);

        QGridLayout* basicTypesGrid = new QGridLayout(basicTypes);
        basicTypes->setLayout(basicTypesGrid);
        basicTypesGrid->setContentsMargins(0,0,0,0);

        QGridLayout* playablesGrid = new QGridLayout(playables);
        playables->setLayout(playablesGrid);
        playablesGrid->setContentsMargins(0,0,0,0);

        chapters->addTab(basicTypes,"Basic types");
        chapters->addTab(playables,"Playables");

        QTabWidget* basicTypesTabs = new QTabWidget(basicTypes);
        basicTypesGrid->addWidget(basicTypesTabs,0,0);

        basicTypesTabs->setStyleSheet(
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

        QFrame* numberChapter = new QFrame(basicTypesTabs);
        QFrame* booleanChapter = new QFrame(basicTypesTabs);
        QFrame* stringChapter = new QFrame(basicTypesTabs);
        QFrame* argumentChapter = new QFrame(basicTypesTabs);
        QFrame* arrayChapter = new QFrame(basicTypesTabs);

        QGridLayout* numberGrid = new QGridLayout(numberChapter);
        numberChapter->setLayout(numberGrid);

        QGridLayout* booleanGrid = new QGridLayout(booleanChapter);
        booleanChapter->setLayout(booleanGrid);

        QGridLayout* stringGrid = new QGridLayout(stringChapter);
        stringChapter->setLayout(stringGrid);

        QGridLayout* argumentGrid = new QGridLayout(argumentChapter);
        argumentChapter->setLayout(argumentGrid);

        QGridLayout* arrayGrid = new QGridLayout(arrayChapter);
        arrayChapter->setLayout(arrayGrid);

        basicTypesTabs->addTab(numberChapter,"Number");
        basicTypesTabs->addTab(booleanChapter,"Boolean");
        basicTypesTabs->addTab(stringChapter,"String");
        basicTypesTabs->addTab(argumentChapter,"Argument");
        basicTypesTabs->addTab(arrayChapter,"Array");

        QLabel* numberTitleLbl = new QLabel("<h2>number</h2>",numberChapter);
        numberGrid->addWidget(numberTitleLbl,0,0,Qt::AlignLeft);

        QLabel* numberContentLbl = new QLabel(
            styles +
                "<p>In the code, numbers are simply any kind of number, "
                "with or without decimal part. Some examples of numbers "
                "could be <span class=\"code\">number myNumber = 1;"
                "</span> or <span class=\"code\">number mySecondNumber = "
                "4.53;</span></p>"
                "<p>Only a dot can be used to separate decimal part, "
                "comma it's not allowed. Valid operations with numbers are "
                "addition <span class=\"code\">+</span>, subtraction <span "
                "class=\"code\">-</span>, multiplication <span class=\"code\">"
                "*</span> and division "
                "<span class=\"code\">/</span> Additionally you can call the "
                "operator <span class=\"code\">++</span> and <span class=\"code\">"
                "--</span> to "
                "add/subtract one and assign the number again to the same "
                "variable."
                "</p>",numberChapter);

        numberContentLbl->setWordWrap(true);
        numberContentLbl->setStyleSheet(
            "QLabel {font-size:15px;}");
        numberGrid->addWidget(numberContentLbl,1,0,Qt::AlignTop);
        numberGrid->setRowStretch(1,2);

        QLabel* booleanTitleLbl = new QLabel("<h2>boolean</h2>",booleanChapter);
        booleanGrid->addWidget(booleanTitleLbl,0,0,Qt::AlignLeft);

        QLabel* booleanContentLbl = new QLabel(
            styles +
                "<p>Boolean values can be expressed as <span class=\"code\">true</span> or <span class=\"code\">false</span>.</p>"
                "<p>It can also be expressed as numbers. A false value is 0 and any other number is considered true.</p>"
                "<p>Valid boolean operations are:</p>"
                "<ul>"
                    "<li><span class=\"code\">==</span> compare if two values are equal and return a boolean as a result of the comparison.</li>"
                    "<li><span class=\"code\">&#60;</span> compare if the right values is bigger than the left value.</li>"
                    "<li><span class=\"code\">></span> compare if the left values is bigger than the right value.</li>"
                    "<li><span class=\"code\">>=</span> compare if the left values is bigger or equal the right value.</li>"
                    "<li><span class=\"code\">&#60;=</span> compare if the left values is smaller or equal the right value.</li>"
                    "<li><span class=\"code\">!=</span> compare if both values are different.</li>"
                    "<li><span class=\"code\">&</span> works as the AND boolean operation. If both values are true, so the returned value is true. Otherwise return false.</li>"
                    "<li><span class=\"code\">|</span> works as the OR boolean operation. If both values are false, return false, otherwise return true.</li>"
                    "<li><span class=\"code\">!</span> this operator placed before a boolean value, works as the NOT boolean operation.</li>"
                "</ul>",booleanChapter);

        booleanContentLbl->setWordWrap(true);
        booleanContentLbl->setStyleSheet(
            "QLabel {font-size:15px;}");
        booleanGrid->addWidget(booleanContentLbl,1,0,Qt::AlignTop);
        booleanGrid->setRowStretch(1,2);

        QLabel* stringTitleLbl = new QLabel("<h2>string</h2>",stringChapter);
        stringGrid->addWidget(stringTitleLbl,0,0,Qt::AlignLeft);

        QLabel* stringContentLbl = new QLabel(
            styles +
                "<p>Strings are text placed between double quotes "
                "<span class=\"code\">\"I am a string!\"</span>. "
                "This is the way to introduce "
                "text inside ChordScript. All text between quotes "
                "aren't interpreted as code. So, can be used for "
                "naming an object or to specify the name of a file "
                "in the computer.</p>"
                "<p>Some functions takes a string as an argument, "
                "for example: <span class=\"code\">sample mySample = "
                "SAMPLE(\"path/to/my/file.wav\");</span></p>"
                "<p>Strings can be concatenated using the operator "
                "<span class=\"code\">+</span></p>",stringChapter);

        stringContentLbl->setWordWrap(true);
        stringContentLbl->setStyleSheet(
            "QLabel {font-size:15px;}");
        stringGrid->addWidget(stringContentLbl,1,0,Qt::AlignTop);
        stringGrid->setRowStretch(1,2);

        QLabel* argumentTitleLbl = new QLabel("<h2>argument</h2>",argumentChapter);
        argumentGrid->addWidget(argumentTitleLbl,0,0,Qt::AlignLeft);

        QLabel* argumentContentLbl = new QLabel(
            styles +
                "<p>Argument is an interesting data type, that "
                "can hold values to be passed later to a function. "
                "An example could be the better explanation:</p>"
            ,argumentChapter);

        argumentContentLbl->setWordWrap(true);
        argumentContentLbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;}");
        argumentGrid->addWidget(argumentContentLbl,1,0,Qt::AlignTop);

        QPushButton* argumentDemo = new QPushButton("Argument demo",argumentChapter);
        argumentDemo->setObjectName(ARGUMENT_DEMO);
        argumentGrid->addWidget(argumentDemo,2,0,Qt::AlignTop);

        connect(argumentDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* argumentContent2Lbl = new QLabel(
            styles +
                "<p>So, this allows you to set argument values before "
                "calling the function.</p>"
                "<h4>push</h4>"
                "<ul>"
                    "<li><span class=\"code\">push(argument)</span></li>"
                "</ul>"
                "<p>Arguments works as a "
                "<a href=\"https://en.wikipedia.org/wiki/Stack_(abstract_data_type)\" "
                "class=\"link\">stack</a>. So, it has push and pop methods. "
                "The value passed as argument, is attached as the last "
                "element in the argument stack.</p>"
            ,argumentChapter);

        argumentContent2Lbl->setOpenExternalLinks(true);
        argumentContent2Lbl->setWordWrap(true);
        argumentContent2Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        argumentGrid->addWidget(argumentContent2Lbl,3,0,Qt::AlignTop);

        QPushButton* argumentPushDemo = new QPushButton("Push demo",argumentChapter);
        argumentPushDemo->setObjectName(ARGUMENT_PUSH_DEMO);
        argumentGrid->addWidget(argumentPushDemo,4,0,Qt::AlignTop);

        connect(argumentPushDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* argumentContent3Lbl = new QLabel(
            styles +
                "<h4>pop</h4>"
                "<ul>"
                    "<li><span class=\"code\">pop()</span></li>"
                "</ul>"
                "<p>Arguments works as a "
                "<a href=\"https://en.wikipedia.org/wiki/Stack_(abstract_data_type)\" "
                "class=\"link\">stack</a>. So, it has push and pop methods. "
                "This method does not take arguments and it "
                "removes the last element of the argument stack.</p>"
            ,argumentChapter);

        argumentContent3Lbl->setOpenExternalLinks(true);
        argumentContent3Lbl->setWordWrap(true);
        argumentContent3Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        argumentGrid->addWidget(argumentContent3Lbl,5,0,Qt::AlignTop);

        QPushButton* argumentPopDemo = new QPushButton("Pop demo",argumentChapter);
        argumentPopDemo->setObjectName(ARGUMENT_POP_DEMO);
        argumentGrid->addWidget(argumentPopDemo,6,0,Qt::AlignTop);

        connect(argumentPopDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        argumentGrid->setRowStretch(6,2);

        QLabel* arrayTitleLbl = new QLabel("<h2>array</h2>",arrayChapter);
        arrayGrid->addWidget(arrayTitleLbl,0,0,Qt::AlignLeft);

        QLabel* arrayContentLbl = new QLabel(
            styles +
                "<p>Arrays are elements between brackets separated by "
                "commas. It is a way to store things that can be "
                "accessed and removed later. To modify the array, "
                "it has some methods detailed below. It works as a "
                "<a href=\"https://en.wikipedia.org/wiki/Linked_list\" "
                "class=\"link\">linked list</a> but with some "
                "'overloaded' methods like insertion/deletion in a "
                "particular position.</p>"
                "<p>Here is a definition example:</p>"
            ,arrayChapter);

        arrayContentLbl->setOpenExternalLinks(true);
        arrayContentLbl->setWordWrap(true);
        arrayContentLbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;}");
        arrayGrid->addWidget(arrayContentLbl,1,0,Qt::AlignTop);

        QPushButton* arrayDemo = new QPushButton("Array demo",arrayChapter);
        arrayDemo->setObjectName(ARRAY_DEMO);
        arrayGrid->addWidget(arrayDemo,2,0,Qt::AlignTop);

        connect(arrayDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* arrayContent2Lbl = new QLabel(
            styles +
                "<p>If you insert a variable inside an array, a copy "
                "of the value is inserted. So, you can change the "
                "variable value later, but the value in the array will "
                "still being the same.</p>"
                "<h4>size</h4>"
                "<ul>"
                "<li><span class=\"code\">size()</span></li>"
                "</ul>"
                "<p>This method returns the number of elements "
                "currently stored in the array.</p>"
                "<p>Here is an example usage:</p>"
            ,arrayChapter);

        arrayContent2Lbl->setOpenExternalLinks(true);
        arrayContent2Lbl->setWordWrap(true);
        arrayContent2Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        arrayGrid->addWidget(arrayContent2Lbl,3,0,Qt::AlignTop);

        QPushButton* arraySizeDemo = new QPushButton("Size demo",arrayChapter);
        arraySizeDemo->setObjectName(ARRAY_SIZE_DEMO);
        arrayGrid->addWidget(arraySizeDemo,4,0,Qt::AlignTop);

        connect(arraySizeDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* arrayContent3Lbl = new QLabel(
            styles +
                "<h4>at</h4>"
                "<ul>"
                "<li><span class=\"code\">at(number)</span></li>"
                "</ul>"
                "<p>It takes an integer greater or equal than 0 "
                "as argument and returns a copy of the element stored "
                "in that position of the array. So you can modify the "
                "returned element without changing the stored value. "
                "Positions in an array starts at index 0. So, if an "
                "array holds 3 elements, the first element is stored "
                "in the position 0 and the last one is stored in the "
                "position 2.</p>"
                "<p>Here is an example usage:</p>"
            ,arrayChapter);

        arrayContent3Lbl->setOpenExternalLinks(true);
        arrayContent3Lbl->setWordWrap(true);
        arrayContent3Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        arrayGrid->addWidget(arrayContent3Lbl,5,0,Qt::AlignTop);

        QPushButton* arrayAtDemo = new QPushButton("At demo",arrayChapter);
        arrayAtDemo->setObjectName(ARRAY_AT_DEMO);
        arrayGrid->addWidget(arrayAtDemo,6,0,Qt::AlignTop);

        connect(arrayAtDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* arrayContent4Lbl = new QLabel(
            styles +
                "<h4>push</h4>"
                "<ul>"
                "<li><span class=\"code\">push(element)</span></li>"
                "</ul>"
                "<p>This method insert a copy of the element passed "
                "as argument in the last position of the array, "
                "increasing the size of the array by one.</p>"
                "<p>Here is an example usage:</p>"
            ,arrayChapter);

        arrayContent4Lbl->setWordWrap(true);
        arrayContent4Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        arrayGrid->addWidget(arrayContent4Lbl,7,0,Qt::AlignTop);

        QPushButton* arrayPushDemo = new QPushButton("Push demo",arrayChapter);
        arrayPushDemo->setObjectName(ARRAY_PUSH_DEMO);
        arrayGrid->addWidget(arrayPushDemo,8,0,Qt::AlignTop);

        connect(arrayPushDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* arrayContent5Lbl = new QLabel(
            styles +
                "<h4>insert</h4>"
                "<ul>"
                "<li><span class=\"code\">insert(number, element)</span></li>"
                "</ul>"
                "<p>This method receives two parameters, a position to store "
                "the value and the element to store in that position. "
                "The number must be greater or equal than 0 because it "
                "represents a position of the array. Elements after the "
                "position are moved, in order to insert the element in "
                "that position. Array size is increased by one.</p>"
                "<p>A copy of the value are inserted, so you can modify "
                "the value of the element later and the value inside "
                "the array still being the same.</p>"
                "<p>Here is an example usage:</p>"
            ,arrayChapter);

        arrayContent5Lbl->setWordWrap(true);
        arrayContent5Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        arrayGrid->addWidget(arrayContent5Lbl,9,0,Qt::AlignTop);

        QPushButton* arrayInsertDemo = new QPushButton("Insert demo",arrayChapter);
        arrayInsertDemo->setObjectName(ARRAY_INSERT_DEMO);
        arrayGrid->addWidget(arrayInsertDemo,10,0,Qt::AlignTop);

        connect(arrayInsertDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* arrayContent6Lbl = new QLabel(
            styles +
                "<h4>remove</h4>"
                "<ul>"
                "<li><span class=\"code\">remove(number)</span></li>"
                "</ul>"
                "<p>This method removes an element of the array, so "
                "array size is reduced by one. The number received as "
                "parameter, must be greater or equal than 0 because it "
                "represents a position in the array.</p>"
                "<p>Here is an example usage:</p>"
            ,arrayChapter);

        arrayContent6Lbl->setWordWrap(true);
        arrayContent6Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        arrayGrid->addWidget(arrayContent6Lbl,11,0,Qt::AlignTop);

        QPushButton* arrayRemoveDemo = new QPushButton("Remove demo",arrayChapter);
        arrayRemoveDemo->setObjectName(ARRAY_REMOVE_DEMO);
        arrayGrid->addWidget(arrayRemoveDemo,12,0,Qt::AlignTop);

        connect(arrayRemoveDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* arrayContent7Lbl = new QLabel(
            styles +
                "<h4>pop</h4>"
                "<ul>"
                "<li><span class=\"code\">pop()</span></li>"
                "</ul>"
                "<p>This method removes the last element of the "
                "array, so array size is reduced by one.</p>"
                "<p>Here is an example usage:</p>"
            ,arrayChapter);

        arrayContent7Lbl->setWordWrap(true);
        arrayContent7Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        arrayGrid->addWidget(arrayContent7Lbl,13,0,Qt::AlignTop);

        QPushButton* arrayPopDemo = new QPushButton("Pop demo",arrayChapter);
        arrayPopDemo->setObjectName(ARRAY_POP_DEMO);
        arrayGrid->addWidget(arrayPopDemo,14,0,Qt::AlignTop);

        connect(arrayPopDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        arrayGrid->setRowStretch(14,2);

        QTabWidget* playablesTabs = new QTabWidget(playables);
        playablesGrid->addWidget(playablesTabs,0,0);

        playablesTabs->setStyleSheet(
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

        QFrame* soundChapter = new QFrame(playablesTabs);
        QFrame* sampleChapter = new QFrame(playablesTabs);

        QGridLayout* soundGrid = new QGridLayout(soundChapter);
        soundChapter->setLayout(soundGrid);

        QGridLayout* sampleGrid = new QGridLayout(sampleChapter);
        sampleChapter->setLayout(sampleGrid);

        playablesTabs->addTab(soundChapter,"Sound");
        playablesTabs->addTab(sampleChapter,"Sample");

        QLabel* soundTitleLbl = new QLabel("<h2>sound</h2>",soundChapter);
        soundGrid->addWidget(soundTitleLbl,0,0,Qt::AlignLeft);

        QLabel* soundContentLbl = new QLabel(
            styles +
                "<p>This data types allows you to synthesize sounds. "
                "This type has the following methods and operations:</p>"
                "<h4>play</h4>"
                "<ul>"
                    "<li><span class=\"code\">play( [array] , number = 0)</span></li>"
                "</ul>"
                "<p>As its name says, this method play the sound "
                "during a time in seconds specified in the second "
                "number parameter. If you call this method without a "
                "duration, the sounds play forever (until you stop "
                "it).</p>"
                "<p>The first parameter is an array of frequencies "
                "of type <span class=\"code\">number</span> between "
                "brackets and separated by "
                "commas. All this frequencies are passed to the sound "
                "at the same time, so this is the way to play chords "
                "for example.</p>"
            ,soundChapter);

        soundContentLbl->setWordWrap(true);
        soundContentLbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContentLbl,1,0,Qt::AlignTop);

        QPushButton* soundPlayDemo = new QPushButton("Play demo",soundChapter);
        soundPlayDemo->setObjectName(SOUND_PLAY_DEMO);
        soundGrid->addWidget(soundPlayDemo,2,0,Qt::AlignTop);

        connect(soundPlayDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent2Lbl = new QLabel(
            styles +
                "<ul>"
                    "<li><span class=\"code\">play( (arguments) )</span></li>"
                "</ul>"
                "<p>This is another way to call the play method. It "
                "consist of arguments similar to the previous play "
                "definition passed as the same time to play one after "
                "the other. This is the way to play a song for "
                "example.</p>"
            ,soundChapter);

        soundContent2Lbl->setWordWrap(true);
        soundContent2Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent2Lbl,3,0,Qt::AlignTop);

        QPushButton* soundPlay2Demo = new QPushButton("Play demo",soundChapter);
        soundPlay2Demo->setObjectName(SOUND_PLAY2_DEMO);
        soundGrid->addWidget(soundPlay2Demo,4,0,Qt::AlignTop);

        connect(soundPlay2Demo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent3Lbl = new QLabel(
            styles +
                "<p><b>Important</b>: you can't call this method as the "
                "second way and giving no duration on any argument. "
                "It throws a syntax error.</p>"
                "<h4>stop</h4>"
                "<ul>"
                "<li><span class=\"code\">stop()</span></li>"
                "</ul>"
                "<p>This method doesn't receive any argument. It just "
                "stops all generated sounds that are playing of a "
                "particular <span class=\"code\">sound</span>.</p>"
            ,soundChapter);

        soundContent3Lbl->setWordWrap(true);
        soundContent3Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent3Lbl,5,0,Qt::AlignTop);

        QPushButton* soundStopDemo = new QPushButton("Stop demo",soundChapter);
        soundStopDemo->setObjectName(SOUND_STOP_DEMO);
        soundGrid->addWidget(soundStopDemo,6,0,Qt::AlignTop);

        connect(soundStopDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent4Lbl = new QLabel(
            styles +
                "<p><b>Note</b>: if you copy and paste this code "
                "and run, you won't hear anything because you are "
                "playing and stopping the sound immediately after "
                "that. This method is useful when live coding, and "
                "you want to stop some sounds that are currently "
                "playing.</p>"
                "<h4>loop</h4>"
                "<ul>"
                "<li><span class=\"code\">loop( [array] , number ), loop( (arguments) )</span></li>"
                "</ul>"
                "<p>This method is similar to <span class=\"code\">"
                "play</span> but with the difference that when all "
                "arguments are played, starts again infinitely.</p>"
            ,soundChapter);

        soundContent4Lbl->setWordWrap(true);
        soundContent4Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent4Lbl,7,0,Qt::AlignTop);

        QPushButton* soundLoopDemo = new QPushButton("Loop demo",soundChapter);
        soundLoopDemo->setObjectName(SOUND_LOOP_DEMO);
        soundGrid->addWidget(soundLoopDemo,8,0,Qt::AlignTop);

        connect(soundLoopDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent5Lbl = new QLabel(
            styles +
                "<p>This 3 chords will play in a loop indefinitely "
                "until you stop the sound.</p>"
                "<h4>setPanning</h4>"
                "<ul>"
                "<li><span class=\"code\">setPanning( number )</span></li>"
                "</ul>"
                "<p>This method receives a number between 0 and 1 "
                "representing the panning of the sound. 1 is the "
                "stereo right channel and 0 the left one. So, 0.5 is "
                "the default centered sound.</p>"
                "<p>This method returns a new sound with the same "
                "characteristics of the current one but with the new "
                "panning. To understand how to use it, go to Synthesis "
                "section</p>"
            ,soundChapter);

        soundContent5Lbl->setWordWrap(true);
        soundContent5Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent5Lbl,9,0,Qt::AlignTop);

        QPushButton* soundSetPanningDemo = new QPushButton("Set Panning demo",soundChapter);
        soundSetPanningDemo->setObjectName(SOUND_SET_PANNING_DEMO);
        soundGrid->addWidget(soundSetPanningDemo,10,0,Qt::AlignTop);

        connect(soundSetPanningDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent6Lbl = new QLabel(
            styles +
                "<ul>"
                "<li><span class=\"code\">setPanning( sound )</span></li>"
                "</ul>"
                "<p>Optionally you can call this method but giving "
                "another sound as parameter. This generates a panning "
                "effect based on the sound passed as argument.</p>"
                "<p>This method returns a new sound with the same "
                "characteristics of the current one but with the new "
                "panning. To understand how to use it, go to Synthesis "
                "section.</p>"
            ,soundChapter);

        soundContent6Lbl->setWordWrap(true);
        soundContent6Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent6Lbl,11,0,Qt::AlignTop);

        QPushButton* soundSetPanningDemo2 = new QPushButton("Set Panning demo",soundChapter);
        soundSetPanningDemo2->setObjectName(SOUND_SET_PANNING_DEMO2);
        soundGrid->addWidget(soundSetPanningDemo2,12,0,Qt::AlignTop);

        connect(soundSetPanningDemo2,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent7Lbl = new QLabel(
            styles +
                "<h4>constantFreq</h4>"
                "<ul>"
                "<li><span class=\"code\">constantFreq( number )</span></li>"
                "</ul>"
                "<p>This method receives a constant frequency number as "
                "parameter and returns a new sound with the same characteristics "
                "of the current sound, but with a constant frequency. This "
                "means that no matter the frequencies you sent when you call "
                "play or loop method, it always play the same note.</p>"
                "<p>This is useful for synthesis. See Synthesis section for "
                "more interesting examples.</p>"
            ,soundChapter);

        soundContent7Lbl->setWordWrap(true);
        soundContent7Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent7Lbl,13,0,Qt::AlignTop);

        QPushButton* soundConstantFreqDemo = new QPushButton("ConstantFreq demo",soundChapter);
        soundConstantFreqDemo->setObjectName(SOUND_CONSTANT_FREQ_DEMO);
        soundGrid->addWidget(soundConstantFreqDemo,14,0,Qt::AlignTop);

        connect(soundConstantFreqDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent8Lbl = new QLabel(
            styles +
                "<p>Additionally you can call this method as an "
                "anonymous method with the following syntax:</p>"
            ,soundChapter);

        soundContent8Lbl->setWordWrap(true);
        soundContent8Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent8Lbl,15,0,Qt::AlignTop);

        QPushButton* soundConstantFreqDemo2 = new QPushButton("ConstantFreq demo",soundChapter);
        soundConstantFreqDemo2->setObjectName(SOUND_CONSTANT_FREQ_DEMO2);
        soundGrid->addWidget(soundConstantFreqDemo2,16,0,Qt::AlignTop);

        connect(soundConstantFreqDemo2,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent9Lbl = new QLabel(
            styles +
                "<h4>freqFactor</h4>"
                "<ul>"
                "<li><span class=\"code\">freqFactor( number )</span></li>"
                "</ul>"
                "<p>This method set a factor number multiplying "
                "the frequency of the sound. Receives a number "
                "representing this factor, and returns a new sound "
                "with this factor set. To see more examples, see "
                "Synthesis section</p>"
                "<p>It can be used to octave a sound for example.</p>"
            ,soundChapter);

        soundContent9Lbl->setWordWrap(true);
        soundContent9Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent9Lbl,17,0,Qt::AlignTop);

        QPushButton* soundFreqFactorDemo = new QPushButton("FreqFactor demo",soundChapter);
        soundFreqFactorDemo->setObjectName(SOUND_FREQ_FACTOR_DEMO);
        soundGrid->addWidget(soundFreqFactorDemo,18,0,Qt::AlignTop);

        connect(soundFreqFactorDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent10Lbl = new QLabel(
            styles +
                "<h4>freqModulation</h4>"
                "<ul>"
                "<li><span class=\"code\">freqModulation( sound )</span></li>"
                "</ul>"
                "<p>This method modulates a copy of the current sound "
                "with the sound passed as argument using <a "
                "class=\"link\" href=\"https://en.wikipedia.org/wiki/Frequency"
                "_modulation\">frequency modulation</a> and return this copy. "
                "To see more examples, see Synthesis section</p>"
                "<p>It works as a <a class=\"link\" href=\"https://en.wikiped"
                "ia.org/wiki/Low-frequency_oscillation\">LFO</a> controlling "
                "the frequency of the original sound.</p>"
            ,soundChapter);

        soundContent10Lbl->setOpenExternalLinks(true);
        soundContent10Lbl->setWordWrap(true);
        soundContent10Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent10Lbl,19,0,Qt::AlignTop);

        QPushButton* soundFreqModulationDemo = new QPushButton("FreqModulation demo",soundChapter);
        soundFreqModulationDemo->setObjectName(SOUND_FREQ_MODULATION_DEMO);
        soundGrid->addWidget(soundFreqModulationDemo,20,0,Qt::AlignTop);

        connect(soundFreqModulationDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent11Lbl = new QLabel(
            styles +
                "<h4>ampFactor</h4>"
                "<ul>"
                "<li><span class=\"code\">ampFactor( number )</span></li>"
                "</ul>"
                "<p>This method receives a <span class=\"code\">number"
                "</span> that multiply the amplitude and return a new "
                "sound with this value set. For example, a factor of "
                "0.5 will reduce the volume of the sound to the half. "
                "To see more examples, see Synthesis section.</p>"
                "<p>This is the best way to set the volume of a sound.</p>"
                "<p>Additionally you can call this method using the "
                "operator <span class=\"code\">*</span> or <span "
                "class=\"code\">/</span>:</p>"
            ,soundChapter);

        soundContent11Lbl->setWordWrap(true);
        soundContent11Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent11Lbl,21,0,Qt::AlignTop);

        QPushButton* soundAmpFactorDemo = new QPushButton("AmpFactor demo",soundChapter);
        soundAmpFactorDemo->setObjectName(SOUND_AMP_FACTOR_DEMO);
        soundGrid->addWidget(soundAmpFactorDemo,22,0,Qt::AlignTop);

        connect(soundAmpFactorDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent12Lbl = new QLabel(
            styles +
                "<ul>"
                "<li><span class=\"code\">ampFactor( sound )</span></li>"
                "</ul>"
                "<p>This method can be called giving a sound as a "
                "parameter and return a new sound modulated using "
                "<a class=\"link\" href=\"https://en.wikipedia.org"
                "/wiki/Amplitude_modulation\">amplitude modulation</a>."
                " To see more examples, see Synthesis section.</p>"
                "<p>It works as a <a class=\"link\" href=\"https://"
                "en.wikipedia.org/wiki/Low-frequency_oscillation\">LFO</a> controlling the amplitude of the original sound.</p>"
                "<p>Additionally you can call this method using the"
                " operator <span class=\"code\">*</span>:</p>"
            ,soundChapter);

        soundContent12Lbl->setOpenExternalLinks(true);
        soundContent12Lbl->setWordWrap(true);
        soundContent12Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent12Lbl,23,0,Qt::AlignTop);

        QPushButton* soundAmpFactorDemo2 = new QPushButton("AmpFactor demo",soundChapter);
        soundAmpFactorDemo2->setObjectName(SOUND_AMP_FACTOR_DEMO2);
        soundGrid->addWidget(soundAmpFactorDemo2,24,0,Qt::AlignTop);

        connect(soundAmpFactorDemo2,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* soundContent13Lbl = new QLabel(
            styles +
                "<h4>addition</h4>"
                "<p>The operator <span class=\"code\">+</span> "
                "is implemented to add sounds. "
                "It means to add point-by-point each sample. The "
                "resulting sound will be a new sound with higher "
                "total amplitude, so take care to multiply this "
                "sound with a reduced factor to avoid saturation.</p>"
            ,soundChapter);

        soundContent13Lbl->setWordWrap(true);
        soundContent13Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        soundGrid->addWidget(soundContent13Lbl,25,0,Qt::AlignTop);

        soundGrid->setRowStretch(25,2);

        QLabel* sampleTitleLbl = new QLabel("<h2>sample</h2>",sampleChapter);
        sampleGrid->addWidget(sampleTitleLbl,0,0,Qt::AlignLeft);

        QLabel* sampleContentLbl = new QLabel(
            styles +
                "<p>Sample is a type to store a .wav file and allows "
                "you to play it modifying some parameters.</p>"
                "<p>To load a sample from a file in your computer, "
                "you must use the function <span class=\"code\">SAMPLE()"
                "</span>.</p>"
                "<p>This data type has the following methods and operators:</p>"
                "<h4>play</h4>"
                "<ul>"
                "<li><span class=\"code\">play( number = 1 )</span></li>"
                "</ul>"
                "<p>This method play the sample once and takes one "
                "parameter. It is a time factor that multiplies the "
                "reproduction speed of the sample. By default this "
                "value is 1. For example if you call this method with "
                "a 0.5 the sample is played at half normal speed.</p>"
            ,sampleChapter);

        sampleContentLbl->setWordWrap(true);
        sampleContentLbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        sampleGrid->addWidget(sampleContentLbl,1,0,Qt::AlignTop);

        QPushButton* samplePlayDemo = new QPushButton("Play demo",sampleChapter);
        samplePlayDemo->setObjectName(SAMPLE_PLAY_DEMO);
        sampleGrid->addWidget(samplePlayDemo,2,0,Qt::AlignTop);

        connect(samplePlayDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* sampleContent2Lbl = new QLabel(
            styles +
                "<p><b>Note</b>: If time factor is 0, no sample is played</p>"
                "<ul>"
                "<li><span class=\"code\">play( ( arguments ) )</span></li>"
                "</ul>"
                "<p>Optionally you can call this method with arguments "
                "separated by commas. Each argument can have 2 numeric "
                "parameters. The first one is the time factor explained "
                "in the previous paragraph and the second is a "
                "\"virtual duration\". It's the duration in seconds to "
                "wait before play the next argument. It seems a bit "
                "strange, but an example could help to understand:</p>"
            ,sampleChapter);

        sampleContent2Lbl->setWordWrap(true);
        sampleContent2Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        sampleGrid->addWidget(sampleContent2Lbl,3,0,Qt::AlignTop);

        QPushButton* samplePlayDemo2 = new QPushButton("Play demo",sampleChapter);
        samplePlayDemo2->setObjectName(SAMPLE_PLAY_DEMO2);
        sampleGrid->addWidget(samplePlayDemo2,4,0,Qt::AlignTop);

        connect(samplePlayDemo2,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* sampleContent3Lbl = new QLabel(
            styles +
                "<h4>stop</h4>"
                "<ul>"
                "<li><span class=\"code\">stop()</span></li>"
                "</ul>"
                "<p>This method doesn't receive any argument. It "
                "just stops all playing versions of this sample "
                "variable.</p>"
            ,sampleChapter);

        sampleContent3Lbl->setWordWrap(true);
        sampleContent3Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        sampleGrid->addWidget(sampleContent3Lbl,5,0,Qt::AlignTop);

        QPushButton* sampleStopDemo = new QPushButton("Stop demo",sampleChapter);
        sampleStopDemo->setObjectName(SAMPLE_STOP_DEMO);
        sampleGrid->addWidget(sampleStopDemo,6,0,Qt::AlignTop);

        connect(sampleStopDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* sampleContent4Lbl = new QLabel(
            styles +
                "<p><b>Note</b>: if you copy and paste this code and run, "
                "you won't hear anything because you are playing "
                "and stopping the sound immediately after that. "
                "This method is useful when live coding, and you "
                "want to stop some sounds that are currently playing.</p>"
                "<h4>loop</h4>"
                "<ul>"
                "<li><span class=\"code\">loop( number ), loop( (arguments) )</span></li>"
                "</ul>"
                "<p>This method is similar to <span class=\"code\">"
                "play</span> but with the difference that when all "
                "arguments are played, starts again infinitely.</p>"
            ,sampleChapter);

        sampleContent4Lbl->setWordWrap(true);
        sampleContent4Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        sampleGrid->addWidget(sampleContent4Lbl,7,0,Qt::AlignTop);

        QPushButton* sampleLoopDemo = new QPushButton("Loop demo",sampleChapter);
        sampleLoopDemo->setObjectName(SAMPLE_LOOP_DEMO);
        sampleGrid->addWidget(sampleLoopDemo,8,0,Qt::AlignTop);

        connect(sampleLoopDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* sampleContent5Lbl = new QLabel(
            styles +
                "<p>This 3 chords will play in a loop indefinitely "
                "until you stop the sound.</p>"
                "<h4>setPanning</h4>"
                "<ul>"
                "<li><span class=\"code\">setPanning( number )</span></li>"
                "</ul>"
                "<p>This method receives a number between 0 and 1 "
                "representing the panning of the sound. 1 is the "
                "stereo right channel and 0 the left one. So, 0.5 "
                "is the default centered sound.</p>"
                "<p>This method modifies the current sample and "
                "return null</p>"
            ,sampleChapter);

        sampleContent5Lbl->setWordWrap(true);
        sampleContent5Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        sampleGrid->addWidget(sampleContent5Lbl,9,0,Qt::AlignTop);

        QPushButton* sampleSetPanningDemo = new QPushButton("SetPanning demo",sampleChapter);
        sampleSetPanningDemo->setObjectName(SAMPLE_SET_PANNING_DEMO);
        sampleGrid->addWidget(sampleSetPanningDemo,10,0,Qt::AlignTop);

        connect(sampleSetPanningDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* sampleContent6Lbl = new QLabel(
            styles +
                "<ul>"
                "<li><span class=\"code\">setPanning( sound )</span></li>"
                "</ul>"
                "<p>Optionally you can call this method but giving a "
                "sound as parameter. This generates a panning effect "
                "based on the sound passed as argument.</p>"
            ,sampleChapter);

        sampleContent6Lbl->setWordWrap(true);
        sampleContent6Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-top:15px;margin-bottom:15px;}");
        sampleGrid->addWidget(sampleContent6Lbl,11,0,Qt::AlignTop);

        QPushButton* sampleSetPanningDemo2 = new QPushButton("SetPanning demo",sampleChapter);
        sampleSetPanningDemo2->setObjectName(SAMPLE_SET_PANNING_DEMO2);
        sampleGrid->addWidget(sampleSetPanningDemo2,12,0,Qt::AlignTop);

        connect(sampleSetPanningDemo2,SIGNAL(clicked()),this,SLOT(showDemo()));

        sampleGrid->setRowStretch(12,2);

        dataTypesSectionLayout->addWidget(dataTypesContent);
    }
    break;
    case 3:
    {
        QVBoxLayout* flowControlSectionLayout = new QVBoxLayout(_sections[section]);
        _sections[section]->setLayout(flowControlSectionLayout);

        flowControlSectionLayout->setContentsMargins(0,8,0,8);

        QScrollArea* flowControlContent = new QScrollArea(_sections[section]);

        QGridLayout* mainGrid = new QGridLayout(flowControlContent);
        flowControlContent->setLayout(mainGrid);
        mainGrid->setContentsMargins(0,0,0,0);

        QLabel* titleLbl = new QLabel("<h1>Flow Control</h1>",flowControlContent);
        mainGrid->addWidget(titleLbl,0,0,Qt::AlignJustify);

        QLabel* spaceLbl = new QLabel("<h6></h6>",flowControlContent);
        mainGrid->addWidget(spaceLbl,1,0,Qt::AlignJustify);

        QTabWidget* chapters = new QTabWidget(flowControlContent);
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

        QFrame* ifFlow = new QFrame(chapters);
        QFrame* forFlow = new QFrame(chapters);

        QGridLayout* ifFlowGrid = new QGridLayout(ifFlow);
        ifFlow->setLayout(ifFlowGrid);

        QGridLayout* forFlowGrid = new QGridLayout(forFlow);
        forFlow->setLayout(forFlowGrid);

        chapters->addTab(ifFlow,"If");
        chapters->addTab(forFlow,"For");

        QLabel* ifTitleLbl = new QLabel("<h2>If statement</h2>",ifFlow);
        ifFlowGrid->addWidget(ifTitleLbl,0,0,Qt::AlignLeft);

        QLabel* ifContentLbl = new QLabel(
            styles +
                "<p>If statement is the most basic control flow. It "
                "checks the boolean comparison and if the result is "
                "<span class=\"code\">true</span>, code between braces are executed. Otherwise, "
                "code between braces are ignored.</p>"
                "<p><span class=\"code\">if( boolean comparison ) { code }</span></p>"
                "<p>An example of usage:</p>"
            ,ifFlow);

        ifContentLbl->setWordWrap(true);
        ifContentLbl->setOpenExternalLinks(true);
        ifContentLbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;}");
        ifFlowGrid->addWidget(ifContentLbl,1,0);

        QPushButton* ifDemo = new QPushButton("If demo",ifFlow);
        ifDemo->setObjectName(IF_DEMO);
        ifFlowGrid->addWidget(ifDemo,2,0,Qt::AlignTop);

        connect(ifDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* ifContent2Lbl = new QLabel(
            styles +
                "<p>Optionally, another keyword can be added after "
                "closing the braces. An <span class=\"code\">else"
                "</span> statement:</p>"
            ,ifFlow);

        ifContent2Lbl->setWordWrap(true);
        ifContent2Lbl->setOpenExternalLinks(true);
        ifContent2Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;margin-top:15px;}");
        ifFlowGrid->addWidget(ifContent2Lbl,3,0);

        QPushButton* ifDemo2 = new QPushButton("If-else demo",ifFlow);
        ifDemo2->setObjectName(IF_DEMO2);
        ifFlowGrid->addWidget(ifDemo2,4,0,Qt::AlignTop);

        connect(ifDemo2,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* ifContent3Lbl = new QLabel(
            styles +
                "<p>Between braces, a new scope is created. It means "
                "that all variables created inside it, are only valid "
                "between braces.</p>"
            ,ifFlow);

        ifContent3Lbl->setWordWrap(true);
        ifContent3Lbl->setOpenExternalLinks(true);
        ifContent3Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;margin-top:15px;}");
        ifFlowGrid->addWidget(ifContent3Lbl,5,0,Qt::AlignTop);

        ifFlowGrid->setRowStretch(5,2);

        QLabel* forTitleLbl = new QLabel("<h2>For statement</h2>",forFlow);
        forFlowGrid->addWidget(forTitleLbl,0,0,Qt::AlignLeft);

        QLabel* forContentLbl = new QLabel(
            styles +
                "<p>For statement is a loop that iterates over a "
                "variable in order to control the number of "
                "iterations. The syntax is as follows:</p>"
                "<p><span class=\"code\">for( assignation ; "
                "boolean comparison ; assignation ) { code }</span></p>"
                "<p>The most common usage is something like this:</p>"
            ,forFlow);

        forContentLbl->setWordWrap(true);
        forContentLbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;}");
        forFlowGrid->addWidget(forContentLbl,1,0,Qt::AlignTop);

        QPushButton* forDemo = new QPushButton("For demo",forFlow);
        forDemo->setObjectName(FOR_DEMO);
        forFlowGrid->addWidget(forDemo,2,0,Qt::AlignTop);

        connect(forDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* forContent2Lbl = new QLabel(
            styles +
                "<p>Steps of executions are:</p>"
                "<ol>"
                "<li>First, a variable <span class=\"code\">i</span> is created equal to 0.</li>"
                "<li>Comparison is evaluated. If true execute braces, else continue executing code after close brace.</li>"
                "<li>Execute the code between braces.</li>"
                "<li>Execute the second assignation ( <span class=\"code\">i++ (i = i + 1)</span> ).</li>"
                "<li>Go back to step 2.</li>"
                "</ol>"
                "<p>Between braces, a new scope is created. It means "
                "that all variables created inside this loop, are "
                "only valid between braces.</p>"
            ,forFlow);

        forContent2Lbl->setWordWrap(true);
        forContent2Lbl->setOpenExternalLinks(true);
        forContent2Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;margin-top:15px;}");
        forFlowGrid->addWidget(forContent2Lbl,3,0,Qt::AlignTop);

        forFlowGrid->setRowStretch(3,2);

        flowControlSectionLayout->addWidget(flowControlContent);
    }
    break;
    case 4:
    {
        QVBoxLayout* functionsSectionLayout = new QVBoxLayout(_sections[section]);
        _sections[section]->setLayout(functionsSectionLayout);
        functionsSectionLayout->setContentsMargins(0,8,0,8);

        QScrollArea* functionsContent = new QScrollArea(_sections[section]);

        QGridLayout* grid = new QGridLayout(functionsContent);
        functionsContent->setLayout(grid);

        QLabel* titleLbl = new QLabel("<h1>Functions</h1>",functionsContent);
        grid->addWidget(titleLbl,0,0,Qt::AlignJustify);

        QLabel* spaceLbl = new QLabel("<h6></h6>",functionsContent);
        grid->addWidget(spaceLbl,1,0,Qt::AlignJustify);

        QLabel* functionsContentLbl = new QLabel(
            styles +
                "<p>Functions are pieces of code that are encapsulated "
                "to be executed later (more than one time). Examples "
                "of functions are on this language when you create a "
                "sample <span class=\"code\">SAMPLE()</span>. Basically "
                "functions have a name, can take arguments and can "
                "return a value.</p>"
                "<p>ChordScript gives you a possibility to create your "
                "custom functions. The syntax to <b>define<b> a "
                "function is the following:</p>"
            ,functionsContent);

        functionsContentLbl->setOpenExternalLinks(true);
        functionsContentLbl->setWordWrap(true);
        functionsContentLbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;}");
        grid->addWidget(functionsContentLbl,2,0,Qt::AlignTop);

        QPushButton* functionsDemo = new QPushButton("Function demo",functionsContent);
        functionsDemo->setObjectName(FUNCTIONS_DEMO);
        grid->addWidget(functionsDemo,3,0,Qt::AlignTop);

        connect(functionsDemo,SIGNAL(clicked()),this,SLOT(showDemo()));

        QLabel* functionsContent2Lbl = new QLabel(
            styles +
                "<p>Additionally, a function can return a <span "
                "class=\"code\">null</span> value. In that case, "
                "return keyword must be written without any value: "
                "<span class=\"code\">return</span>;</p>"
                "<p>Between braces, a new scope is created. "
                "It means that all variables created inside it, "
                "are only valid between braces.</p>"
            ,functionsContent);

        functionsContent2Lbl->setOpenExternalLinks(true);
        functionsContent2Lbl->setWordWrap(true);
        functionsContent2Lbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;margin-top:15px;}");
        grid->addWidget(functionsContent2Lbl,4,0,Qt::AlignTop);

        grid->setRowStretch(4,2);

        functionsSectionLayout->addWidget(functionsContent);
    }
    break;
    case 5:
    {
        QVBoxLayout* definitionsSectionLayout = new QVBoxLayout(_sections[section]);
        _sections[section]->setLayout(definitionsSectionLayout);
        definitionsSectionLayout->setContentsMargins(0,8,0,8);

        QScrollArea* definitionsContent = new QScrollArea(_sections[section]);

        QGridLayout* grid = new QGridLayout(definitionsContent);
        definitionsContent->setLayout(grid);

        QLabel* titleLbl = new QLabel("<h1>Pre-definitions</h1>",definitionsContent);
        grid->addWidget(titleLbl,0,0,Qt::AlignJustify);

        QLabel* spaceLbl = new QLabel("<h6></h6>",definitionsContent);
        grid->addWidget(spaceLbl,1,0,Qt::AlignJustify);

        QLabel* definitionsContentLbl = new QLabel(
            styles +
                "<p>ChordScript defines some variables and functions "
                "to simplify and make easy to program. Here is a list "
                "of defined variables:</p>"
                "<h4>Notes</h4>"
                "<p>Notes are defined to replace frequencies and uses "
                "a simple nomenclature. Starting from C0 (the lowest "
                "C of the first octave) and ends in B8 (the highest "
                "B in the ninth octave)</p>"
                "<ul>"
                "<li><span class=\"code\">C0</span></li>"
                "<li><span class=\"code\">Cs0</span>=<span class=\"code\">Db0</span></li>"
                "<li><span class=\"code\">D0</span></li>"
                "<li><span class=\"code\">Ds0</span>=<span class=\"code\">Eb0</span></li>"
                "<li>...</li>"
                "<li><span class=\"code\">As8</span>=<span class=\"code\">Bb8</span></li>"
                "<li><span class=\"code\">B8</span></li>"
                "</ul>"
                "<h4>Sounds</h4>"
                "<p>As you see, sounds can be combined to generate "
                "new sounds, but basic sounds are:</p>"
                "<ul>"
                "<li><span class=\"code\">S_SIN</span> a sinusoidal wave</li>"
                "<li><span class=\"code\">S_SQUARE</span> a squared wave</li>"
                "<li>New sounds incoming...</li>"
                "</ul>"
                "<p>This is the list of defined functions:</p>"
                "<ul>"
                "<li><span class=\"code\">sample SAMPLE( string )</span> receives the path of a file and returns a sample</li>"
                "<li><span class=\"code\">STOP()</span> stops all the sound</li>"
                "<li><span class=\"code\">START_RECORDING()</span> start recording all sound to be stored in a file</li>"
                "<li><span class=\"code\">STOP_RECORDING()</span> stop recording and dump the .wav file</li>"
                "<li><span class=\"code\">START_SERVER()</span> start the Jack server if it's not running</li>"
                "<li><span class=\"code\">RESTART_SERVER()</span> stop and start the server again</li>"
                "<li><span class=\"code\">KILL_SERVER()</span> stop the Jack server</li>"
                "</ul>"
            ,definitionsContent);

        definitionsContentLbl->setOpenExternalLinks(true);
        definitionsContentLbl->setWordWrap(true);
        definitionsContentLbl->setStyleSheet(
            "QLabel {font-size:15px;margin-bottom:15px;}");
        grid->addWidget(definitionsContentLbl,2,0,Qt::AlignTop);

        grid->setRowStretch(2,2);

        definitionsSectionLayout->addWidget(definitionsContent);
    }
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
    case 3:
    {
        QString code =
            "number pitch = A4;\n\n"

            "argument myArgument = ([pitch],1);\n\n"

            "pitch = E5;\n\n"

            "S_SIN.play(myArgument); # Here is played A4 during a second\n\n";
        emit demo(code);
    }
    break;
    case 4:
    {
        QString code =
            "argument myArgument = (4, 5, 6);\n\n"

            "# Call the method to add another number at the end\n"
            "myArgument.push( 7 );\n\n"

            "# At this moment, the argument is (4, 5, 6, 7)\n";
        emit demo(code);
    }
    break;
    case 5:
    {
        QString code =
            "argument myArgument = (4, 5, 6);\n\n"

            "# Call the method to add another number at the end\n"
            "myArgument.pop();\n\n"

            "# At this moment, the argument is (4, 5)\n";
        emit demo(code);
    }
    break;
    case 6:
    {
        QString code =
            "# Here we creates an array. It can hold elements of any type.\n"
            "array myArray = [1, 3, 6, \"hi\", S_SIN];\n";
        emit demo(code);
    }
    break;
    case 7:
    {
        QString code =
            "array myArray = [3, 5, 6, 7];\n\n"

            "number aNumber = myArray.size();\n\n"

            "# aNumber is equal to 4;\n";
        emit demo(code);
    }
    break;
    case 8:
    {
        QString code =
            "array myArray = [3, 5, 6];\n\n"

            "number aNumber = myArray.at(1);\n\n"

            "# aNumber is equal to 5\n";
        emit demo(code);
    }
    break;
    case 9:
    {
        QString code =
            "array myArray = [3, 5, 6];\n\n"

            "myArray.push(7);\n\n"

            "# Now, myArray is [3, 5, 6, 7];\n";
        emit demo(code);
    }
    break;
    case 10:
    {
        QString code =
            "array myArray = [3, 5, 6];\n\n"

            "myArray.insert(1, \"water\");\n\n"

            "# Now, myArray is [3, \"water\", 5, 6];\n";
        emit demo(code);
    }
    break;
    case 11:
    {
        QString code =
            "array myArray = [3, 5, 6];\n\n"

            "myArray.remove(1);\n\n"

            "# Now, myArray is [3, 6];\n";
        emit demo(code);
    }
    break;
    case 12:
    {
        QString code =
            "array myArray = [3, 5, 6];\n\n"

            "myArray.pop();\n\n"

            "# Now, myArray is [3, 5];\n";
        emit demo(code);
    }
    break;
    case 13:
    {
        QString code =
            "# This line creates a sound using the pre defined sound called S_SIN.\n"
            "# See synthesis section for more information about sounds.\n"
            "sound mySound = S_SIN;\n\n"

            "# Play a chord of A minor during one second\n"
            "mySound.play( [A4, C5, E5], 1 );\n\n"

            "# This sound is executed at the same time and holds a tone forever.\n"
            "mySound.play( [A2] );\n";
        emit demo(code);
    }
    break;
    case 14:
    {
        QString code =
            "# Define a sound\n"
            "sound mySound = S_SIN;\n\n"

            "# Call the method to play 3 chords one after the other.\n"
            "# The total duration of this play is 4 seconds.\n"
            "mySound.play(\n"
            "\t( [A4, C5, E5], 1 ),\n"
            "\t( [F4, A4, C5], 1 ),\n"
            "\t( [G4, C5, E5], 2 )\n"
            ");\n";
        emit demo(code);
    }
    break;
    case 15:
    {
        QString code =
            "# Create a new sound using the base sound S_SQUARE\n"
            "sound mySound = S_SQUARE;\n"

            "# Play a 440 A during some time\n"
            "mySound.play( [440] );\n\n"

            "# And some seconds later...\n"
            "# Stop the sound\n"
            "mySound.stop();\n";
        emit demo(code);
    }
    break;
    case 16:
    {
        QString code =
            "# Define a sound\n"
            "sound mySound = S_SIN;\n\n"

            "# Call the method to loop this 3 chords.\n"
            "mySound.loop(\n"
            "\t( [A4, C5, E5], 1 ),\n"
            "\t( [F4, A4, C5], 1 ),\n"
            "\t( [G4, C5, E5], 2 )\n"
            ");\n";
        emit demo(code);
    }
    break;
    case 17:
    {
        QString code =
            "# Define a sound completely panned to the right channel\n"
            "sound mySound = S_SIN.setPanning(1);\n\n"

            "# Play this chords in the right channel\n"
            "mySound.play( [A4, C5, E5], 1 );\n";
        emit demo(code);
    }
    break;
    case 18:
    {
        QString code =
            "# Define a simple sound\n"
            "sound mySound = S_SIN;\n"

            "# Define a sound with panning controlled by the other sound\n"
            "sound myPannedSound = S_SIN.setPanning(mySound);\n\n"

            "# Play this bass with a fast panning change between channels during 1 second\n"
            "myPannedSound.play( [100], 1 );\n";
        emit demo(code);
    }
    break;
    case 19:
    {
        QString code =
            "# Define a sound with a constant frequency\n"
            "sound mySound = S_SIN.constantFreq( A4 );\n\n"

            "# Play A4 during 1 second (instead of E3)\n"
            "mySound.play( [E3], 2 );\n";
        emit demo(code);
    }
    break;
    case 20:
    {
        QString code =
            "# Define a sound with a constant frequency\n"
            "sound mySound = S_SIN( A4 );\n\n"

            "# Play A4 during 1 second (instead of E3)\n"
            "mySound.play( [E3], 2 );\n";
        emit demo(code);
    }
    break;
    case 21:
    {
        QString code =
            "# Define a sound with a frequency factor set\n"
            "sound mySound = S_SIN.freqFactor( 2 );\n\n"

            "# Play E4 instead of E3 during 2 seconds\n"
            "mySound.play( [E3], 2 );\n";
        emit demo(code);
    }
    break;
    case 22:
    {
        QString code =
            "# Define a basic sound with a constant freq of 10Hz\n"
            "sound myLFO = S_SIN( 10 );\n\n"

            "# Creates a new sound modulated using myLFO\n"
            "sound mySound = S_SIN.freqModulation(myLFO);\n\n"

            "# Play a note during 2 seconds\n"
            "mySound.play( [E3], 2 );\n";
        emit demo(code);
    }
    break;
    case 23:
    {
        QString code =
            "# Define a sound that will be played 0.3 times low.\n"
            "sound mySound = S_SIN.ampFactor( 0.3 );\n\n"

            "# This line generates the same result.\n"
            "mySound = S_SIN * 0.3;\n\n"

            "# Play a note during 2 seconds\n"
            "mySound.play( [E3], 2 );\n";
        emit demo(code);
    }
    break;
    case 24:
    {
        QString code =
            "# Define a sound with a small constant frequency (5 Hz)\n"
            "sound myLFO = S_SIN(5);\n\n"

            "# Use this LFO to modulate a \"carrier\" of a higher frequency\n"
            "sound mySound = S_SIN.ampFactor(myLFO);\n\n"

            "# This line generates the same result.\n"
            "mySound = S_SIN * myLFO;\n\n"

            "# Play a note during 2 seconds\n"
            "mySound.play( [E3], 2 );\n";
        emit demo(code);
    }
    break;
    case 25:
    {
        QString code =
            "# Define a sample variable\n"
            "sample mySample = SAMPLE(\"path/to/my/sample.wav\");\n\n"

            "# Play the sample once\n"
            "mySample.play();\n\n"

            "# Play the sample once at the same time but at double speed\n"
            "mySample.play(2);\n";
        emit demo(code);
    }
    break;
    case 26:
    {
        QString code =
            "# Define a sample variable\n"
            "sample mySample = SAMPLE(\"path/to/my/sample.wav\");\n"
            "# Lets suppose that this sample has a duration of 2 seconds\n\n"

            "mySample.play(\n"
            "\t(1),    # Play the sample with no alterations\n"
            "\t(1, 1), # Play the sample normally but with a virtual duration of 1 second\n"
            "\t(1, 3), # Play the sample again but it starts 1 second before the last one ends\n"
            "\t(),     # Play the sample 1 second (2s + 1s = 3s) after the last ends.\n"
            ");\n";
        emit demo(code);
    }
    break;
    case 27:
    {
        QString code =
            "# Create a new sample\n"
            "sample mySample = SAMPLE(\"path/to/my/sample.wav\");\n\n"

            "# Play the samples\n"
            "mySample.play();\n\n"

            "# And some seconds later...\n"
            "# Stop the sample\n"
            "mySample.stop();\n";
        emit demo(code);
    }
    break;
    case 28:
    {
        QString code =
            "# Create a new sample\n"
            "sample mySample = SAMPLE(\"path/to/my/sample.wav\");\n\n"

            "mySample.loop(\n"
            "\t(1),\n"
            "\t(1, 3),\n"
            "\t(),\n"
            ");\n";
        emit demo(code);
    }
    break;
    case 29:
    {
        QString code =
            "# Create a new sample\n"
            "sample mySample = SAMPLE(\"path/to/my/sample.wav\");\n\n"

            "mySample.setPanning(1);\n\n"

            "# Play the sample in the right channel\n"
            "mySample.play();\n";
        emit demo(code);
    }
    break;
    case 30:
    {
        QString code =
            "# Define a simple sound\n"
            "sound mySound = S_SIN(100);\n\n"

            "# Define a sample with panning controlled by the other sound\n"
            "sample myPannedSample = SAMPLE(\"path/to/my/sample.wav\");\n\n"

            "myPannedSample.setPanning(mySound);\n\n"

            "# Play this sample with a fast panning change between channels during 1 second\n"
            "myPannedSample.play();\n";
        emit demo(code);
    }
    break;
    case 31:
    {
        QString code =
            "number a = 5;\n"
            "if( a < 10 ) {\n"
            "\t# a is less than 10, so this code is executed\n"
            "}\n"
            "# Code continues execution after executing the code inside if braces\n";
        emit demo(code);
    }
    break;
    case 32:
    {
        QString code =
            "number a = 5;\n"
            "if( a < 10 ) {\n"
            "\t# a is less than 10, so this code is executed\n"
            "}\n"
            "else {\n"
            "\t# If a is bigger than 10, this code is executed.\n"
            "}\n"
            "# More code...\n";
        emit demo(code);
    }
    break;
    case 33:
    {
        QString code =
            "for( number i = 0; i < 10 ; i++ ) \n"
            "{\n"
            "\t# This code will be executed 10 times\n"
            "}\n";
        emit demo(code);
    }
    break;
    case 34:
    {
        QString code =
            "# The first word is the data type returned by the function.\n"
            "sound myFunction( sound aSound ) {   # This function receives a sound as parameter and can be used inside the function with the name \"aSound\"\n"
            "\t # Some code that can use the sound received as parameter.\n"
            "\t return aSound.setPanning(0.2);  # This function must return a sound, because its the defined data type\n"
            "}\n";
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
