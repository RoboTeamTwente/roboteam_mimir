//
// Created by rolf on 29-11-19.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZER_H
#define ROBOTEAM_MIMIR_VISUALIZER_H
#include <QGraphicsView>
#include <proto/messages_robocup_ssl_geometry.pb.h>
#include "../simulator/config/WorldSettings.h"
#include "../simulator/config/RobotSettings.h"
namespace interface {

class Visualizer : public QGraphicsView {
    Q_OBJECT
    public:
        explicit Visualizer(WorldSettings* worldSettings, RobotSettings* yellow, RobotSettings* blue,
                const SSL_GeometryData &geometry, QWidget* parent = nullptr);
        void setGeometryData(const SSL_GeometryData &geometry);
        void setRobotSettings(RobotSettings* settings, bool isYellow);
        void setWorldSettings(WorldSettings* settings);
    protected:
        void drawBackground(QPainter* painter, const QRectF &rect) override;
        //void drawForeground(QPainter *painter, const QRectF &rect) override;
    private:
        void drawField(QPainter* painter);
        void drawFieldLines(QPainter* painter);
        void drawGoal(QPainter* painter, bool isLeft);
        void showWholeField();
        void updateField();
    private slots:
        void updateAll();

    private:
        QGraphicsScene* scene;
        QRectF sceneRect;
        QTimer* timer;

        RobotSettings* yellowSettings;
        RobotSettings* blueSettings;
        WorldSettings* worldSettings;

        SSL_GeometryData geometryData;
        bool geometryUpdated;

};
}
#endif //ROBOTEAM_MIMIR_VISUALIZER_H
