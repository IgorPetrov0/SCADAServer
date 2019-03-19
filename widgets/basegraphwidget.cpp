#include "basegraphwidget.h"
#include "ui_basegraphwidget.h"

baseGraphWidget::baseGraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::baseGraphWidget)
{
    ui->setupUi(this);
    graphWidget *gWidget = new graphWidget;
    graphWidgetsArray.append(gWidget);
    ui->tabWidget->addTab(gWidget,tr("Текущая"));
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(tabCloseSlot(int)));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(changedSlot(int)));
}
////////////////////////////////////////////////////////////////////
baseGraphWidget::~baseGraphWidget()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////
void baseGraphWidget::visualiseGraphFirstTab(dayGraph *array){
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabText(0,array->name+tr(" сегодня"));
    graphWidgetsArray.at(0)->visualiseGraph(array);
}
////////////////////////////////////////////////////////////////////////////////
void baseGraphWidget::visualizeGraphNewTab(dayGraph *array){
    graphWidget *gWidget = new graphWidget;
    graphWidgetsArray.append(gWidget);
    ui->tabWidget->addTab(gWidget,array->name+" "+array->date.toString("dd_MM_yyyy"));
    int index=ui->tabWidget->count()-1;
    ui->tabWidget->setCurrentIndex(index);
    graphWidgetsArray.at(index)->visualiseGraph(array);
}
/////////////////////////////////////////////////////////////////////////////////////////
void baseGraphWidget::updateContent(){
    int currentTab=ui->tabWidget->currentIndex();
    graphWidgetsArray.at(currentTab)->updateContent();
}
///////////////////////////////////////////////////////////////////////////////////////
void baseGraphWidget::resizeEvent(QResizeEvent *event){
    QRect rect;
    rect.setX(0);
    rect.setY(0);
    rect.setWidth(this->width());
    rect.setHeight(this->height());
    ui->tabWidget->setGeometry(rect);
}
/////////////////////////////////////////////////////////////////////////////////////
void baseGraphWidget::tabCloseSlot(int index){
    if(index!=0){//нулевой таб не удаляем
        delete graphWidgetsArray[index];
        graphWidgetsArray.remove(index);
        ui->tabWidget->removeTab(index);
    }
}
//////////////////////////////////////////////////////////////////////////////////////
void baseGraphWidget::changedSlot(int index){
    int t=ui->tabWidget->currentIndex();
    t=0;
}
////////////////////////////////////////////////////////////////////////////
