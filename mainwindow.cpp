#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QPointLight>
#include <QFileDialog>
#include <QtMath>
#include <QTimer>
#include <QColorDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(QCoreApplication::organizationName(), QCoreApplication::applicationName())
{
    ui->setupUi(this);
    isStop = true;
    window = new Qt3DExtras::Qt3DWindow();
    QString srgb = settings.value("color_background", "0xffffff").toString();
    //window->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d9f)));
    bool ok;
    window->defaultFrameGraph()->setClearColor(QColor(QRgb(srgb.toInt(&ok, 16))));
    QWidget *widget3d = QWidget::createWindowContainer(window);
    ui->splitter->addWidget(widget3d);
    //ui->widget = window;
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 3);
    rootEntity = new Qt3DCore::QEntity;
    window->setRootEntity(rootEntity);
    window->show();

    // Material
    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0xbeb32b)));

    camera = window->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    vector3D_camera = QVector3D(10, 10, 10);
    camera->setPosition(vector3D_camera);
    camera->setUpVector(QVector3D(0, 1, 0));
    camera->setViewCenter(QVector3D(0, 0, 0));

    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);//鼠标拖动场景
    camController->setCamera(camera);

    //光源
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(QVector3D(10, 10, 10));
    lightEntity->addComponent(transform);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRender_triggered()
{
    //旋转坐标轴
    /*
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setRotationX(90);
    //transform->setRotationY(90);
    rootEntity->addComponent(transform);
    */

    //坐标轴
    //https://forum.qt.io/topic/66808/qt3d-draw-grid-axis-lines
    drawLine({ 0, 0, 0 }, { 10, 0, 0 }, Qt::red, rootEntity); // X
    drawLine({ 0, 0, 0 }, { 0, 10, 0 }, Qt::green, rootEntity); // Y
    drawLine({ 0, 0, 0 }, { 0, 0, 10 }, Qt::blue, rootEntity); // Z
    //文字 Qt>=5.9
    /*
    auto *text2d = new Qt3DExtras::QText2DEntity(_rootEntity);
    text2d->setText("LOL!!!");
    text2d->setHeight(3);
    text2d->setWidth(3);
    text2d->setColor(Qt::green);
    text2d->setFont(QFont("Courier New", 10));
    */

    Qt3DExtras::QCylinderMesh *cylinderMesh = new Qt3DExtras::QCylinderMesh();
    cylinderMesh->setRadius(1);
    cylinderMesh->setLength(4);
    cylinderMesh->setRings(10);
    cylinderMesh->setSlices(10);
    //Qt3DCore::QTransform *cylinderTransform = new Qt3DCore::QTransform();
    //cylinderTransform->setScale(1.5f);
    //cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 45.0f));
    //cylinderTransform->setTranslation(QVector3D(-5.0f, 4.0f, -1.5));
    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0xff0000)));
    Qt3DCore::QEntity *cylinderEntity = new Qt3DCore::QEntity(rootEntity);
    cylinderEntity->addComponent(cylinderMesh);
    cylinderEntity->addComponent(material);
    //m_cylinderEntity->addComponent(cylinderTransform);

    Qt3DCore::QEntity *cuboidEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DExtras::QCuboidMesh *cuboidMesh = new Qt3DExtras::QCuboidMesh();
    cuboidMesh->setXExtent(3);
    cuboidMesh->setYExtent(2);
    cuboidMesh->setZExtent(4);
    //Qt3DCore::QTransform *cuboidTransform = new Qt3DCore::QTransform();
    //cuboidTransform->setTranslation(QVector3D(1.0f, -1.0f, 1.0f));
    //cuboidTransform->setRotationX(30);
    //cuboidTransform->setRotationY(40);
    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0x00ff00)));
    cuboidEntity->addComponent(cuboidMesh);
    cuboidEntity->addComponent(material);
    //cuboidEntity->addComponent(cuboidTransform);

    Qt3DExtras::QConeMesh *coneMesh = new Qt3DExtras::QConeMesh();
    coneMesh->setTopRadius(1);
    coneMesh->setBottomRadius(2);
    coneMesh->setLength(2);
    coneMesh->setRings(5);
    coneMesh->setSlices(20);
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setTranslation(QVector3D(3, 3, 2));
    Qt3DCore::QEntity *coneEntity = new Qt3DCore::QEntity(rootEntity);
    coneEntity->addComponent(coneMesh);
    material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(QColor(QRgb(0x0000ff)));
    coneEntity->addComponent(material);
    coneEntity->addComponent(transform);

}

