//
// Created by rolf on 29-11-19.
//

#ifndef ROBOTEAM_MIMIR_VISUALIZER_H
#define ROBOTEAM_MIMIR_VISUALIZER_H
#include <QGraphicsView>
#include <proto/messages_robocup_ssl_wrapper.pb.h>
#include <simulator/settings/WorldSettings.h>
#include <simulator/settings/RobotSettings.h>
#include <unordered_map>
namespace interface {

class Visualizer : public QGraphicsView {
    Q_OBJECT
    public:
        explicit Visualizer(const WorldSettings& worldSettings,const RobotSettings& yellow,const RobotSettings& blue,
                const SSL_GeometryData &geometry, QWidget* parent = nullptr);
    public slots:
        void setGeometryData(const SSL_GeometryData &geometry, WorldSettings settings);
        void setBlueSettings(RobotSettings settings);
        void setYellowSettings(RobotSettings settings);
        void setWorldSettings(WorldSettings settings);
        void addDetections(const std::vector<SSL_WrapperPacket>& frames);
    protected:
        void drawBackground(QPainter* painter, const QRectF &rect) override;
        void drawForeground(QPainter *painter, const QRectF &rect) override;
        void resizeEvent(QResizeEvent *event) override;
    private:
        void drawRobot(QPainter* painter,const SSL_DetectionRobot &robot, RobotSettings* settings, const QColor& color);
        void drawBall(QPainter* painter,const SSL_DetectionBall &ball);
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

        RobotSettings yellowSettings;
        RobotSettings blueSettings;
        WorldSettings worldSettings;

        SSL_GeometryData geometryData;
        std::unordered_map<int,SSL_DetectionFrame> cameraFrames;
        bool geometryUpdated=true;

};
}
#endif //ROBOTEAM_MIMIR_VISUALIZER_H
