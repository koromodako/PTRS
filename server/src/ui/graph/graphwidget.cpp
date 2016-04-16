/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include "../../utils/logger.h"

#include <math.h>

#include <QKeyEvent>
#include <QUuid>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0), newNodePos(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Elastic Nodes"));

    centerNode = new Node(this, QColor(Qt::darkBlue), QColor(Qt::blue), true);
    scene->addItem(centerNode);
    centerNode->setPos(0, 0);
    centerNode->setToolTip("Server");
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 50);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

#ifndef QT_NO_WHEELEVENT
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    Q_UNUSED(painter);
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void GraphWidget::newClient(QUuid newClientId)
{
    Node *client = seekItem(newClientId);

    if(client == NULL)
    {
        LOG_DEBUG("Now adding client "+newClientId.toString()+" to graph!");

        Node *newNode = new Node(this, Qt::darkRed, Qt::red, newClientId);
        scene()->addItem(newNode);
        scene()->addItem(new Edge(newNode, centerNode));
        newNode->setPos(newNodePos++, 10);
        newNode->setToolTip("Client, id "+newClientId.toString());
    }
    else
    {
        clientNotWorkingAnymore(newClientId);
    }
}

void GraphWidget::newCalculation(QUuid calculationId, QString name)
{
    LOG_DEBUG("Now adding calc "+calculationId.toString()+" to graph!");

    Node *newNode = new Node(this, Qt::darkGreen, Qt::green, calculationId);
    scene()->addItem(newNode);
    newNode->setPos(newNodePos++, 10);
    newNode->setToolTip("Calculation "+name+", id "+calculationId.toString());
}

void GraphWidget::clientWorkingOnCalculation(QUuid calculationId, QUuid clientId)
{
    LOG_DEBUG("Now linking calculation "+calculationId.toString()+" to client "+clientId.toString()+"!");

    Node *calculation = seekItem(calculationId);
    Node *client = seekItem(clientId);

    if(calculation == NULL || client == NULL)
    {
        LOG_ERROR("Error: Calculation or client does not exist on graph!");
    }
    else
    {
        scene()->addItem(new Edge(client, calculation));
    }
}

void GraphWidget::clientNotWorkingAnymore(QUuid clientId)
{
    LOG_DEBUG("Now removing links from "+clientId.toString()+"!");

    Node *client = seekItem(clientId);

    if(client == NULL)
    {
        LOG_ERROR("Error: Client does not exist on graph!");
    }
    else
    {
        for(Edge *e : client->edges())
        {
            if(!e->destNode()->isServer())
            {
                //it is a calculation (not the server)
                e->destNode()->removeEdge(e);
                client->removeEdge(e);

                if(e->scene() == scene()) scene()->removeItem(e);
            }
        }
    }
}


void GraphWidget::deleteClient(QUuid clientId)
{
    LOG_DEBUG("Now removing client "+clientId.toString()+"!");

    Node *client = seekItem(clientId);

    if(client == NULL)
    {
        LOG_ERROR("Error: Client does not exist on graph!");
    }
    else
    {
        for(Edge *e : client->edges())
        {
            if(e->scene() == scene()) scene()->removeItem(e);
        }
        client->removeEdges();

        if(client->scene() == scene()) scene()->removeItem(client);
    }
}

void GraphWidget::deleteCalculation(QUuid calculationId)
{
    LOG_DEBUG("Now removing calculation "+calculationId.toString()+"!");

    Node *calculation = seekItem(calculationId);

    if(calculation == NULL)
    {
        LOG_ERROR("Error: Calculation does not exist on graph!");
    }
    else
    {
        for(Edge *e : calculation->edges())
        {
            if(e->scene() == scene()) scene()->removeItem(e);
        }
        calculation->removeEdges();

        if(calculation->scene() == scene()) scene()->removeItem(calculation);
    }
}

Node * GraphWidget::seekItem(QUuid itemId)
{
    Node *foundNode = NULL;

    for(QGraphicsItem *item : items())
    {
        if (Node * node = dynamic_cast<Node *>(item))
        {
            if(node->getObjectId() == itemId)
            {
                foundNode = node;
                break;
            }
        }

    }

    return foundNode;
}
