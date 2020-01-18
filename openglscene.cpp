#include "openglscene.h"
#include "model.h"
#include<mainwindow.h>
#include <QtGui>
#include <QtOpenGL>
#include<QtConcurrent/QtConcurrent>
#include<GL/gl.h>
#include<GL/glu.h>
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

QDialog *OpenGLScene::createDialog(const QString &windowTitle) const
{
    QDialog *dialog = new QDialog(0);

    dialog->setAutoFillBackground(Qt::white);
    dialog->setFont(QFont ("Andalus",9.5,2,true));
    dialog->setStyleSheet("QDialog{ background-color : qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 grey, stop:1 white); }");
    dialog->setWindowOpacity(0.8);
    dialog->setWindowTitle(windowTitle);

    dialog->setLayout(new QVBoxLayout);
   return dialog;
}

OpenGLScene::OpenGLScene()
    : m_wireframeEnabled(false)
    , m_normalsEnabled(false)
    , m_modelColor(153, 255, 0)
    , m_backgroundColor(.5, .5, .5)
    , m_model(0)
    , m_lastTime(0)
    , m_distance(1.4f)
    , m_angularMomentum(0, 40, 0)
{

    QWidget *controls = createDialog(tr("Controls"));

    m_modelButton = new QPushButton(tr("Load model"));
    connect(m_modelButton, SIGNAL(clicked()), this, SLOT(loadModel()));
#ifndef QT_NO_CONCURRENT
    connect(&m_modelLoader, SIGNAL(finished()), this, SLOT(modelLoaded()));
#endif
    controls->layout()->addWidget(m_modelButton);

    QPushButton *colorButton = new QPushButton(tr("Choose model color"));
    connect(colorButton, SIGNAL(clicked()), this, SLOT(setModelColor()));
    controls->layout()->addWidget(colorButton);

    QCheckBox *wireframe = new QCheckBox(tr("Render as wireframe"));
    connect(wireframe, SIGNAL(toggled(bool)), this, SLOT(enableWireframe(bool)));
    controls->layout()->addWidget(wireframe);

    QCheckBox *axes = new QCheckBox(tr("Enable Axes"));
    connect(axes, SIGNAL(toggled(bool)), this, SLOT(enableAxis(bool)));
    controls->layout()->addWidget(axes);

    QCheckBox *normals = new QCheckBox(tr("Display normals vectors"));
    connect(normals, SIGNAL(toggled(bool)), this, SLOT(enableNormals(bool)));
    controls->layout()->addWidget(normals);

    QCheckBox *nodes = new QCheckBox(tr("Display all Nodes"));
    connect(nodes, SIGNAL(toggled(bool)), this, SLOT(enableNodes(bool)));
    controls->layout()->addWidget(nodes);


    QPushButton *Post = new QPushButton(tr("Post Analysis"));
    connect(Post, SIGNAL(clicked()), this, SLOT(postdisplay()));
    controls->layout()->addWidget(Post);

    QPushButton *setText = new QPushButton(tr("set text"));
    connect(setText, SIGNAL(clicked()), this, SLOT(setText()));
    controls->layout()->addWidget(setText);

    QPushButton *Pre = new QPushButton(tr("Pre - Analysis"));
    connect(Pre, SIGNAL(clicked()), this, SLOT(predisplay()));
    controls->layout()->addWidget(Pre);

    QPushButton *backgroundButton = new QPushButton(tr("Choose background color"));
    connect(backgroundButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
    controls->layout()->addWidget(backgroundButton);

    QWidget *statistics = createDialog(tr("Model info"));
    statistics->layout()->setMargin(15);

    for (int i = 0; i < 4; ++i) {
        m_labels[i] = new QLabel;
        statistics->layout()->addWidget(m_labels[i]);
    }

    QWidget *conditions = createDialog(tr("Conditions"));
    QCheckBox *boundary = new QCheckBox(tr("Boundary Condition"));
    connect(boundary, SIGNAL(toggled(bool)), this, SLOT(enableBoundary(bool)));
    conditions->layout()->addWidget(boundary);

    QCheckBox *pointleads = new QCheckBox(tr("Point Leads"));
    connect(pointleads, SIGNAL(toggled(bool)), this, SLOT(enablePointleads(bool)));
    conditions->layout()->addWidget(pointleads);

    QCheckBox *pressureloads = new QCheckBox(tr("Pressure Loads"));
    connect(pressureloads, SIGNAL(toggled(bool)), this, SLOT(enablePressureloads(bool)));
    conditions->layout()->addWidget(pressureloads);

    QCheckBox *Potentials = new QCheckBox(tr("Potentials"));
    connect(Potentials, SIGNAL(toggled(bool)), this, SLOT(enablePotentials(bool)));
    conditions->layout()->addWidget(Potentials);

    QCheckBox *temperature = new QCheckBox(tr("Temperature"));
    connect(temperature, SIGNAL(toggled(bool)), this, SLOT(enableTemperature(bool)));
    conditions->layout()->addWidget(temperature);


    QWidget *widgets[] = { conditions, controls, statistics };

    for (uint i = 0; i < sizeof(widgets) / sizeof(*widgets); ++i) {
        QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(0, Qt::Dialog);
        proxy->setWidget(widgets[i]);
        addItem(proxy);
    }

    QPointF pos(10, 10);
    foreach (QGraphicsItem *item, items()) {
        item->setFlag(QGraphicsItem::ItemIsMovable);
        item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

        const QRectF rect = item->boundingRect();
        item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
        pos += QPointF(0, 10 + rect.height());
    }

    QRadialGradient gradient(40, 40, 40, 40, 40);
    gradient.setColorAt(0.2, Qt::yellow);
    gradient.setColorAt(1, Qt::transparent);


    m_lightItem = new QGraphicsRectItem(0, 0, 80, 80);
    m_lightItem->setPen(Qt::NoPen);
    m_lightItem->setBrush(gradient);
    m_lightItem->setFlag(QGraphicsItem::ItemIsMovable);
    m_lightItem->setPos(700, 200);
    addItem(m_lightItem);


    QLinearGradient linGrad( 0, 0, 0, 125);
     linGrad.setColorAt( 0, Qt::red );
     linGrad.setColorAt( 0.2, QColor(255, 153, 102) );
     linGrad.setColorAt(0.4, Qt::yellow);
     linGrad.setColorAt(0.6,Qt::green);
     linGrad.setColorAt(0.8,Qt::blue);

     linGrad.setSpread( QGradient::PadSpread );
     linGrad.setSpread( QGradient::ReflectSpread );


     scale1= new QGraphicsRectItem(0,0,25,150);
     scale1->setPen(Qt::NoPen);
     scale1->setBrush(linGrad);
     scale1->setFlag(QGraphicsItem::ItemIsMovable);
     scale1->setPos(700,300);
     addItem(scale1);

    loadModel(QLatin1String("Model"));
    m_time.start();
}
void OpenGLScene::postdisplay()
{
    loadModel("C:\\Users\\Shruti\\Desktop\\27v.obj");
}
void OpenGLScene::predisplay()
{
    loadModel("C:\\Users\\Shruti\\Desktop\\21v.obj");
}
void OpenGLScene::drawBackground(QPainter *painter, const QRectF &)
{
    if (painter->paintEngine()->type() != QPaintEngine::OpenGL
        && painter->paintEngine()->type() != QPaintEngine::OpenGL2)
    {
        qWarning("OpenGLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
        //return;
    }

    painter->beginNativePainting();

    glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_model) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluPerspective(70, width() / height(), 0.01, 1000);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        const float pos[] = { m_lightItem->x() - width() / 2, height() / 2 - m_lightItem->y(), 512, 0 };
      glLightfv(GL_LIGHT0, GL_POSITION, pos);
     glColor4f(m_modelColor.redF(), m_modelColor.greenF(), m_modelColor.blueF(), m_modelColor.alphaF());

        const int delta = m_time.elapsed() - m_lastTime;
        m_rotation += m_angularMomentum * (delta / 1000.0);
        m_lastTime += delta;

        glTranslatef(0, 0, -m_distance);
        glRotatef(m_rotation.x, 1, 0, 0);
        glRotatef(m_rotation.y, 0, 1, 0);
        glRotatef(m_rotation.z, 0, 0, 1);

        glEnable(GL_MULTISAMPLE);
        m_model->render(m_wireframeEnabled, m_normalsEnabled, m_boundaryEnabled, m_pointleadsEnabled, m_pressureloadsEnabled, m_potentialsEnabled, m_temperatureEnabled, m_axisEnabled, m_nodesEnabled);
        glDisable(GL_MULTISAMPLE);

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }

    painter->endNativePainting();

    QTimer::singleShot(20, this, SLOT(update()));
}

