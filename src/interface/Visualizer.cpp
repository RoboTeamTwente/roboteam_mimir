//
// Created by rolf on 29-11-19.
//

#include "Visualizer.h"
#include <QTimer>
#include <QWheelEvent>
namespace interface {

Visualizer::Visualizer(WorldSettings* worldSettings, RobotSettings* yellow, RobotSettings* blue,
        const SSL_GeometryData &geometry, QWidget* parent)
        :
        blueSettings(blue),
        yellowSettings(yellow),
        worldSettings(worldSettings),
        QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setGeometryData(geometry);
    setOptimizationFlag(QGraphicsView::DontSavePainterState);
    setCacheMode(QGraphicsView::CacheBackground);



    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::HighQualityAntialiasing, true);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    timer = new QTimer(); //TODO: remove again?
    connect(timer, &QTimer::timeout, this, &Visualizer::updateAll);
    timer->start(20);

}
void Visualizer::setGeometryData(const SSL_GeometryData &geometry) {
    //Unfortunately there is no pretty way to compare protobuf objects.
    if (geometry.SerializeAsString() != geometryData.SerializeAsString()) {
        std::cout<<"updated geometry"<<std::endl;
        geometryData = geometry;
        geometryUpdated = true;
        const auto &field = geometry.field();
        double length = field.field_length();
        double width = field.field_width();
        double margin = 0.1*width;
        sceneRect = QRectF(- length*0.5 - margin, - width*0.5 - margin, length + 2*margin, width + 2*margin);
        showWholeField();
    }
}

void Visualizer::drawBackground(QPainter* painter, const QRectF &rect) {
    painter->save();// we save it to restore it to the original state later
    painter->setRenderHint(QPainter::Antialiasing);//turn anti aliasing on
    drawField(painter); //TODO: fix caching to not have to redraw, or put things in items.
    geometryUpdated = false;
    painter->restore();

}

void Visualizer::drawForeground(QPainter* painter, const QRectF &rect) {
    for (const auto &frame : cameraFrames) {
        const auto &det=frame.second;
        for (int i = 0; i <det.robots_blue_size() ; ++ i) {
            drawRobot(painter,det.robots_blue(i),blueSettings,Qt::blue);
        }
        for (int i = 0; i <det.robots_yellow_size() ; ++ i) {
            drawRobot(painter,det.robots_yellow(i),yellowSettings,Qt::yellow);
        }
        for (int i = 0; i<det.balls_size(); ++i){
            drawBall(painter,det.balls(i));
        }
    }

}
void Visualizer::drawField(QPainter* painter) {
    drawFieldLines(painter);
    drawGoal(painter, true);
    drawGoal(painter, false);

}
void Visualizer::drawFieldLines(QPainter* painter) {
    QPen pen;
    pen.setColor(Qt::white);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);

    const SSL_GeometryFieldSize &geometryField = geometryData.field();
    for (int i = 0; i < geometryField.field_lines_size(); ++ i) {
        const SSL_FieldLineSegment &line = geometryField.field_lines(i);
        QLineF graphicLine(line.p1().x(), line.p1().y(), line.p2().x(), line.p2().y());
        pen.setWidthF(line.thickness());
        painter->setPen(pen);
        painter->drawLine(graphicLine);
    }
    for (int j = 0; j < geometryField.field_arcs_size(); ++ j) {
        const SSL_FieldCicularArc &arc = geometryField.field_arcs(j);
        pen.setWidthF(arc.thickness());
        QRectF boundingBox(arc.center().x()-arc.radius(),arc.center().y()-arc.radius(),arc.radius()*2,arc.radius()*2);
        const double scaleFactor=180.0/(16.0*3.14159);//qt works with 1/16 of a degree rounded to ints for some unholy reason.
        painter->drawArc(boundingBox,arc.a1(),scaleFactor*arc.radius()*(arc.a2()-arc.a1()));
    }

}

void Visualizer::updateAll() {
    updateField();
}
void Visualizer::updateField() {


}
void Visualizer::showWholeField() {
    fitInView(sceneRect);

}
void Visualizer::drawGoal(QPainter* painter, bool isLeft) {
    const SSL_GeometryFieldSize &field = geometryData.field();
    QPen pen;
    pen.setColor(Qt::darkRed);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    QPainterPath path;
    double side = isLeft ? -1.0 : 1.0;
    double d = worldSettings->goalWallThickness* 0.5*1000;
    double l = field.field_length() * 0.5;
    double w = field.goal_width()* 0.5 + d;
    pen.setWidthF(2*d);
    painter->setPen(pen);
    path.moveTo(side * l, w);
    path.lineTo(side * (l + field.goal_depth() + d), w);
    path.lineTo(side * (l + field.goal_depth() + d), -w);
    path.lineTo(side * l, -w);
    painter->drawPath(path);
}
void Visualizer::resizeEvent(QResizeEvent* event) {
    showWholeField();
    QGraphicsView::resizeEvent(event);
}
void Visualizer::drawRobot(QPainter* painter,const SSL_DetectionRobot &bot, RobotSettings* settings, const QColor& color){
    const float radius=settings->radius*1000;
    const float startAngle=settings->startAngle; // should already be in degrees
    const float endAngle=settings->endAngle;
    const float sweepLength=endAngle-startAngle;
    QRectF rect(-radius, -radius, radius*2,radius*2);
    QPointF botPos(bot.x(),bot.y());
    rect.translate(botPos);

    QPainterPath path;
    path.arcMoveTo(rect,startAngle);
    path.arcTo(rect,startAngle+bot.orientation()*180/3.14159,sweepLength);
    path.closeSubpath();


    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->setOpacity(0.5);
    painter->drawPath(path);

    QFont f("Helvetica");
    f.setPointSizeF(radius);
    painter->setFont(f);
    painter->setPen(Qt::white);
    painter->setOpacity(1.0);
    painter->drawText(botPos+QPointF(-radius*0.2,radius*0.4),QString::number(bot.robot_id()));
}
void Visualizer::drawBall(QPainter* painter, const SSL_DetectionBall &ball) {
    const float radius=worldSettings->ballRadius*1000 ;
    QRectF rect(-radius,-radius,radius*2,radius*2);
    QPointF ballPos(ball.x(),ball.y());
    rect.translate(ballPos);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::green);
    painter->setOpacity(0.5);
    painter->drawEllipse(rect);

}
void Visualizer::setBlueSettings(RobotSettings* settings) {
    blueSettings=settings;
}
void Visualizer::setYellowSettings(RobotSettings* settings) {
    yellowSettings=settings;
}
void Visualizer::setWorldSettings(WorldSettings* settings) {
    worldSettings=settings;
}
void Visualizer::addDetections(const std::vector<SSL_WrapperPacket>& frames) {
    for (const auto &frame : frames) {
        if (frame.has_detection()){
            cameraFrames[frame.detection().camera_id()]=frame.detection();
        }
    }
}
}