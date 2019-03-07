#include "graphwidget.h"
#include "ui_graphwidget.h"

graphWidget::graphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::graphWidget)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    graphArray=NULL;
    posY=0;
    xFactor=2;
    resizeXFactor=2;
    yFactor=1;
    contextMenu = createContextMenu();
    xOffset=0;
    connect(ui->horizontalScrollBar,SIGNAL(sliderMoved(int)),this,SLOT(sliderMovedSlot(int)));
}
/////////////////////////////////////////////////////////////////////////////////////
graphWidget::~graphWidget()
{
    delete contextMenu;
    delete ui;
}
///////////////////////////////////////////////////////////////////////////////////////
void graphWidget::visualiseGraph(dayGraph *array){
    int maxValue=0;
    maxValue=array->minutesArray[0].value;
    for(int n=1;n!=1440;n++){
        if(maxValue<array->minutesArray[n].value){
            maxValue=array->minutesArray[n].value;
        }
    }
    float tmp=graphZero/4*3;//три четверти от размера виджета
    if(tmp!=0){//tmp может стать нулем, если все значения =0
        if(tmp>maxValue){
            yFactor=maxValue/tmp;//оставляем сверху четверть от максимального значения
        }
        else{
            yFactor=tmp/maxValue;
        }
    }
    else{
        yFactor=1;
    }

    //рассчитываем xFactor так, чтобы уместить сутки на графике
    resizeXFactor=((float)this->width()-30)/1440;
    xFactor=resizeXFactor;
    graphArray=array;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void graphWidget::updateContent(){
    if(!this->hasMouseTracking()){
        int posInArray=QTime::currentTime().msecsSinceStartOfDay()/60000;
        calculateRails(posInArray);
        mousePosX=posInArray*xFactor+30;
    }
    update();
}
/////////////////////////////////////////////////////////////////////////////////
void graphWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    QRect rect;
    rect.setX(0);
    rect.setY(0);
    rect.setWidth(this->width());
    rect.setHeight(this->height());
    painter.drawRect(rect);

    //прямоугольник окна и координатные линии
    painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
    //painter.drawLine(0,0,0,this->height()-30);
    //painter.drawLine(0,this->height()-30,this->width(),this->height()-30);

    if(graphArray!=NULL){
        for(int n=0;n!=1440;n++){
            minutePoint tmpPoint=graphArray->minutesArray[n];
            int graphValue=graphZero-tmpPoint.value*yFactor;
            switch(tmpPoint.event){
                case(EVENT_OK):{
                    painter.setPen(QPen(QColor(),2,Qt::SolidLine));
                    break;
                }
            }
            int xPos=xFactor+30+xOffset;
            if(n==0){
                painter.drawPoint(n*xPos,graphValue);
            }
            else{
                int prevValue=graphZero-graphArray->minutesArray[n-1].value*yFactor;
                painter.drawLine((n-1)*xPos,prevValue,n*xPos,graphValue);
            }
        }
        //направляющие
        painter.setPen(QPen(Qt::black,1,Qt::SolidLine));
        painter.drawLine(0,posY,this->width(),posY);
        painter.drawLine(mousePosX,0,mousePosX,this->height()-0);
    }
}
///////////////////////////////////////////////////////////////////////////////
void graphWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->x()<30){
        mousePosX=30;
    }
    else{
        mousePosX=event->x();
    }
    mousePosY=event->y();
    if(graphArray!=NULL){
        calculateRails(mousePosX/xFactor);//позиция в массиве зависит от позиции мыши на экране);
    }
    update();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void graphWidget::wheelEvent(QWheelEvent *event){
    QPoint angle=event->angleDelta();
    if(angle.y()>0){
        xFactor+=0.1f;
    }
    else{
        xFactor-=0.1f;
    }
    if(xFactor>=resizeXFactor){
        ui->horizontalScrollBar->setVisible(true);
    }
    else{
        ui->horizontalScrollBar->setVisible(false);
        xFactor=resizeXFactor;
    }
    updateContent();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void graphWidget::contextMenuEvent(QContextMenuEvent *event){
    contextMenu->popup(event->globalPos());
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void graphWidget::resizeEvent(QResizeEvent *event){
    graphZero=this->height()-40;
    QRect rect;
    rect.setX(0);
    rect.setY(this->height()-20);
    rect.setWidth(this->width());
    rect.setHeight(20);
    ui->horizontalScrollBar->setGeometry(rect);
    resizeXFactor=((float)this->width()-30)/1440;
    xFactor=resizeXFactor;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void graphWidget::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        if(this->hasMouseTracking()){
            setMouseTracking(false);
            updateContent();
        }
        else{
            setMouseTracking(true);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
QMenu *graphWidget::createContextMenu(){
    QMenu *menu = new QMenu;
    QAction *calculateAll = new QAction(tr("Отчет за сутки"));
    menu->addAction(calculateAll);
    return menu;
}
////////////////////////////////////////////////////////////////////////
void graphWidget::calculateRails(int posInArray){
    if(graphArray!=NULL){
        visibleValue=QString::number(graphArray->minutesArray[posInArray].value);//отображаемое значение
        QTime time=QTime::fromMSecsSinceStartOfDay(posInArray*60000);
        visibleDateTime=graphArray->date.toString("dd_MM_yyyy ")+time.toString("hh:mm");
        posY=this->height()-graphArray->minutesArray[posInArray].value*yFactor-40;
    }
}
//////////////////////////////////////////////////////////////////
void graphWidget::slotCalcAll(){

}
///////////////////////////////////////////////////////////////////////////////////////////
void graphWidget::sliderMovedSlot(int value){
    xOffset=-value;
}


