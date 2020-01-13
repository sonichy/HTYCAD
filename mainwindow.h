#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QCamera>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Qt3DExtras::Qt3DWindow *window;
    Qt3DCore::QEntity *rootEntity;
    Qt3DExtras::QPhongMaterial *material;
    Qt3DRender::QCamera *camera;
    QString filename="";
    QWidget *widget3d;
    QVector3D vector3D_camera;
    bool isStop;
    void drawLine(const QVector3D &start, const QVector3D &end, const QColor &color, Qt3DCore::QEntity *_rootEntity);
    QSettings settings;

private slots:
    void on_actionRender_triggered();
    void on_actionReduce_triggered();
    void on_actionIncrease_triggered();
    void on_actionReset_triggered();
    void on_actionRotateLeft_triggered();
    void on_actionAutoRotateLeft_triggered();
    void on_actionRotateRight_triggered();
    void on_action_export_triggered();
    void on_action_background_triggered();
    void on_action_about_triggered();

};

#endif // MAINWINDOW_H