void MainWindow::on_actionReduce_triggered()
{
    QVector3D vector3d = camera->position();
    if(ui->actionX->isChecked())
        vector3d.setX(vector3d.x() - 0.1);
    if(ui->actionY->isChecked())
        vector3d.setY(vector3d.y() - 0.1);
    if(ui->actionZ->isChecked())
        vector3d.setZ(vector3d.z() - 0.1);
    camera->setPosition(vector3d);
    ui->statusBar->showMessage("Camera.Position (" + QString::number(vector3d.x()) + ", " + QString::number(vector3d.y()) + ", " + QString::number(vector3d.z()) + ")");
}

void MainWindow::on_actionIncrease_triggered()
{
    QVector3D vector3d = camera->position();
    if(ui->actionX->isChecked())
        vector3d.setX(vector3d.x() + 0.1);
    if(ui->actionY->isChecked())
        vector3d.setY(vector3d.y() + 0.1);
    if(ui->actionZ->isChecked())
        vector3d.setZ(vector3d.z() + 0.1);
    camera->setPosition(vector3d);
    ui->statusBar->showMessage("Camera.Position (" + QString::number(vector3d.x()) + ", " + QString::number(vector3d.y()) + ", " + QString::number(vector3d.z()) + ")");
}

void MainWindow::on_actionReset_triggered()
{
    isStop = true;
    camera->setUpVector(QVector3D(0, 1, 0));
    camera->setPosition(vector3D_camera);
    ui->statusBar->showMessage("Camera.Position (" + QString::number(camera->position().x()) + ", " + QString::number(camera->position().y()) + ", " + QString::number(camera->position().z()) + ")");
}

void MainWindow::on_actionRotateLeft_triggered()
{
    QVector3D vector3D = camera->position();
    qreal r = qSqrt(vector3D.x() * vector3D.x() + vector3D.z() * vector3D.z());
    qreal a = qAsin(vector3D.z() / r);
    a += 0.01;
    qreal z = r * qSin(a);
    qreal x = r * qCos(a);
    vector3D.setX(x);
    vector3D.setZ(z);
    camera->setPosition(vector3D);
    camera->setUpVector(QVector3D(0, 1, 0));
    ui->statusBar->showMessage("a = " + QString::number(a) + ", Camera.Position (" + QString::number(camera->position().x()) + ", " + QString::number(camera->position().y()) + ", " + QString::number(camera->position().z()) + ")");
}

void MainWindow::on_actionRotateRight_triggered()
{
    QVector3D vector3D = camera->position();
    qreal r = qSqrt(vector3D.x() * vector3D.x() + vector3D.z() * vector3D.z());
    qreal a = qAsin(vector3D.z() / r);
    a -= 0.01;
    qreal z = r * qSin(a);
    qreal x = r * qCos(a);
    vector3D.setX(x);
    vector3D.setZ(z);
    camera->setPosition(vector3D);
    camera->setUpVector(QVector3D(0, 1, 0));
    ui->statusBar->showMessage("a = " + QString::number(a) + ", Camera.Position (" + QString::number(camera->position().x()) + ", " + QString::number(camera->position().y()) + ", " + QString::number(camera->position().z()) + ")");
}

