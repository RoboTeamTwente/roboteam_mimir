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
        geometryData = geometry;
        geometryUpdated = true;
        const auto &field = geometry.field();
        double length = field.field_length();
        double width = field.field_width();
        double margin = 0.1*width;
        sceneRect = QRectF(- length*0.5 - margin, - width*0.5 - margin, length + 2*margin, width + 2*margin);
    }
}

void Visualizer::drawBackground(QPainter* painter, const QRectF &rect) {
    painter->save();// we save it to restore it to the original state later
    painter->setRenderHint(QPainter::Antialiasing);//turn anti aliasing on
    drawField(painter); //TODO: fix caching to not have to redraw, or put things in items.
    geometryUpdated = false;
    showWholeField();
    painter->restore();

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
        const double scaleFactor=180.0/(16.0*3.14159);//qt works with 1/16 of a degree. We approximate pi here because it's rounded to an int
        painter->drawArc(boundingBox,arc.a1(),scaleFactor*arc.radius()*(arc.a2()-arc.a1()));
    }

}

void Visualizer::updateAll() {
    updateField();
}
void Visualizer::updateField() {
    const SSL_GeometryFieldSize &geometryField = geometryData.field();
    double length = geometryField.field_length();
    double width = geometryField.field_width();
    double margin = 0.05*geometryField.field_width();
    sceneRect = QRectF(- length*0.5 - margin, - width*0.5 - margin, length + 2*margin, width + 2*margin);
    std::cout << "update field" << std::endl;

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
}