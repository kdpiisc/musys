#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QVector>

#include <math.h>
#include<GL/glu.h>
#include "point3d.h"

class Model
{
public:
    Model() {}
    Model(const QString &filePath);

    void render(bool wireframe = false, bool normals = false, bool boundary = false, bool pointleads = false, bool pressureloads = false, bool potentials = false, bool temperature = false, bool axis = false, bool nodes = false) const;

    QString fileName() const { return m_fileName; }
    int faces() const { return m_pointIndices.size() / 3; }
    int edges() const { return m_edgeIndices.size() / 2; }
    int points() const { return m_points.size(); }

    float length=0.1;


float d[6];
private:
    QString m_fileName;
    QVector<Point3d> m_points;
    QVector<Point3d> m_pointlead;
    QVector<Point3d> m_normals;
    QVector<Point3d> m_dir;
    QVector<Point3d> m_pointloads;
    QVector<Color3d> m_colors;

    QVector<Color3d> m_colorsb;
    QVector<Color3d> m_colorsp;
    QVector<Color3d> m_colorspl;
    QVector<Color3d> m_colorsprl;
    QVector<Color3d> m_colorst;
    QVector<Point3d> m_boundary;

    QVector<int> m_edgeIndices;
    QVector<int> m_pointIndices;
    QVector<int> m_pointIndicesp;
    QVector<int> m_pointIndicest;
    QVector<int> m_pointIndicesq;
     QVector<int> m_pointIndicesb;
     QVector<int> m_pointIndicesk;
     QVector<int> m_pointIndicesprl;
     QVector<int> m_edgeIndicesb;
};

#endif