void MainWindow::on_actionAutoRotateLeft_triggered()
{
    isStop = false;
    for(qreal a=0; a<2 * M_PI; a+= 0.1){
        if (isStop) break;
        QVector3D vector3D = camera->position();
        qreal r = qSqrt(vector3D.x() * vector3D.x() + vector3D.z() * vector3D.z());
        qreal z = r * qSin(a);
        qreal x = r * qCos(a);
        vector3D.setX(x);
        vector3D.setY(camera->position().y());
        vector3D.setZ(z);
        camera->setPosition(vector3D);
        camera->setUpVector(QVector3D(0, 1, 0));
        ui->statusBar->showMessage("a = " + QString::number(a) + ", Camera.Position (" + QString::number(camera->position().x()) + ", " + QString::number(camera->position().y()) + ", " + QString::number(camera->position().z()) + "), Camera.UpVector(" + QString::number(camera->upVector().x()) + ", " + QString::number(camera->upVector().y()) + ", " + QString::number(camera->upVector().z()) + ")");
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
}

void MainWindow::on_action_export_triggered()
{
    filename = QFileDialog::getSaveFileName(this, tr("Save File"), filename, tr("Images (*.png *.jpg)"));
    if(filename != ""){
        // >=Qt5.8 https://code.woboq.org/qt5/qt3d/tests/manual/rendercapture-cpp/mycapture.h.html
        //QPixmap pixmap(size());
        //widget3d->render(&pixmap);
        //pixmap.save(filename);
    }
}

void MainWindow::on_action_background_triggered()
{
    QString srgb = settings.value("color_background", "0xffffff").toString();
    bool ok;
    QColor color(QRgb(srgb.toInt(&ok, 16)));
    color = QColorDialog::getColor(color, this);
    if(color.isValid()){
        window->defaultFrameGraph()->setClearColor(color);
        QString srgb = "0x" + QString::number(color.red(),16) + QString::number(color.green(),16) + QString::number(color.blue(),16);
        //QString srgb = QString::number(color.rgb(),16);
        qDebug() << srgb;
        settings.setValue("color_background", srgb);
    }
}

void MainWindow::on_action_about_triggered()
{
    QMessageBox MB(QMessageBox::NoIcon, "关于", "海天鹰CAD 0.1\n一款基于 Qt3D 的3D程序。\n作者：海天鹰\nE-mail: sonichy@163.com\n主页：https://github.com/sonichy");
    MB.setIconPixmap(QPixmap(":/HTYCAD.png"));
    MB.exec();
}

void MainWindow::drawLine(const QVector3D &start, const QVector3D &end, const QColor &color, Qt3DCore::QEntity *_rootEntity)
{
    auto *geometry = new Qt3DRender::QGeometry(_rootEntity);

    // position vertices (start and end)
    QByteArray bufferBytes;
    bufferBytes.resize(3 * 2 * sizeof(float)); // start.x, start.y, start.end + end.x, end.y, end.z
    float *positions = reinterpret_cast<float*>(bufferBytes.data());
    *positions++ = start.x();
    *positions++ = start.y();
    *positions++ = start.z();
    *positions++ = end.x();
    *positions++ = end.y();
    *positions++ = end.z();

    auto *buf = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry);
    buf->setData(bufferBytes);

    auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    //positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setDataType(Qt3DRender::QAttribute::Float);
    //positionAttribute->setVertexSize(3);
    positionAttribute->setDataSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(buf);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(2);
    geometry->addAttribute(positionAttribute); // We add the vertices in the geometry

    // connectivity between vertices
    QByteArray indexBytes;
    indexBytes.resize(2 * sizeof(unsigned int)); // start to end
    unsigned int *indices = reinterpret_cast<unsigned int*>(indexBytes.data());
    *indices++ = 0;
    *indices++ = 1;

    auto *indexBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry);
    indexBuffer->setData(indexBytes);

    auto *indexAttribute = new Qt3DRender::QAttribute(geometry);
    //indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(2);
    geometry->addAttribute(indexAttribute); // We add the indices linking the points in the geometry

    // mesh
    auto *line = new Qt3DRender::QGeometryRenderer(_rootEntity);
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    auto *material = new Qt3DExtras::QPhongMaterial(_rootEntity);
    material->setAmbient(color);

    // entity
    auto *lineEntity = new Qt3DCore::QEntity(_rootEntity);
    lineEntity->addComponent(line);
    lineEntity->addComponent(material);
}