static Model *loadModel(const QString &filePath)
{
    return new Model(filePath);
}

void OpenGLScene::loadModel()
{
    loadModel(QFileDialog::getOpenFileName(0, tr("Choose model"), QString(), QLatin1String("*.obj")));
}

void OpenGLScene::loadModel(const QString &filePath)
{
    if (filePath.isEmpty())
        return;

    m_modelButton->setEnabled(false);
   QApplication::setOverrideCursor(Qt::BusyCursor);
#ifndef QT_NO_CONCURRENT
    m_modelLoader.setFuture(QtConcurrent::run(::loadModel, filePath));
#else
    setModel(::loadModel(filePath));
    modelLoaded();
#endif
}

void OpenGLScene::modelLoaded()
{
#ifndef QT_NO_CONCURRENT
    setModel(m_modelLoader.result());
#endif
    m_modelButton->setEnabled(true);
    QApplication::restoreOverrideCursor();
}

void OpenGLScene::setModel(Model *model)
{
    delete m_model;
    m_model = model;

    m_labels[0]->setText(tr("File: %0").arg(m_model->fileName()));
    m_labels[1]->setText(tr("Points: %0").arg(m_model->points()));
    m_labels[2]->setText(tr("Edges: %0").arg(m_model->edges()));
    m_labels[3]->setText(tr("Faces: %0").arg(m_model->faces()));

    update();
}

