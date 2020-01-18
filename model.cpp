#include "model.h"

#include <QFile>
#include <QTextStream>
#include <QVarLengthArray>
#define RADPERDEG 0.017453292
#include <QtOpenGL>

Model::Model(const QString &filePath)
    : m_fileName(QFileInfo(filePath).fileName())
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    Point3d boundsMin( 1e9, 1e9, 1e9);
    Point3d boundsMax(-1e9,-1e9,-1e9);

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString input = in.readLine();
        if (input.isEmpty() || input[0] == '#')
            continue;

        QTextStream ts(&input);
        QString id;
        ts >> id;
        if (id == "v") {
            Point3d p;
            for (int i = 0; i < 3; ++i) {
                ts >> p[i];
                boundsMin[i] = qMin(boundsMin[i], p[i]);
                boundsMax[i] = qMax(boundsMax[i], p[i]);
            }
            m_points << p;
        } else if (id == "f" || id == "fo") {
            QVarLengthArray<int, 4> p;

            while (!ts.atEnd()) {
                QString vertex;
                ts >> vertex;
                const int vertexIndex = vertex.split('/').value(0).toInt();
                if (vertexIndex)
                    p.append(vertexIndex > 0 ? vertexIndex - 1 : m_points.size() + vertexIndex);
            }

            for (int i = 0; i < p.size(); ++i) {
                const int edgeA = p[i];
                const int edgeB = p[(i + 1) % p.size()];

                if (edgeA < edgeB)
                    m_edgeIndices << edgeA << edgeB;
            }

            for (int i = 0; i < 3; ++i)
                m_pointIndices << p[i];

            if (p.size() == 4)
                for (int i = 0; i < 3; ++i)
                    m_pointIndices << p[(i + 2) % 4];
        }
        else if(id == "c")
        {
            Color3d colord;
            while(!ts.atEnd()){
                for (int i = 0; i < 4; ++i) {

                    ts>>colord[i];
                }
                m_colors<<colord;

            }
        }
        else if(id == "b")
        {
             QVarLengthArray<int, 4> pb;

                        while (!ts.atEnd()) {
                            QString vertexb;
                            ts >> vertexb;
                            const int vertexIndexb = vertexb.split('/').value(0).toInt();
                            if (vertexIndexb)
                                pb.append(vertexIndexb > 0 ? vertexIndexb - 1 : m_points.size() + vertexIndexb);
                        }


                        for (int i = 0; i < 3; ++i)
                            m_pointIndicesb << pb[i];

                        if (pb.size() == 4)
                            for (int i = 0; i < 3; ++i)
                                m_pointIndicesb << pb[(i + 2) % 4];

        }
        else if(id == "p")
        {
             QVarLengthArray<int, 4> pp;

                        while (!ts.atEnd()) {
                            QString vertexp;
                            ts >> vertexp;
                            const int vertexIndexp = vertexp.split('/').value(0).toInt();
                            if (vertexIndexp)
                                pp.append(vertexIndexp > 0 ? vertexIndexp - 1 : m_points.size() + vertexIndexp);
                        }


                        for (int i = 0; i < 3; ++i)
                            m_pointIndicesp<< pp[i];

                        if (pp.size() == 4)
                            for (int i = 0; i < 3; ++i)
                                m_pointIndicesp << pp[(i + 2) % 4];

        }
        else if(id == "t")
        {
             QVarLengthArray<int, 4> pt;

                        while (!ts.atEnd()) {
                            QString vertext;
                            ts >> vertext;
                            const int vertexIndext = vertext.split('/').value(0).toInt();
                            if (vertexIndext)
                                pt.append(vertexIndext > 0 ? vertexIndext - 1 : m_points.size() + vertexIndext);
                        }


                        for (int i = 0; i < 3; ++i)
                            m_pointIndicest << pt[i];

                        if (pt.size() == 4)
                            for (int i = 0; i < 3; ++i)
                                m_pointIndicest << pt[(i + 2) % 4];

        }
        else if(id == "q")
        {
             QVarLengthArray<int, 4> pt;

                        while (!ts.atEnd()) {
                            QString vertext;
                            ts >> vertext;
                            const int vertexIndext = vertext.split('/').value(0).toInt();
                            if (vertexIndext)
                                pt.append(vertexIndext > 0 ? vertexIndext - 1 : m_points.size() + vertexIndext);
                        }


                        for (int i = 0; i < 3; ++i)
                            m_pointIndicesq << pt[i];

                        if (pt.size() == 4)
                            for (int i = 0; i < 3; ++i)
                                m_pointIndicesq << pt[(i + 2) % 4];

        }
        else if(id == "u")
        {
            Color3d colorb;
            while(!ts.atEnd()){
                for (int i = 0; i < 4; ++i) {

                    ts>>colorb[i];
                }
                m_colorsb<<colorb;

            }
        }
        else if(id == "m")
        {
            Color3d colorp;
            while(!ts.atEnd()){
                for (int i = 0; i < 4; ++i) {

                    ts>>colorp[i];
                }
                m_colorsp<<colorp;

            }
        }
        else if(id == "w")
        {
            Color3d colort;
            while(!ts.atEnd()){
                for (int i = 0; i < 4; ++i) {

                    ts>>colort[i];
                }
                m_colorst<<colort;

            }
        }
        else if(id == "a")
        {
            Color3d colorpl;
            while(!ts.atEnd()){
                for (int i = 0; i < 4; ++i) {

                    ts>>colorpl[i];
                }
                m_colorspl<<colorpl;

            }
        }
        else  if (id == "d") {

            for (int i = 0; i <6; ++i) {
                in >> d[i];
             }
        }
        else if(id == "k")
        {
            Point3d point;
            while(!ts.atEnd()){
                for (int i = 0; i < 3; ++i) {

                    ts>>point[i];
                }
                m_pointloads<<point;

            }
        }
        else if(id == "n")
        {
             QVarLengthArray<int, 4> pt;

                        while (!ts.atEnd()) {
                            QString vertext;
                            ts >> vertext;
                            const int vertexIndext = vertext.split('/').value(0).toInt();
                            if (vertexIndext)
                                pt.append(vertexIndext > 0 ? vertexIndext - 1 : m_points.size() + vertexIndext);
                        }


                        for (int i = 0; i < 3; ++i)
                            m_pointIndicesk << pt[i];

                        if (pt.size() == 4)
                            for (int i = 0; i < 3; ++i)
                                m_pointIndicesk << pt[(i + 2) % 4];

        }

    }

    const Point3d bounds = boundsMax - boundsMin;
    const qreal scale = 1 / qMax(bounds.x, qMax(bounds.y, bounds.z));
    for (int i = 0; i < m_points.size(); ++i)
        m_points[i] = (m_points[i] - (boundsMin + bounds * 0.5)) * scale;

    m_normals.resize(m_points.size());
    for (int i = 0; i < m_pointIndices.size(); i += 3) {
        const Point3d a = m_points.at(m_pointIndices.at(i));
        const Point3d b = m_points.at(m_pointIndices.at(i+1));
        const Point3d c = m_points.at(m_pointIndices.at(i+2));

        const Point3d normal = cross(b - a, c - a).normalize();

        for (int j = 0; j < 3; ++j)
            m_normals[m_pointIndices.at(i + j)] += normal;
    }

    for (int i = 0; i < m_normals.size(); ++i)
        m_normals[i] = m_normals[i].normalize();




}

void Model::render(bool wireframe, bool normals, bool boundary, bool pointleads, bool pressureloads, bool potentials, bool temperature, bool axis, bool nodes) const
{




    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    if (wireframe) {
        glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
        glDrawElements(GL_LINES, m_edgeIndices.size(), GL_UNSIGNED_INT, m_edgeIndices.data());
    } else {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        //glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearDepth(1);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glEnable( GL_COLOR_ARRAY);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glColorPointer(4,GL_FLOAT,0,(float *)m_colors.data());
        glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
        glNormalPointer(GL_FLOAT, 0, (float *)m_normals.data());


        glDrawElements(GL_TRIANGLES, m_pointIndices.size(), GL_UNSIGNED_INT, m_pointIndices.data());


        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisable( GL_COLOR_ARRAY);

        glEnable(GL_BLEND);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);

     }


    if (normals) {
        QVector<Point3d> normals;
        for (int i = 0; i < m_normals.size(); ++i)
            normals << m_points.at(i) << (m_points.at(i) + m_normals.at(i) * 0.02f);
        glVertexPointer(3, GL_FLOAT, 0, (float *)normals.data());
        glDrawArrays(GL_LINES, 0, normals.size());
    }

    if (boundary)
    {
            glPushMatrix();
            glEnable( GL_COLOR_ARRAY);
            glColorPointer(4,GL_FLOAT,0,(float *)m_colorsb.data());
            glPointSize(5);
            glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
            glDrawElements(GL_POINTS, m_pointIndicesb.size(), GL_UNSIGNED_INT, m_pointIndicesb.data());
            glDisable( GL_COLOR_ARRAY);
            glPopMatrix();
    }
    if (pointleads)
    {
            glPushMatrix();

            glEnable( GL_COLOR_ARRAY);
            glColorPointer(4,GL_FLOAT,0,(float *)m_colorspl.data());
            glPointSize(5);
            glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
            glDrawElements(GL_LINES, m_pointIndicesk.size(), GL_UNSIGNED_INT, m_pointIndicesk.data());
            glDisable( GL_COLOR_ARRAY);

            glPopMatrix();


    }
    if (pressureloads)
    {

    }
    if (potentials)
    {
        glPushMatrix();
        glEnable( GL_COLOR_ARRAY);
        glColorPointer(3,GL_FLOAT,0,(float *)m_colorsp.data());
        glPointSize(10);
        glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
        glDrawElements(GL_POINTS, m_pointIndicesp.size(), GL_UNSIGNED_INT, m_pointIndicesp.data());
        glDisable( GL_COLOR_ARRAY);
        glPopMatrix();

    }
    if (temperature)
    {
        glPushMatrix();
        glEnable( GL_COLOR_ARRAY);
        glColorPointer(4,GL_FLOAT,0,(float *)m_colorst.data());
        glPointSize(15);
        glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
        glDrawElements(GL_POINTS, m_pointIndicest.size(), GL_UNSIGNED_INT, m_pointIndicest.data());
        glDisable( GL_COLOR_ARRAY);
        glPopMatrix();
    }
    if (nodes)
    {
        glPushMatrix();
        glEnable(GL_COLOR_ARRAY);

        glColorPointer(4,GL_FLOAT,0,(float *)m_colors.data());
        glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
        glDrawElements(GL_POINTS, m_pointIndicesq.size(), GL_UNSIGNED_INT, m_pointIndicesq.data());

        glDisable( GL_COLOR_ARRAY);
        glPopMatrix();
    }
    if(axis)
    {
        glViewport(650,0,150,150);
        glClearColor(1.0,1.0,1.0,1.0);

       glDisable(GL_DEPTH_TEST); // if you want axes on top
      glPushMatrix();

      glEnable(GL_NORMALIZE);
        glScalef(.01,.01,.01);
       glTranslatef(-1,-1,0);
        glLineWidth(3);
        GLUquadricObj *quadObj;
        quadObj = gluNewQuadric ();
        gluQuadricDrawStyle (quadObj, GLU_FILL);
        gluQuadricNormals (quadObj, GLU_SMOOTH);
        gluSphere(quadObj,0.2,32,32);

        glBegin(GL_LINES);

        glColor4ub(255, 0, 0, 255);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(70.0f, 0.0f, 0.0f);

        glColor4ub(0, 255, 0, 255);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 70.0f, 0.0f);

        glColor4ub(0, 0, 255, 255);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 70.0f);

        glEnd();
        glDisable(GL_NORMALIZE);
        glBegin(GL_LINES);

         glColor4ub(255, 0, 0, 255);
        glVertex3f(63.0f,7.0f,0.0);
        glVertex3f(70.0f, 0.0f, 0.0f);
        glVertex3f(70.0f, 0.0f, 0.0f);
        glVertex3f(63.0f,-7.0f,0.0f);


        glColor4ub(0, 255, 0, 255);
        glVertex3f(7.0f,63.0f,0.0);
       glVertex3f(0.0f, 70.0f, 0.0f);
       glVertex3f(0.0f, 70.0f, 0.0f);
       glVertex3f(-7.0f,63.0f,0.0f);


      glColor4ub(0, 0, 255, 255);
      glVertex3f(7.0f,0.0,63.0f);
       glVertex3f(0.0f, 0.0f, 70.0f);
        glVertex3f(0.0f, 0.0f, 70.0f);
      glVertex3f(-7.0f,0.0f,63.0f);

        glEnd();

        glLineWidth(1);
        glBegin(GL_LINES);


         glColor4ub(255, 255, 255, 255);
        glVertex3f(73.0f,9.0f,0.0);
        glVertex3f(77.0f,-9.0f,0.0f);

        glVertex3f(77.0f,9.0f,0.0);
       glVertex3f(73.0f,-9.0f,0.0f);


       glColor4ub(255, 255, 255, 255);
       glVertex3f(-5.0f,77.0f,0.0);
       glVertex3f(0.0f,75.0f,0.0f);

       glVertex3f(5.0f,77.0f,0.0);
       glVertex3f(0.0f,75.0f,0.0f);

       glVertex3f(0.0f,72.0f,0.0f);
       glVertex3f(0.0f,75.0f,0.0f);


      glColor4ub(255, 255, 255, 255);

      glVertex3f(0.0,9.0f,77.0f);
      glVertex3f(0.0,9.0f,72.0f);

       glVertex3f(0.0,9.0f,72.0f);
        glVertex3f(0.0,-9.0f,77.0f);

      glVertex3f(0.0,-9.0f,77.0f);
      glVertex3f(0.0,-9.0f,72.0f);



        glEnd();

       glEnable(GL_DEPTH_TEST);// draw axis will use lines.
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
   // glDisable(GL_DEPTH_TEST);
}