void OpenGLScene::enableWireframe(bool enabled)
{
    m_wireframeEnabled = enabled;
    update();
}

void OpenGLScene::enableNormals(bool enabled)
{
    m_normalsEnabled = enabled;
    update();
}

void OpenGLScene::enableBoundary(bool enabled)
{
    m_boundaryEnabled = enabled;
    update();
}

void OpenGLScene::enablePointleads(bool enabled)
{
    m_pointleadsEnabled = enabled;
    update();
}

void OpenGLScene::enablePressureloads(bool enabled)
{
    m_pressureloadsEnabled = enabled;
    update();
}

void OpenGLScene::enablePotentials(bool enabled)
{
    m_potentialsEnabled = enabled;
    update();
}

void OpenGLScene::enableTemperature(bool enabled)
{
    m_temperatureEnabled = enabled;
    update();
}

void OpenGLScene::enableAxis(bool enabled)
{
    m_axisEnabled = enabled;
    update();
}

void OpenGLScene::enableNodes(bool enabled)
{
    m_nodesEnabled = enabled;
    update();
}

void OpenGLScene::setModelColor()
{
    const QColor color = QColorDialog::getColor(m_modelColor);


     if (color.isValid()) {
        m_modelColor = color;
        update();
    }

}

void OpenGLScene::setBackgroundColor()
{
    const QColor color = QColorDialog::getColor(m_backgroundColor);
   //color.addCheckBox(tr("Show alpha channel") , QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        m_backgroundColor = color;
        update();
    }
}

void OpenGLScene::setText()
{

    QPainterPath path;

    QBrush b;
    b.setColor(Qt::red);
    QFont font;
    font.setPixelSize(25);
    font.setBold(true);
    font.setItalic(true);
    font.setPointSize(20);
    font.setFamily("Andalus");
    font.setStyleStrategy(QFont::ForceOutline);
    path.addText(150, 50, font,  "Cube");
    path.fillRule();

    QLinearGradient linGrad( 0, 0, 0, 125);
     linGrad.setColorAt( 0, Qt::red );
     linGrad.setColorAt( 0.2, QColor(255, 153, 102) );
     linGrad.setColorAt(0.4, Qt::yellow);
     linGrad.setColorAt(0.6,Qt::green);
     linGrad.setColorAt(0.8,Qt::blue);
     OpenGLScene::addPath(path,QPen(QBrush(linGrad),1),QBrush(linGrad));

}

void OpenGLScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted())
        return;
    if (event->buttons() & Qt::LeftButton) {
        const QPointF delta = event->scenePos() - event->lastScenePos();
        const Point3d angularImpulse = Point3d(delta.y(), delta.x(), 0) * 0.1;

        m_rotation += angularImpulse;
        m_accumulatedMomentum += angularImpulse;

        event->accept();
        update();
    }
}

void OpenGLScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted())
        return;

    m_mouseEventTime = m_time.elapsed();
    m_angularMomentum = m_accumulatedMomentum = Point3d();
    event->accept();


}

void OpenGLScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->isAccepted())
        return;

    const int delta = m_time.elapsed() - m_mouseEventTime;
    m_angularMomentum = m_accumulatedMomentum * (1000.0 / qMax(1, delta));
    event->accept();
    update();
}

void OpenGLScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsScene::wheelEvent(event);
    if (event->isAccepted())
        return;

    m_distance *= qPow(1.2, -event->delta() / 120);
    event->accept();
    update();
}
